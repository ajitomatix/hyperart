#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets>
#include "datareader.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new PoincareView(this);
    centralWidget()->layout()->addWidget(view);
}

MainWindow::~MainWindow()
{
    delete diagram;
    delete view;
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    const QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open a HyperArt Design"), "../../../../../hyperart_qtwidgets/designs/", tr("HyperArt Designs (*.had)")
        );

    qDebug() << "Filename: " << fileName;

    if (!fileName.isEmpty())
        openFile(fileName);

//    openFile("/Users/ajit/Development/qt/hyperart/hyperart_qtwidgets/designs/irp/tri.had");
//    openFile("/Users/ajit/Development/qt/hyperart/hyperart_qtwidgets/designs/cl2.had");

}


bool MainWindow::openFile(const QString &fileName)
{
    const bool succeeded = loadFile(fileName);
    if (succeeded)
        statusBar()->showMessage(tr("File loaded"), 2000);
    else
        statusBar()->showMessage(tr("Didn't load a design"), 2000);
    return succeeded;
}

bool MainWindow::loadFile(const QString &fileName)
{
    diagram = DataReader::createDiagram(fileName);
    bool succeeded = false;
    if(diagram != nullptr) {
        succeeded = true;
        view->documentChanged(diagram);
    }
    return succeeded;
}

