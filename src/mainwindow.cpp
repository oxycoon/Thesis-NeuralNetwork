#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    findAllFiles();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::findAllFiles()
{
    //TODO: get all file strings in res/docs
    //      and add to listWidget.
}

void MainWindow::onButtonPress()
{
    //TODO: Do stuffs
}
