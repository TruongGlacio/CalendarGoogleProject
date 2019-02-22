#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include<calendar.h>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("O2");
    QCoreApplication::setApplicationName("Calendar Test");
    QGuiApplication app(argc, argv);
    qmlRegisterType<Calendar>("Calendar", 1, 0, "Calendar");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
