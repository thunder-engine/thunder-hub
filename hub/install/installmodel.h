#ifndef INSTALLMODEL_H
#define INSTALLMODEL_H

#include <QAbstractListModel>

class Downloader;

struct Sdk {
    bool operator== (Sdk right) {
        return (path == right.path) && (version == right.version);
    }

    QString path;
    QString version;
    QString status;
    int progress;
};

class InstallModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        VersionRole = Qt::UserRole + 1,
        PathRole,
        ProgressRole,
        StatusRole
    };

    InstallModel();
    ~InstallModel() override;

    QString defaultSdk() const;

    Q_INVOKABLE void locateSdk();
    Q_INVOKABLE void installSdk(const QString &version, const QStringList &components);
    Q_INVOKABLE void uninstallSdk(const QString &path);

private slots:
    void onJobFinished();
    void onDownloadUpdated(float value);
    void onExtractingUpdated(float value);

private:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void commitInstallRecord();

protected:
    QList<Sdk> m_List;

    QList<Downloader *> m_Jobs;

    QString m_TargetDir;

    QStringList m_Versions;

    QMap<QString, QMap<QString, QString>> m_Modules;
};

#endif // INSTALLMODEL_H
