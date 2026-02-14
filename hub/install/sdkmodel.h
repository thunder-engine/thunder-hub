#ifndef SDKMODEL_H
#define SDKMODEL_H

#include <QAbstractListModel>

class SdkModel : public QAbstractListModel {
    Q_OBJECT

    enum Roles {
        NameRole = Qt::UserRole + 1,
        OptionalityRole,
        InstalledRole,
        SizeRole,
        PlatformRole
    };
public:
    static SdkModel *instance();

    Q_INVOKABLE void setVersion(const QString &version);

private:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

protected:
    QString m_version;

};

#endif // SDKMODEL_H
