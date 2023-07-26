#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QString appName = "HyperArt";
    QApplication a(argc, argv);
    a.setApplicationName(appName);
    a.setApplicationDisplayName(appName);
    MainWindow w;
    w.show();
    return a.exec();
}
