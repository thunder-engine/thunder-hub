#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QAbstractListModel>

class ProjectManager : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        PathRole,
        DirRole,
        IconRole
    };

    ProjectManager();

    static ProjectManager *instance();
    void commitProjectRecord();

    Q_INVOKABLE void newProject();
    Q_INVOKABLE void importProject();
    Q_INVOKABLE void openProject(const QString &path);
    Q_INVOKABLE void removeProject(const QString &path);

private:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

protected:
    QStringList m_projects;

};

#endif // PROJECTMANAGER_H
