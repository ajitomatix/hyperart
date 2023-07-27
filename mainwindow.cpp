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

void MainWindow::on_action_file_open_triggered()
{
    auto filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open a HyperArt Design"),
        "../../../../../hyperart_qtwidgets/designs/",
        tr("HyperArt Designs (*.had)")
        );

    qDebug() << "File: " << filePath;

    if (!filePath.isEmpty())
        openFile(filePath);
}

void MainWindow::on_action_file_save_as_triggered()
{
    qDebug() << "File > Save as...";
    auto filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save this design as a PNG or JPEG image"),
        "",
        tr("Images (*.png *.jpg *.jpeg)")
        );

    if (!filePath.isEmpty())
        view->saveAs(filePath);
}

bool MainWindow::openFile(const QString &filePath)
{
    const bool succeeded = loadFile(filePath);
    if (succeeded) {
        statusBar()->showMessage(tr("File loaded"), 2000);
        QFileInfo fileinfo(filePath);
        this->setWindowTitle(fileinfo.baseName());
    }
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
