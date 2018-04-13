#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QTime>

#include "include/controller/updatercontroller.h"

QObject *updaterController_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    static UpdaterController *controller = new UpdaterController();
    return controller;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));

    QQuickView *view = new QQuickView;

    if (argc >= 5) {
        UpdaterController::ApplicationPath = QString(argv[3]);
        UpdaterController::ApplicationName = QString(argv[4]);
        static_cast<UpdaterController *>(updaterController_singletontype_provider(NULL, NULL))->setVersion(argv[2]);
        static_cast<UpdaterController *>(updaterController_singletontype_provider(NULL, NULL))->setPrevVersion(argv[1]);
        qDebug() << UpdaterController::ApplicationPath;
        qDebug() << UpdaterController::ApplicationName;
        qDebug() << argv[2];
        qDebug() << argv[1];

    }
    qmlRegisterSingletonType<UpdaterController>("UpdaterController", 1, 0, "Controller", updaterController_singletontype_provider);

    view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view->show();
    view->setMinimumHeight(450);
    view->setMinimumWidth(800);
    view->setMaximumHeight(450);
    view->setMaximumWidth(800);


    return app.exec();
}
