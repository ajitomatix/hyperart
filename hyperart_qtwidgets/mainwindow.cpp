/***************************************************************************
 *   HyperArt Copyright (C) 2005-2023 by Ajit Datar (ajitdatar@gmail.com)  *
 *                                                                         *
 *   Hyperbolic geometry drawing algorithms developed by                   *
 *   and credited to Dr. Douglas Dunham (ddunham@d.umn.edu)                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Please see LICENSE file for more details.                             *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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
