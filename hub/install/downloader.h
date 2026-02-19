#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>

#include "extractor.h"

struct Sdk;

class QFile;
class QNetworkAccessManager;

class Downloader : public QObject {
    Q_OBJECT

public:
    explicit Downloader(QNetworkAccessManager *manager, QObject *parent = nullptr);
    bool get(Sdk *sdk, const QStringList &components);

signals:
    void updateDownloadProgress(float percent);
    void updateExtractProgress(float percent);

    void jobFinished();
    void jobUpdated();

private slots:
    void onReadyRead();
    void onUpdateDownloadProgress(int64_t bytesReceived, int64_t bytesTotal);
    void onUpdateExtractProgress(int64_t filesExtracted, int64_t filesTotal);

    void onDownloadFinished();
    void onExtractFinished();

private:
    QStringList m_downloadComponents;
    QStringList m_extractComponents;

    Extractor m_extractor;

    QNetworkAccessManager *m_manager;

    Sdk *m_sdk;
    QFile *m_file;

    int m_componentCount;
};

#endif // DOWNLOADER_H
