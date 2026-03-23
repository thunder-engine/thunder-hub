#include "feedmanager.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QtXml/QDomDocument>

FeedManager::FeedManager(QObject *parent) :
        QAbstractListModel(parent),
        m_manager(new QNetworkAccessManager()) {

    QNetworkRequest req(QUrl("http://thunderengine.org/feed.xml"));
    req.setRawHeader("Accept", "application/xml,*/*");

    QNetworkReply *reply = m_manager->get(req);
    connect(reply, &QNetworkReply::finished, this, &FeedManager::onUpdateCheckFinished);
}

FeedManager::~FeedManager() {
    delete m_manager;
}

void FeedManager::onUpdateCheckFinished() {
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if(reply) {
        m_items.clear();

        QDomDocument doc;
        doc.setContent(reply->readAll());
        QDomElement root = doc.documentElement();

        QDomNodeList entries = root.elementsByTagName("entry");
        for(int i = 0; i < entries.count(); ++i) {
            QDomNode entryNode = entries.at(i);

            RssItem item;

            QDomNodeList titleNodes = entryNode.toElement().elementsByTagName("title");
            if(!titleNodes.isEmpty()) {
                item.title = extractTextFromNode(titleNodes.at(0));
            }

            QDomNodeList linkNodes = entryNode.toElement().elementsByTagName("link");
            if(!linkNodes.isEmpty()) {
                QDomElement linkElem = linkNodes.at(0).toElement();
                if (linkElem.hasAttribute("href")) {
                    item.link = linkElem.attribute("href");
                }
            }

            QDomNodeList summaryNodes = entryNode.toElement().elementsByTagName("summary");
            if(!summaryNodes.isEmpty()) {
                item.description = extractTextFromNode(summaryNodes.at(0));
            }

            QDomNodeList categoryNodes = entryNode.toElement().elementsByTagName("category");
            if(!categoryNodes.isEmpty()) {
                QDomElement categoryElem = categoryNodes.at(0).toElement();
                if (categoryElem.hasAttribute("term")) {
                    item.category = categoryElem.attribute("term");
                }
            }

            QDomNodeList thumbnailNodes = entryNode.toElement().elementsByTagName("media:thumbnail");
            if(!thumbnailNodes.isEmpty()) {
                QDomElement thumbnailElem = thumbnailNodes.at(0).toElement();
                if (thumbnailElem.hasAttribute("url")) {
                    item.media = thumbnailElem.attribute("url");
                }
            }

            if(!item.title.isEmpty()) {
                m_items.push_back(item);
            }
        }

        emit layoutAboutToBeChanged();
        emit layoutChanged();
    }
}

QString FeedManager::extractTextFromNode(const QDomNode &node) {
    QString result;
    QDomNode child = node.firstChild();

    while (!child.isNull()) {
        if (child.isText()) {
            result += child.toText().data();
        } else if (child.isCDATASection()) {
            result += child.toCDATASection().data();
        }
        child = child.nextSibling();
    }

    return result.trimmed();
}

int FeedManager::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }
    return m_items.size();
}

QVariant FeedManager::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    RssItem item( m_items.at(index.row()) );
    switch(role) {
        case Title: { return item.title; }
        case Brief: { return item.description; }
        case Link:  { return item.link; }
        case Media: { return item.media; };
        default: break;
    }
    return QVariant();
}

QHash<int, QByteArray> FeedManager::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Title] = "title";
    roles[Brief] = "brief";
    roles[Link]  = "link";
    roles[Media] = "icon";

    return roles;
}
