#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractListModel>

class InstallModel;

class ProjectModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        PathRole,
        DirRole,
        IconRole
    };

    explicit ProjectModel(InstallModel &install);
    ~ProjectModel() override;

    Q_INVOKABLE void newProject();
    Q_INVOKABLE void importProject();
    Q_INVOKABLE void openProject(const QString &path);
    Q_INVOKABLE void removeProject(const QString &path);

private:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void commitProjectRecord();

protected:
    QStringList m_List;

    InstallModel &m_InstallModel;
};

#endif // PROJECTMODEL_H
