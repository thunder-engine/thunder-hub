#include "feedmanager.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>

FeedManager::FeedManager(QObject *parent) :
        QObject(parent) {

    QNetworkRequest req(QUrl("http://thunderengine.org/feed.xml"));
    req.setRawHeader("Accept", "application/xml,*/*");

    m_pManager = new QNetworkAccessManager();
    connect(m_pManager, &QNetworkAccessManager::finished,
            this, &FeedManager::replyFinished, Qt::QueuedConnection);

    m_pThread = new QThread();

    m_pReply = m_pManager->get(req);
    m_pManager->moveToThread(m_pThread);
    m_pThread->start();
}

FeedManager::~FeedManager() {
    m_pReply->abort();
    m_pThread->terminate();
    delete m_pThread;
    delete m_pManager;
}

QString FeedManager::blogFeed() const {
    return m_BlogData;
}

void FeedManager::replyFinished(QNetworkReply *reply) {
    m_BlogData = reply->readAll();
    emit blogFeedChanged();
}
