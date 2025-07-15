#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>

#include "datagenerator.h"
#include "plotrenderer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Register QML types
    qmlRegisterType<DataGenerator>("DataMonitor", 1, 0, "DataGenerator");
    qmlRegisterType<PlotRenderer>("DataMonitor", 1, 0, "PlotRenderer");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DataMonitor/qml/main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);

    return app.exec();
}
