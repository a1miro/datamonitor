#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>

#include "datasource.h"
#include "sinewave.h"
#include "squarewave.h"
#include "trianglewave.h"
#include "plotrenderer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Register QML types
    qmlRegisterUncreatableType<DataSource>("DataMonitor", 1, 0, "DataSource", "DataSource is abstract");
    qmlRegisterType<SineWave>("DataMonitor", 1, 0, "SineWave");
    qmlRegisterType<SquareWave>("DataMonitor", 1, 0, "SquareWave");
    qmlRegisterType<TriangleWave>("DataMonitor", 1, 0, "TriangleWave");
    qmlRegisterType<PlotRenderer>("DataMonitor", 1, 0, "PlotRenderer");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qt/qml/DataMonitor/qml/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
                     {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
