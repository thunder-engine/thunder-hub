#ifndef INSTALLMODEL_H
#define INSTALLMODEL_H

#include <QAbstractListModel>

class Downloader;

class InstallModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        VersionRole = Qt::UserRole + 1,
        PathRole,
        ProgressRole
    };

    InstallModel();
    ~InstallModel() override;

    QString defaultSdk() const;

    Q_INVOKABLE void locateSdk();
    Q_INVOKABLE void installSdk(const QString &version);
    Q_INVOKABLE void uninstallSdk(const QString &path);

    Q_INVOKABLE QStringList sdkVersions();

private slots:
    void onDownloadFinished();
    void onDownloadUpdated(int64_t bytesReceived, int64_t bytesTotal);

private:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void commitInstallRecord();

protected:
    QStringList m_List;

    QList<Downloader *> m_Jobs;

    QString m_TargetDir;

    QStringList m_Versions;
};

#endif // INSTALLMODEL_H
