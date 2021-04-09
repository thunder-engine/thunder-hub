#include "projectmodel.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QProcess>
#include <QFileDialog>

#include "../install/installmodel.h"

const QString gProjects("launcher.projects");
const QString gProjectExt(".forge");

const QString gDefaultPath("");

ProjectModel::ProjectModel(InstallModel &install) :
        QAbstractListModel(),
        m_InstallModel(install) {

    QSettings settings(COMPANY_NAME, EDITOR_NAME);
    QVariant value  = settings.value(gProjects);
    if(value.isValid()) {
        m_List = value.toStringList();
        foreach(const QString it, m_List) {
            if(!QFileInfo(it).exists()) {
                m_List.removeAll(it);
            }
        }
    }
}

ProjectModel::~ProjectModel() {
    QSettings settings(COMPANY_NAME, EDITOR_NAME);
    settings.setValue(gProjects, m_List);
}

int ProjectModel::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }
    return m_List.size();
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    QFileInfo info( m_List.at(index.row()) );
    switch(role) {
        case NameRole: { return info.baseName(); }
        case PathRole: { return info.absoluteFilePath(); }
        case DirRole:  { return info.absolutePath(); }
        case IconRole: {
            QFileInfo icon(info.absolutePath() + "/cache/thumbnails/auto.png");
            if(icon.isReadable()) {
                return "file:///" + icon.absoluteFilePath();
            }
            return "qrc:/icons/thunderlight.svg";
        }
        default: break;
    }
    return QVariant();
}

QHash<int, QByteArray> ProjectModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    roles[DirRole]  = "dir";
    roles[IconRole] = "icon";

    return roles;
}

void ProjectModel::newProject() {
    QString path = QFileDialog::getSaveFileName(nullptr, tr("Create New Project"), gDefaultPath, "*" + gProjectExt);
    if(!path.isEmpty()) {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly)) {
            file.close();
            m_List.push_front(QDir::fromNativeSeparators(path));
            commitProjectRecord();
        }
    }
}

void ProjectModel::importProject() {
    QString path = QFileDialog::getOpenFileName(nullptr, tr("Import Existing Project"), gDefaultPath, "*" + gProjectExt);
    if(!path.isEmpty()) {
        m_List.push_front(QDir::fromNativeSeparators(path));
        commitProjectRecord();
    }
}

void ProjectModel::commitProjectRecord() {
    m_List.removeDuplicates();
    QSettings settings(COMPANY_NAME, EDITOR_NAME);
    settings.setValue(gProjects, m_List);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void ProjectModel::openProject(const QString &path) {
    QProcess::startDetached(m_InstallModel.defaultSdk(), QStringList() << path);
}

void ProjectModel::removeProject(const QString &path) {
    m_List.removeAll(path);
    commitProjectRecord();
}
