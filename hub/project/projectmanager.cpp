#include "projectmanager.h"

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QProcess>
#include <QFileDialog>

#include "../install/installmodel.h"
#include "settings.h"

namespace {
    const char *gProjects("launcher.projects");
    const char *gProjectExt("*.forge");
}

ProjectManager::ProjectManager() {
    QSettings settings(COMPANY_NAME, EDITOR_NAME);
    QVariant value = settings.value(gProjects);
    if(value.isValid()) {
        m_projects = value.toStringList();
        foreach(const QString it, m_projects) {
            if(!QFileInfo::exists(it)) {
                m_projects.removeAll(it);
            }
        }
    }
}

ProjectManager *ProjectManager::instance() {
    static ProjectManager instance;

    return &instance;
}

void ProjectManager::newProject() {
    QString path = QFileDialog::getSaveFileName(nullptr, tr("Create New Project"), Settings::instance()->projectsDir(), gProjectExt);
    if(!path.isEmpty()) {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly)) {
            file.close();
            m_projects.push_front(QDir::fromNativeSeparators(path));
            commitProjectRecord();
        }
    }
}

void ProjectManager::importProject() {
    QString path = QFileDialog::getOpenFileName(nullptr, tr("Import Existing Project"), Settings::instance()->projectsDir(), gProjectExt);
    if(!path.isEmpty()) {
        m_projects.push_front(QDir::fromNativeSeparators(path));
        commitProjectRecord();
    }
}

void ProjectManager::commitProjectRecord() {
    m_projects.removeDuplicates();
    QSettings settings(COMPANY_NAME, EDITOR_NAME);
    settings.setValue(gProjects, m_projects);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

void ProjectManager::openProject(const QString &path) {
    QProcess::startDetached(InstallModel::instance()->defaultSdk(), { path });
}

void ProjectManager::removeProject(const QString &path) {
    m_projects.removeAll(path);
    commitProjectRecord();
}

int ProjectManager::rowCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return 0;
    }
    return m_projects.size();
}

QVariant ProjectManager::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    QFileInfo info( m_projects.at(index.row()) );
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

QHash<int, QByteArray> ProjectManager::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    roles[DirRole]  = "dir";
    roles[IconRole] = "icon";

    return roles;
}
