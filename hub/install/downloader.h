#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "extractor.h"

class QFile;

class Downloader : public QObject {
    Q_OBJECT

public:
    explicit Downloader(QObject *parent = nullptr);
    bool get(const QString &targetFolder, const QString &version, const QStringList &components);

    QString path() const;
    QString version() const;

public slots:
    void cancelDownload();

signals:
    void updateDownloadProgress(float percent);
    void updateExtractProgress(float percent);

    void jobFinished();

private slots:
    void onReadyRead();
    void onReply(QNetworkReply *reply);
    void onUpdateDownloadProgress(int64_t bytesReceived, int64_t bytesTotal);
    void onUpdateExtractProgress(int64_t filesExtracted, int64_t filesTotal);

    void onDownloadFinished();
    void onExtractFinished();

private:
    QNetworkReply *m_currentReply;
    QFile *m_file;
    QNetworkAccessManager m_manager;
    QString m_path;
    QString m_version;
    QString m_targetFolder;
    QStringList m_downloadComponents;
    QStringList m_extractComponents;

    Extractor m_extractor;

    int m_componentCount;
};

#endif // DOWNLOADER_H
