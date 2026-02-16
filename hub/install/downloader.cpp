#include "downloader.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

#include <QDebug>

#include "installmodel.h"
#include "settings.h"

Downloader::Downloader(QNetworkAccessManager *manager, QObject *parent) :
        QObject(parent),
        m_manager(manager),
        m_sdk(nullptr),
        m_file(nullptr),
        m_componentCount(0) {

    connect(&m_extractor, &Extractor::extractProgress, this, &Downloader::onUpdateExtractProgress);
    connect(&m_extractor, &Extractor::extractFinished, this, &Downloader::onExtractFinished);
}

bool Downloader::get(Sdk *sdk, const QStringList &components) {
    if(sdk == nullptr || components.isEmpty()) {
        return false;
    }

    m_sdk = sdk;
    m_downloadComponents = components;

    m_componentCount = m_downloadComponents.count();

    onDownloadFinished();

    return true;
}

void Downloader::onReadyRead() {
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if(m_file && reply) {
        m_file->write(reply->readAll());
    }
}

void Downloader::onUpdateDownloadProgress(int64_t bytesReceived, int64_t bytesTotal) {
    double value = (double)bytesReceived / (double)bytesTotal;
    value += m_componentCount - (m_downloadComponents.count() + 1);
    value /= (double)m_componentCount;

    int progress = value * 50;
    if(progress != m_sdk->progress) {
        m_sdk->progress = progress;
        m_sdk->status = tr("Downloading...");
    }

    emit jobUpdated();
}

void Downloader::onUpdateExtractProgress(int64_t filesExtracted, int64_t filesTotal) {
    double value = (double)filesExtracted / (double)filesTotal;
    value += m_componentCount - (m_extractComponents.count() + 1);
    value /= (double)m_componentCount;

    int progress = 50 + value * 50;
    if(progress != m_sdk->progress) {
        m_sdk->progress = progress;
        m_sdk->status = tr("Extracting...");
    }

    emit jobUpdated();
}

void Downloader::onDownloadFinished() {
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if(reply) {
        if(reply->error() == QNetworkReply::NoError) {
            m_file->flush();
            m_file->close();
        } else {
            qDebug() << reply->errorString();
            m_file->remove();
        }

        delete m_file;
        m_file = nullptr;
        reply->deleteLater();
    }

    if(!m_downloadComponents.isEmpty()) {
        QUrl url(m_downloadComponents.takeFirst());

        QDir dir;
        dir.mkpath(Settings::instance()->tempDir());
        QString path(Settings::instance()->tempDir() + QDir::separator() + url.fileName());
        m_extractComponents << path;

        m_file = new QFile(path);
        if(!m_file->open(QIODevice::WriteOnly)) {
            delete m_file;
            m_file = nullptr;
            return;
        }

        reply = m_manager->get(QNetworkRequest(url));

        connect(reply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
        connect(reply, &QNetworkReply::downloadProgress, this, &Downloader::onUpdateDownloadProgress);
        connect(reply, &QNetworkReply::finished, this, &Downloader::onDownloadFinished);
    } else { // Start extraction procedure
        onExtractFinished();
    }
}

void Downloader::onExtractFinished() {
    if(!m_extractComponents.isEmpty()) {
        m_extractor.extract(m_extractComponents.takeFirst(), Settings::instance()->sdkDir());
    } else {
        m_sdk->progress = -1;
        m_sdk->status.clear();
        m_sdk->checkInstalled();

        emit jobFinished();
    }
}
