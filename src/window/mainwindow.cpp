#include "include/window/mainwindow.h"
#include "ui_mainwindow.h"

#include "include/window/networkcreationdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    QString sPath = "../";
    _dirModel = new QFileSystemModel(this);
    _dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    _fileModel = new QFileSystemModel(this);
    _fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    _ui->dirtreeview->setModel(_dirModel);
    _ui->filelistview->setModel(_fileModel);
    _ui->dirtreeview->setRootIndex(_dirModel->setRootPath(sPath));
    _ui->filelistview->setRootIndex(_fileModel->setRootPath(sPath));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::onButtonPress()
{
    //TODO: Do stuffs
}

void MainWindow::on_dirtreeview_clicked(const QModelIndex &index)
{
    QString sPath = _dirModel->fileInfo(index).absoluteFilePath();
    _ui->filelistview->setRootIndex(_fileModel->setRootPath(sPath));
}

void MainWindow::on_filelistview_clicked(const QModelIndex &index)
{
    _selectedFile = _fileModel->fileInfo(index).absoluteFilePath();
}

void MainWindow::on_button_addFile_clicked()
{
    bool checker = false;
    for(int i = 0; i < _fileNames.size(); i++)
    {
        if(_selectedFile == _fileNames[i])
            checker = true;
    }
    if(!checker)
    {
        _fileNames.push_back(_selectedFile);
    }
}

void MainWindow::on_pushButton_clicked()
{
    NetworkCreationDialog* dialog = new NetworkCreationDialog(this);
    dialog->show();
}
