#include <QApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "project/projectmodel.h"
#include "feed/feedmanager.h"
#include "install/installmodel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setApplicationName(PRODUCT_NAME);
    QCoreApplication::setApplicationVersion(SDK_VERSION);

    InstallModel installModel;
    ProjectModel projectModel(installModel);
    FeedManager feedManager;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("installModel", &installModel);
    engine.rootContext()->setContextProperty("projectsModel", &projectModel);
    engine.rootContext()->setContextProperty("feedManager", &feedManager);

    engine.load(QUrl("qrc:/qml/Startup.qml"));

    return a.exec();
}
