#include "sdkmodel.h"

#include "installmodel.h"

namespace {
    const char *gName("name");
    const char *gSize("size");
}

SdkModel *SdkModel::instance() {
    static SdkModel instance;

    return &instance;
}

void SdkModel::setVersion(const QString &version) {
    m_version = version;

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

int SdkModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }
    return InstallModel::instance()->getModules(m_version).size();
}

QVariant SdkModel::data(const QModelIndex &index, int role) const {
    if(index.isValid()) {

        const Module module = InstallModel::instance()->getModules(m_version).at(index.row());
        switch(role) {
            case NameRole: return module.name;
            case OptionalityRole: return module.optional;
            case InstalledRole: return module.optional ? module.installed : true;
            case SizeRole: return module.size;
            case PlatformRole: return module.platform;
        default: break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> SdkModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[NameRole] = gName;
    roles[OptionalityRole] = "optional";
    roles[InstalledRole] = "installed";
    roles[SizeRole] = gSize;
    roles[PlatformRole] = "platform";

    return roles;
}
