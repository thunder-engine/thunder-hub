#include "sdkmodel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QFile>

namespace {
    const QString gCommon("Common");
}

SdkModel::SdkModel() {
    QFile file(":/manifest.json");
    if(file.open(QIODevice::ReadOnly)) {
        QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        file.close();
        QJsonObject root = document.object();
        for(auto it : root.value("SDK").toArray()) {
            QJsonObject sdk = it.toObject();
            QString version = sdk.value("Version").toString();

            QList<Module> modules;
            for(auto it : sdk.value("Modules").toArray()) {
                Module m;

                QJsonObject module = it.toObject();
                QJsonObject platform = module.value("Links").toObject();
                m.url = platform.value(gCommon).toString();
                if(platform.contains("Windows")) {
                    m.url = platform.value("Windows").toString();
                }
                m.name = module.value("Name").toString();

                modules.push_back(m);
            }

            m_Modules[version] = modules;
        }
    }
}

QStringList SdkModel::sdkVersions() {
    return m_Modules.keys();
}

void SdkModel::setVersion(const QString &version) {
    m_Version = version;

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

int SdkModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }
    return m_Modules[m_Version].size();
}

QVariant SdkModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    Module module = m_Modules[m_Version].at(index.row());
    switch(role) {
        case NameRole:        { return module.name; }
        case UrlRole:         { return module.url; }
        case OptionalityRole: { return module.optional; }
        case SizeRole:        { return module.size; }
        default: break;
    }
    return QVariant();
}

QHash<int, QByteArray> SdkModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[NameRole]        = "name";
    roles[UrlRole]         = "url";
    roles[OptionalityRole] = "optionality";
    roles[SizeRole]        = "size";

    return roles;
}
