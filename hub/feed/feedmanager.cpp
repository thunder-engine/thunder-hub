#include "feedmanager.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

FeedManager::FeedManager(QObject *parent) :
        QObject(parent),
        m_manager(new QNetworkAccessManager()) {

    QNetworkRequest req(QUrl("http://thunderengine.org/feed.xml"));
    req.setRawHeader("Accept", "application/xml,*/*");

    QNetworkReply *reply = m_manager->get(req);
    connect(reply, &QNetworkReply::finished, this, &FeedManager::onUpdateCheckFinished);
}

FeedManager::~FeedManager() {
    delete m_manager;
}

QString FeedManager::blogFeed() const {
    return m_blogData;
}

void FeedManager::onUpdateCheckFinished() {
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if(reply) {
        m_blogData = reply->readAll();
        emit blogFeedChanged();
    }
}
