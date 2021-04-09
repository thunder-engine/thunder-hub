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

const QString gInstalls("launcher.installs");
const QString gTarget("launcher.target");

const QString gEditor("WorldEditor.exe");

InstallModel::InstallModel() :
        QAbstractListModel() {

    QSettings settings(COMPANY_NAME, EDITOR_NAME);

    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    defaultPath.append("/");
    defaultPath.append(COMPANY_NAME);
    m_TargetDir = settings.value(gTarget, defaultPath).toString();

    QVariant value = settings.value(gInstalls);
    if(value.isValid()) {
        m_List = value.toStringList();
        for(auto it : m_List) {
            QStringList pair = it.split(';');
            if(!QFileInfo(pair.front()).exists()) {
                m_List.removeAll(it);
            }
        }
    }

    QFile file(":/versions.json");
    if(file.open(QIODevice::ReadOnly)) {
        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        file.close();
        QJsonObject root = document.object();
        for(auto it : root.value("SDK").toArray()) {
            QJsonObject sdk = it.toObject();
            m_Versions << sdk.value("Version").toString();
        }
    }
}

InstallModel::~InstallModel() {
    QSettings settings(COMPANY_NAME, EDITOR_NAME);
    settings.setValue(gInstalls, m_List);
}

QString InstallModel::defaultSdk() const {
    if(!m_List.isEmpty()) {
        return m_List.back();
    }
    return QString();
}

QStringList InstallModel::sdkVersions() {
    return m_Versions;
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
    QStringList pair = m_List.at(index.row()).split(';');
    QFileInfo info(pair.front());
    switch(role) {
        case VersionRole: { return pair.last(); }
        case PathRole: { return info.absoluteFilePath(); }
        case ProgressRole: { return 0; }
        default: break;
    }
    return QVariant();
}

QHash<int, QByteArray> InstallModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[VersionRole]  = "version";
    roles[PathRole]     = "path";
    roles[ProgressRole] = "progress";

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
            m_List.push_front(QDir::fromNativeSeparators(path) + ";" + version);
            commitInstallRecord();
        } else {
            qDebug() << "Can't find a valid SDK installation";
        }
    }
}

void InstallModel::installSdk(const QString &version) {
    Q_UNUSED(version)

    if(!m_List.contains(version)) {
        Downloader *job = new Downloader(this);

        QString url = QString("https://github.com/thunder-engine/thunder/releases/download/%1/ThunderEngine-windows-x64.7z").arg(version);

        job->get(m_TargetDir, QUrl(url));
        connect(job, &Downloader::finished, this, &InstallModel::onDownloadFinished);
        connect(job, &Downloader::updateDownloadProgress, this, &InstallModel::onDownloadUpdated);

        m_Jobs.append(job);
    }
}

void InstallModel::uninstallSdk(const QString &path) {
    for(auto it : m_List) {
        if(it.contains(path)) {
            m_List.removeAll(it);
            commitInstallRecord();
            return;
        }
    }
}

void InstallModel::commitInstallRecord() {
    m_List.removeDuplicates();
    QSettings settings(COMPANY_NAME, EDITOR_NAME);
    settings.setValue(gInstalls, m_List);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void InstallModel::onDownloadFinished() {
    Downloader *job = dynamic_cast<Downloader *>(sender());
    m_Jobs.removeOne(job);
    job->deleteLater();
    qDebug() << "onDownloadFinished";
}

void InstallModel::onDownloadUpdated(int64_t bytesReceived, int64_t bytesTotal) {
    qDebug() << "onDownloadUpdated" << bytesReceived << bytesTotal;
}
