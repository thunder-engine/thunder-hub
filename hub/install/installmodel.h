#ifndef INSTALLMODEL_H
#define INSTALLMODEL_H

#include <QAbstractListModel>

class Downloader;
class QNetworkAccessManager;

struct Module {
    bool operator== (Module right) {
        return (url == right.url);
    }

    QString platform;

    QString name;

    QString url;

    QString size;

    bool optional = true;

    bool installed = false;

};

struct Sdk {
    bool operator== (const Sdk &right) const {
        return (path == right.path) && (version == right.version);
    }

    void checkInstalled();

    QString path;

    QString root;

    QString version;

    QString status;

    QList<Module> modules;

    int progress = -1;

    bool installed = false;

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

    static InstallModel *instance();

    QString defaultSdk() const;
    void commitInstallRecord();

    Sdk *getSdk(const QString &version);
    const QList<Module> getModules(const QString &version);

    Q_INVOKABLE QStringList sdkVersions();
    Q_INVOKABLE QStringList installedModules(const QString &version);
    Q_INVOKABLE void locateSdk();
    Q_INVOKABLE void updateSdk(const QString &version, const QStringList &modules);
    Q_INVOKABLE void uninstallSdk(const QString &version);
    Q_INVOKABLE void explorer(const QString &version);

signals:
    void sdkListChanged();

private slots:
    void onUpdateCheckFinished();
    void onJobFinished();
    void onJobUpdated();

private:
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

protected:
    QList<Sdk> m_sdk;

    QList<Downloader *> m_jobs;

    QNetworkAccessManager *m_manager;

};

#endif // INSTALLMODEL_H
