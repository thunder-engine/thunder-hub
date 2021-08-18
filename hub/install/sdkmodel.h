#ifndef SDKMODEL_H
#define SDKMODEL_H

#include <QAbstractListModel>

struct Module {
    bool operator== (Module right) {
        return (url == right.url);
    }

    QString name;
    QString url;
    QString size;
    bool optional;
};

class SdkModel : public QAbstractListModel {
    Q_OBJECT
public:
    SdkModel();

    enum Roles {
        NameRole = Qt::UserRole + 1,
        UrlRole,
        OptionalityRole,
        SizeRole
    };

    Q_INVOKABLE QStringList sdkVersions();
    Q_INVOKABLE void setVersion(const QString &version);

private:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

protected:
    QMap<QString, QList<Module>> m_Modules;

    QString m_Version;
};

#endif // SDKMODEL_H
