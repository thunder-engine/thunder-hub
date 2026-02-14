#ifndef FEEDMANAGER_H
#define FEEDMANAGER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class FeedManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString blogFeed READ blogFeed NOTIFY blogFeedChanged)

public:
    explicit FeedManager(QObject *parent = nullptr);
    ~FeedManager();

    QString blogFeed() const;

signals:
    void blogFeedChanged();

private slots:
    void onUpdateCheckFinished();

private:
    QNetworkAccessManager *m_manager;

    QString m_blogData;

};

#endif // FEEDMANAGER_H
