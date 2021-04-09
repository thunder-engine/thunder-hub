#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>

class QNetworkReply;
class QFile;

class Downloader : public QObject {
    Q_OBJECT

public:
    explicit Downloader(QObject *parent = nullptr);
    bool get(const QString &targetFolder, const QUrl &url);

public slots:
    void cancelDownload();

signals:
    void updateDownloadProgress(int64_t bytesReceived, int64_t bytesTotal);
    void finished();

private slots:
    void onReadyRead();
    void onReply(QNetworkReply *reply);

private:
    QNetworkReply *m_currentReply;
    QFile *m_file;
    QNetworkAccessManager m_manager;
};

#endif // DOWNLOADER_H
