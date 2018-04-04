#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView *view = new QQuickView;
    view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view->show();
    view->setMinimumHeight(450);
    view->setMinimumWidth(800);
    view->setMaximumHeight(450);
    view->setMaximumWidth(800);

    return app.exec();
}
