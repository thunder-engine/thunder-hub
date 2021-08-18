#include "downloader.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

#include <QDebug>

Downloader::Downloader(QObject *parent) :
        QObject(parent),
        m_currentReply(nullptr) {
    connect(&m_manager, &QNetworkAccessManager::finished, this, &Downloader::onReply);

    connect(&m_extractor, &Extractor::extractProgress, this, &Downloader::onUpdateExtractProgress);
    connect(&m_extractor, &Extractor::extractFinished, this, &Downloader::onExtractFinished);
}

bool Downloader::get(const QString &targetFolder, const QString &version, const QStringList &components) {
    if(targetFolder.isEmpty() || version.isEmpty() || components.isEmpty()) {
        return false;
    }

    m_version = version;
    m_downloadComponents = components;
    m_targetFolder = targetFolder;

    m_componentCount = m_downloadComponents.count();

    onDownloadFinished();

    return true;
}

QString Downloader::path() const {
    return m_path;
}

QString Downloader::version() const {
    return m_version;
}

void Downloader::onReadyRead() {
    if(m_file) {
        m_file->write(m_currentReply->readAll());
    }
}

void Downloader::cancelDownload() {
    if(m_currentReply) {
        m_currentReply->abort();
    }
}

void Downloader::onReply(QNetworkReply *reply) {
    if(reply->error() == QNetworkReply::NoError) {
        m_file->flush();
        m_file->close();
    } else {
        m_file->remove();
    }

    delete m_file;
    m_file = nullptr;
    reply->deleteLater();
}

void Downloader::onUpdateDownloadProgress(int64_t bytesReceived, int64_t bytesTotal) {
    double value = (double)bytesReceived / (double)bytesTotal;
    value += m_componentCount - (m_downloadComponents.count() + 1);
    value /= (double)m_componentCount;

    emit updateDownloadProgress(value);
}

void Downloader::onUpdateExtractProgress(int64_t filesExtracted, int64_t filesTotal) {
    double value = (double)filesExtracted / (double)filesTotal;
    value += m_componentCount - (m_extractComponents.count() + 1);
    value /= (double)m_componentCount;

    emit updateExtractProgress(value);
}

void Downloader::onDownloadFinished() {
    if(m_currentReply) {
        if(m_currentReply->error() != QNetworkReply::NoError) {
            qDebug() << m_currentReply->errorString();
            return;
        }
    }

    if(!m_downloadComponents.isEmpty()) {
        QUrl url(m_downloadComponents.takeFirst());

        m_path = m_targetFolder + QDir::separator() + url.fileName();
        m_extractComponents << m_path;

        m_file = new QFile(m_path);
        if(!m_file->open(QIODevice::WriteOnly)) {
            delete m_file;
            m_file = nullptr;
            return;
        }

        QNetworkRequest request(url);
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
        m_currentReply = m_manager.get(request);

        connect(m_currentReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
        connect(m_currentReply, &QNetworkReply::downloadProgress, this, &Downloader::onUpdateDownloadProgress);
        connect(m_currentReply, &QNetworkReply::finished, this, &Downloader::onDownloadFinished);
    } else { // Start extraction procedure
        onExtractFinished();
    }
}

void Downloader::onExtractFinished() {
    if(!m_extractComponents.isEmpty()) {
        m_extractor.extract(m_extractComponents.takeFirst(), "C:/Projects/");
    } else {
        emit jobFinished();
    }
}
