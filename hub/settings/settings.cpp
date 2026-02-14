#include "settings.h"

#include <QSettings>
#include <QStandardPaths>

namespace {
    const char *gSdkDir("sdkDir");
    const char *gTempDir("tempDir");
    const char *gProjectsDir("projectsDir");
}

Settings::Settings() {
    QSettings settings(COMPANY_NAME, EDITOR_NAME);

    QString defaultSdkPath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    defaultSdkPath.append("/");
    defaultSdkPath.append(COMPANY_NAME);
    m_sdkDir = settings.value(gSdkDir, defaultSdkPath).toString();

    QString defaultTempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    defaultTempPath.append("/");
    defaultTempPath.append(COMPANY_NAME);
    m_tempDir = settings.value(gTempDir, defaultTempPath).toString();

    QString defaultProjectsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    defaultSdkPath.append("/");
    defaultSdkPath.append(COMPANY_NAME);
    m_projectsDir = settings.value(gProjectsDir, defaultProjectsPath).toString();
}

Settings *Settings::instance() {
    static Settings instance;

    return &instance;
}

QString Settings::sdkDir() const {
    return m_sdkDir;
}

void Settings::setSdkDir(const QString &dir) {
    if(m_sdkDir != dir) {
        m_sdkDir = dir;
        commitChanges();
        emit sdkDirChanged();
    }
}

QString Settings::projectsDir() const {
    return m_projectsDir;
}

void Settings::setProjectsDir(const QString &dir) {
    if(m_projectsDir != dir) {
        m_projectsDir = dir;
        commitChanges();
        emit projectsDirChanged();
    }
}

QString Settings::tempDir() const {
    return m_tempDir;
}

void Settings::setTempDir(const QString &dir) {
    if(m_tempDir != dir) {
        m_tempDir = dir;
        commitChanges();
        emit tempDirChanged();
    }
}

void Settings::commitChanges() {
    QSettings settings(COMPANY_NAME, EDITOR_NAME);

    settings.setValue(gSdkDir, m_sdkDir);
    settings.setValue(gTempDir, m_tempDir);
    settings.setValue(gProjectsDir, m_projectsDir);
}
