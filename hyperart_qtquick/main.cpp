#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "matrix.h" // Ensures that `hyperart_core` is correctly linked and found.

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Instantiate a core object to verify linkage
    Matrix m;

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
        
    engine.loadFromModule("hyperart.main", "Main");

    return app.exec();
}
