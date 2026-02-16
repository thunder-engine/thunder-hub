#include "installmodel.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QFileDialog>
#include <QProcess>
#include <QDirIterator>

#include <QDebug>

#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QRegularExpression>

#include "downloader.h"

namespace {
    const char *gInstalls("Installs");

    const char *gTagName("tag_name");
    const char *gAssets("assets");
    const char *gUrl("browser_download_url");

    const char *gWindows("windows");
    const char *gLinux("linux");
    const char *gMacOSX("macosx");

    const char *gName("name");
    const char *gSize("size");
}

void Sdk::checkInstalled() {
    static const QStringList ignoreList = {".", "..", "include", "resources"};
    static const QMap<QString, QString> aliases = {
        {"emscripten", "webgl"}
    };

    installed = QFileInfo::exists(path);
    root = QFileInfo(path).path() + "/../../../";

    QDirIterator it(root);
    while(it.hasNext()) {
        QString name(QFileInfo(it.next()).fileName());
        if(!ignoreList.contains(name)) {
            bool installed = false;
            QString platform(aliases.value(name, name));
            for(auto &module : modules) {
                if(module.platform == platform) {
                    module.installed = true;
                    installed = true;
                }
            }

            if(!installed) {
                Module module;
                module.platform = platform;
                module.installed = true;

                modules << module;
            }
        }
    }
}

InstallModel::InstallModel() :
        QAbstractListModel(),
        m_manager(new QNetworkAccessManager) {

    QSettings settings(COMPANY_NAME, EDITOR_NAME);

    QVariant value = settings.value(gInstalls);
    if(value.isValid()) {
        foreach(auto it, value.toStringList()) {
            QStringList pair = it.split(';');
            Sdk sdk;
            sdk.path = pair.front();
            sdk.version = pair.back();
            if(QFileInfo::exists(sdk.path)) {
                sdk.checkInstalled();
                m_sdk.push_back(sdk);
            }
        }
    }

    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl("https://api.github.com/repos/thunder-engine/thunder/releases")));
    connect(reply, &QNetworkReply::finished, this, &InstallModel::onUpdateCheckFinished);
}

InstallModel *InstallModel::instance() {
    static InstallModel instance;

    return &instance;
}

QString InstallModel::defaultSdk() const {
    for(auto &it : m_sdk) {
        if(it.installed) {
            return it.path;
        }
    }
    return QString();
}

int InstallModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }

    int count = 0;
    for(auto &it : m_sdk) {
        if(it.installed) {
            count++;
        }
    }

    return count;
}

QVariant InstallModel::data(const QModelIndex &index, int role) const {
    if(index.isValid()) {
        int idx = -1;
        for(auto &it : m_sdk) {
            if(it.installed) {
                idx++;
                if(idx == index.row()) {
                    QFileInfo info(it.path);
                    switch(role) {
                        case VersionRole:  { return it.version; }
                        case PathRole:     { return info.absoluteFilePath(); }
                        case ProgressRole: { return it.progress; }
                        case StatusRole:   { return it.status; }
                        default: break;
                    }
                }
            }
        }
    }
    return QVariant();
}

QHash<int, QByteArray> InstallModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[VersionRole]  = "version";
    roles[PathRole]     = "path";
    roles[ProgressRole] = "progress";
    roles[StatusRole]   = "status";

    return roles;
}

QStringList InstallModel::sdkVersions() {
    QStringList list;
    foreach(auto &it, m_sdk) {
        list << it.version;
    }

    list.resize(6);

    return list;
}

void InstallModel::onUpdateCheckFinished() {
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if(reply) {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        foreach(auto it, document.array()) {
            QJsonObject release = it.toObject();
            QString version(release.value(gTagName).toString());

            foreach(auto asset, release.value(gAssets).toArray()) {
                QJsonObject fields = asset.toObject();

                Module m;
                m.url = fields.value(gUrl).toString();
                m.platform = fields.value(gName).toString();
                m.size = QString::number(fields.value(gSize).toInt() / 1024 / 1024) + " MB";

                static const QRegularExpression reg("ThunderEngine-(\\w+)|-(\\w+).7z");
                auto globalMatch = reg.globalMatch(m.platform);
                while(globalMatch.hasNext()) {
                    auto match = globalMatch.next();
                    m.platform = match.captured(1);
                    break;
                }

#ifdef Q_OS_WINDOWS
                if(m.platform == gWindows) {
                    m.optional = false;
                }
#elif defined(Q_OS_LINUX)
                if(m.platform == gLinux) {
                    m.optional = false;
                }
#elif defined(Q_OS_MACOS)
                if(m.platform == gMacOSX) {
                    m.optional = false;
                }
#endif
                static const QMap<QString, QString> names = {{"android", "Android Platform Support"},
                                                             {"tvos", "tvOS Platform Support"},
                                                             {"ios", "iOS Platform Support"},
                                                             {"webgl", "HTML5 Platform Support"}};

                m.name = names.value(m.platform, m.platform);
                if(m.name == m.platform) {
                    m.name.front() = m.name.front().toUpper();
                    m.name += " Platform and Editor";

                    if(m.optional) {
                        continue;
                    }
                }

                Sdk *sdk = getSdk(version);
                if(sdk) {
                    bool installed = false;
                    for(auto &mod : sdk->modules) {
                        if(mod.platform == m.platform) {
                            m.installed = mod.installed;
                            mod = m;
                            installed = true;
                            break;
                        }
                    }

                    if(!installed) {
                        if(m.optional) {
                            sdk->modules.push_back(m);
                        } else {
                            sdk->modules.push_front(m);
                        }
                    }
                    std::sort(sdk->modules.begin(), sdk->modules.end(), [](const Module &left, const Module &right) {
                        return left.optional < right.optional;
                    });
                } else {
                    Sdk sdk;

                    sdk.version = version;
                    if(m.optional) {
                        sdk.modules.push_back(m);
                    } else {
                        sdk.modules.push_front(m);
                    }
                    std::sort(sdk.modules.begin(), sdk.modules.end(), [](const Module &left, const Module &right) {
                        return left.optional < right.optional;
                    });

                    m_sdk.push_back(sdk);
                }
            }
        }
    }

    emit sdkListChanged();
}

Sdk *InstallModel::getSdk(const QString &version) {
    auto it = std::find_if(m_sdk.begin(), m_sdk.end(), [version](const Sdk &sdk) { return sdk.version == version; });
    if(it != m_sdk.end()) {
        return &(*it);
    }
    return nullptr;
}

const QList<Module> InstallModel::getModules(const QString &version) {
    Sdk *sdk = getSdk(version);
    if(sdk) {
        return sdk->modules;
    }

    return QList<Module>();
}

QStringList InstallModel::installedModules(const QString &version) {
    QStringList result;
    Sdk *sdk = getSdk(version);
    if(sdk) {
        foreach(auto &it, sdk->modules) {
            if(it.installed) {
                result << it.platform;
            }
        }
    }

    return result;
}

void InstallModel::locateSdk() {
    QString name(EDITOR_NAME);
#ifdef Q_OS_WIN
    name += ".exe";
#endif

    QString path = QFileDialog::getOpenFileName(nullptr, tr("Locate the ") + EDITOR_NAME, "/", name);
    if(!path.isEmpty()) {
        QFileInfo info(path);

        QProcess probe(this);
        probe.setWorkingDirectory(info.absolutePath());
        probe.start(info.absolutePath() + "/Builder", {"-v"});
        if(probe.waitForStarted() && probe.waitForFinished()) {
            QString version = probe.readAllStandardOutput().constData();
            version = version.split(' ').last().trimmed();

            Sdk *sdk = getSdk(version);
            if(sdk) {
                sdk->path = QDir::fromNativeSeparators(path);
                sdk->installed = true;
                sdk->checkInstalled();
            } else {
                Sdk sdk;
                sdk.path = QDir::fromNativeSeparators(path);
                sdk.version = version;
                sdk.installed = true;
                sdk.checkInstalled();

                m_sdk.push_front(sdk);
            }
            commitInstallRecord();
        } else {
            qDebug() << "Can't find a valid SDK installation";
        }
    }
}

void InstallModel::updateSdk(const QString &version, const QStringList &modules) {
    Sdk *sdk = getSdk(version);

    if(sdk) {
        QStringList downloads;
        for(auto &it : sdk->modules) {
            if(modules.contains(it.platform)) {
                if(it.installed) {
                    continue;
                }
                downloads << it.url;
            }
        }

        static const QMap<QString, QString> aliases = {
            {"webgl", "emscripten"}
        };
        for(auto &it : sdk->modules) {
            if(!modules.contains(it.platform)) {
                QDir dir(sdk->root + QDir::separator() + aliases.value(it.platform, it.platform));
                dir.removeRecursively();
                it.installed = false;
            }
        }

        if(!downloads.isEmpty()) {
            Downloader *job = new Downloader(m_manager, this);
            job->get(sdk, downloads);

            connect(job, &Downloader::jobUpdated, this, &InstallModel::onJobUpdated);
            connect(job, &Downloader::jobFinished, this, &InstallModel::onJobFinished);

            m_jobs.append(job);

            emit layoutAboutToBeChanged();
            emit layoutChanged();
        }
    }
}

void InstallModel::uninstallSdk(const QString &version) {
    Sdk *sdk = getSdk(version);
    if(sdk) {
        QDir dir(sdk->root);
        dir.removeRecursively();

        sdk->installed = false;
        commitInstallRecord();
    }
}

void InstallModel::explorer(const QString &version) {
    Sdk *sdk = getSdk(version);
    if(sdk == nullptr) {
        return;
    }

    QString path(sdk->path);

#if defined(Q_OS_WIN)
    QProcess::startDetached("explorer.exe", { "/select,", QDir::toNativeSeparators(path) });
#elif defined(Q_OS_MAC)
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                      .arg(path);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute("/usr/bin/osascript", scriptArgs);
#elif defined(Q_OS_LINUX)
    QProcess::execute(QLatin1String("xdg-open"), {path});
#endif
}

void InstallModel::commitInstallRecord() {
    QSettings settings(COMPANY_NAME, EDITOR_NAME);

    QStringList values;
    foreach(auto &it, m_sdk) {
        if(it.installed) {
            values << it.path + ";" + it.version;
        }
    }
    values.removeDuplicates();
    settings.setValue(gInstalls, values);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void InstallModel::onJobFinished() {
    Downloader *job = dynamic_cast<Downloader *>(sender());
    if(job) {
        m_jobs.removeOne(job);
        job->deleteLater();
    }

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void InstallModel::onJobUpdated() {
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}
