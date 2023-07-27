#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "poincareview.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool openFile(const QString &fileName);

private:
    bool loadFile(const QString &fileName);

private slots:
    void on_action_file_open_triggered();
    void on_action_file_save_as_triggered();

private:
    Ui::MainWindow *ui;
    Diagram *diagram = nullptr;
    PoincareView *view = nullptr;
};
#endif // MAINWINDOW_H
