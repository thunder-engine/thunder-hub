#include "update.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace {
    const char *gTagName("tag_name");
}

Update::Update() :
        m_manager(new QNetworkAccessManager) {

}

void Update::checkUpdate() {
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl("https://api.github.com/repos/thunder-engine/thunder-hub/releases")));
    connect(reply, &QNetworkReply::finished, this, &Update::onUpdateCheckFinished);
}

void Update::onUpdateCheckFinished() {
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if(reply) {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray array = document.array();
        if(!array.isEmpty()) {
            auto it = document.array().first();
            QString version(it.toObject().value(gTagName).toString());
            if(version != PRODUCT_VERSION) {
                emit updateAvailable(version);
            }
        }
    }
}
