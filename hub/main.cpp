#include <QApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "project/projectmodel.h"
#include "feed/feedmanager.h"
#include "install/installmodel.h"
#include "install/sdkmodel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setApplicationName(PRODUCT_NAME);
    QCoreApplication::setApplicationVersion(SDK_VERSION);

    InstallModel installModel;
    SdkModel sdkModel;
    ProjectModel projectModel(installModel);
    FeedManager feedManager;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("installModel", &installModel);
    engine.rootContext()->setContextProperty("sdkModel", &sdkModel);
    engine.rootContext()->setContextProperty("projectsModel", &projectModel);
    engine.rootContext()->setContextProperty("feedManager", &feedManager);

    engine.load(QUrl("qrc:/qml/Startup.qml"));

    return a.exec();
}
