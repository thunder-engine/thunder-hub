#include "installmodel.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QFileDialog>
#include <QProcess>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>

#include "downloader.h"

namespace {
    const QString gInstalls("launcher.installs");
    const QString gTarget("launcher.target");

    const QString gEditor("WorldEditor.exe");
}

InstallModel::InstallModel() :
        QAbstractListModel() {

    QSettings settings(COMPANY_NAME, EDITOR_NAME);

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    defaultPath.append("/");
    defaultPath.append(COMPANY_NAME);
    m_TargetDir = settings.value(gTarget, defaultPath).toString();

    QVariant value = settings.value(gInstalls);
    if(value.isValid()) {
        for(auto it : value.toStringList()) {
            QStringList pair = it.split(';');
            Sdk sdk;
            sdk.path = pair.front();
            sdk.version = pair.back();
            sdk.progress = -1;
            if(QFileInfo(sdk.path).exists()) {
                m_List.push_back(sdk);
            }
        }
    }
}

InstallModel::~InstallModel() {
    commitInstallRecord();
}

QString InstallModel::defaultSdk() const {
    if(!m_List.isEmpty()) {
        return m_List.back().path;
    }
    return QString();
}

int InstallModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }
    return m_List.size();
}

QVariant InstallModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    Sdk sdk = m_List.at(index.row());
    QFileInfo info(sdk.path);
    switch(role) {
        case VersionRole:  { return sdk.version; }
        case PathRole:     { return info.absoluteFilePath(); }
        case ProgressRole: { return sdk.progress; }
        case StatusRole:   { return sdk.status; }
        default: break;
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

void InstallModel::locateSdk() {
    QString path = QFileDialog::getOpenFileName(nullptr, tr("Locate the WorldEditor"), "/", gEditor);
    if(!path.isEmpty()) {
        QFileInfo info(path);

        QProcess probe(this);
        probe.setWorkingDirectory(info.absolutePath());

        QStringList args;
        args << "-v";

        probe.start(info.absolutePath() + "/Builder", args);
        if(probe.waitForStarted() && probe.waitForFinished()) {
            QString version = probe.readAllStandardOutput().constData();
            version = version.split(' ').last().trimmed();
            Sdk sdk;
            sdk.path = QDir::fromNativeSeparators(path);
            sdk.version = version;
            sdk.progress = -1;
            m_List.push_front(sdk);
            commitInstallRecord();
        } else {
            qDebug() << "Can't find a valid SDK installation";
        }
    }
}

void InstallModel::installSdk(const QString &version, const QStringList &components) {
    Q_UNUSED(version)

    for(auto &it : m_List) {
        if(it.version == version) {
            return;
        }
    }

    Downloader *job = new Downloader(this);

    QStringList urls;
    for(auto &it : components) {
        urls << m_Modules.value(version).value(it);
    }

    job->get(m_TargetDir, version, urls);
    connect(job, &Downloader::updateDownloadProgress, this, &InstallModel::onDownloadUpdated);
    connect(job, &Downloader::updateExtractProgress, this, &InstallModel::onExtractingUpdated);
    connect(job, &Downloader::jobFinished, this, &InstallModel::onJobFinished);

    m_Jobs.append(job);

    Sdk sdk;
    sdk.path = m_TargetDir;
    sdk.version = version;
    sdk.progress = 0;
    m_List.push_front(sdk);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void InstallModel::uninstallSdk(const QString &path) {
    for(auto it : m_List) {
        if(it.path == path) {
            m_List.removeAll(it);
            commitInstallRecord();
            return;
        }
    }
}

void InstallModel::commitInstallRecord() {
    QSettings settings(COMPANY_NAME, EDITOR_NAME);

    QStringList values;
    for(auto it : m_List) {
        values << it.path + ";" + it.version;
    }
    values.removeDuplicates();
    settings.setValue(gInstalls, values);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void InstallModel::onJobFinished() {
    Downloader *job = dynamic_cast<Downloader *>(sender());
    if(job) {
        for(auto &it : m_List) {
            if(it.version == job->version()) {
                it.progress = -1;
                it.status.clear();

                emit layoutAboutToBeChanged();
                emit layoutChanged();

                break;
            }
        }

        m_Jobs.removeOne(job);
        job->deleteLater();
    }
}

void InstallModel::onDownloadUpdated(float value) {
    Downloader *job = dynamic_cast<Downloader *>(sender());
    if(job) {
        for(auto &it : m_List) {
            if(it.version == job->version()) {
                int progress = value * 50;
                if(progress != it.progress) {
                    it.progress = progress;
                    it.status = tr("Downloading...");

                    emit layoutAboutToBeChanged();
                    emit layoutChanged();

                    return;
                }
            }
        }
    }
}

void InstallModel::onExtractingUpdated(float value) {
    Downloader *job = dynamic_cast<Downloader *>(sender());
    if(job) {
        for(auto &it : m_List) {
            if(it.version == job->version()) {
                int progress = 50 + value * 50;
                if(progress != it.progress) {
                    it.progress = progress;
                    it.status = tr("Extracting...");

                    emit layoutAboutToBeChanged();
                    emit layoutChanged();

                    return;
                }
            }
        }
    }
}
