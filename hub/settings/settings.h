#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString sdkDir READ sdkDir WRITE setSdkDir NOTIFY sdkDirChanged FINAL)
    Q_PROPERTY(QString tempDir READ tempDir WRITE setTempDir NOTIFY tempDirChanged FINAL)
    Q_PROPERTY(QString projectsDir READ projectsDir WRITE setProjectsDir NOTIFY tempDirChanged FINAL)

public:
    Settings();

    static Settings *instance();

    QString sdkDir() const;
    void setSdkDir(const QString &dir);

    QString projectsDir() const;
    void setProjectsDir(const QString &dir);

    QString tempDir() const;
    void setTempDir(const QString &dir);

signals:
    void sdkDirChanged();
    void tempDirChanged();
    void projectsDirChanged();

private:
    void commitChanges();

private:
    QString m_sdkDir;

    QString m_tempDir;

    QString m_projectsDir;

};

#endif // SETTINGS_H
