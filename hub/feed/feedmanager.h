#ifndef FEEDMANAGER_H
#define FEEDMANAGER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QThread;

class FeedManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString blogFeed READ blogFeed NOTIFY blogFeedChanged)

public:
    explicit FeedManager(QObject *parent = nullptr);
    ~FeedManager();

    QString blogFeed () const;

signals:
    void blogFeedChanged();

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_pManager;
    QNetworkReply *m_pReply;
    QThread *m_pThread;

    QString m_BlogData;
};

#endif // FEEDMANAGER_H
