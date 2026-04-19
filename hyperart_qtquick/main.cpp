#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDirIterator>
#include "appcontroller.h"
#include "hyperartcanvas.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Register C++ integrations to QML
    qmlRegisterType<AppController>("HyperArt", 1, 0, "AppController");
    qmlRegisterType<HyperArtCanvas>("HyperArt", 1, 0, "HyperArtCanvas");

    QQmlApplicationEngine engine;
    
    QDirIterator it(":", QDirIterator::Subdirectories);
    qDebug() << "--- QRC CONTENTS ---";
    QString foundMainPath;
    while (it.hasNext()) {
        QString path = it.next();
        qDebug() << path;
        if (path.endsWith("Main.qml")) {
            foundMainPath = "qrc" + path; // e.g. ":/..." -> "qrc:/..."
        }
    }
    qDebug() << "--------------------";

    if (foundMainPath.isEmpty()) {
        qCritical() << "Could not find Main.qml in QRC!";
        return -1;
    }

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    
    qDebug() << "Loading QML from:" << foundMainPath;
    engine.load(QUrl(foundMainPath));

    return app.exec();
}
