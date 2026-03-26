#include <QApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "project/projectmanager.h"
#include "feed/feedmanager.h"
#include "install/installmodel.h"
#include "install/sdkmodel.h"
#include "settings/settings.h"
#include "update/update.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setApplicationName(PRODUCT_NAME);
    QCoreApplication::setApplicationVersion(PRODUCT_VERSION);

    FeedManager feedManager;
    Update updateManager;

    QQmlApplicationEngine engine;

    engine.addImportPath(":/qml");
    engine.addImportPath(":/qml/components");

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("installModel", InstallModel::instance());
    context->setContextProperty("sdkModel", SdkModel::instance());
    context->setContextProperty("projectsManager", ProjectManager::instance());
    context->setContextProperty("feedManager", &feedManager);
    context->setContextProperty("updateManager", &updateManager);
    context->setContextProperty("settingsManager", Settings::instance());

    engine.load(QUrl("qrc:/qml/Startup.qml"));

    if(engine.rootObjects().isEmpty()) {
        return -1;
    }

    updateManager.checkUpdate();

    int result = a.exec();

    InstallModel::instance()->commitInstallRecord();
    ProjectManager::instance()->commitProjectRecord();

    return result;
}
