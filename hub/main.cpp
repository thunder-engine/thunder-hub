#include <QApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "project/projectmanager.h"
#include "feed/feedmanager.h"
#include "install/installmodel.h"
#include "install/sdkmodel.h"
#include "settings/settings.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setApplicationName(PRODUCT_NAME);
    QCoreApplication::setApplicationVersion(SDK_VERSION);

    FeedManager feedManager;

    QQmlApplicationEngine engine;

    engine.addImportPath(":/qml");
    engine.addImportPath(":/qml/components");

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("installModel", InstallModel::instance());
    context->setContextProperty("sdkModel", SdkModel::instance());
    context->setContextProperty("projectsManager", ProjectManager::instance());
    context->setContextProperty("feedManager", &feedManager);
    context->setContextProperty("settingsManager", Settings::instance());

    engine.load(QUrl("qrc:/qml/Startup.qml"));

    int result = a.exec();

    InstallModel::instance()->commitInstallRecord();
    ProjectManager::instance()->commitProjectRecord();

    return result;
}
