#include "include/window/mainwindow.h"
#include "ui_mainwindow.h"

#include "include/window/networkcreationdialog.h"
#include "include/cost/quadraticcost.h"
#include "include/cost/crossentropycost.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _selectedNetwork(-1)
{
    _ui->setupUi(this);

    _reader = new CSVReader();


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
    delete _reader;
}

std::vector<QString> MainWindow::getNetworkNames()
{
    std::vector<QString> result;

    for(int i = 0; i < _networkList.size(); i++)
    {
        result.push_back(QString::fromStdString(_networkList.at(i)->getNetworkName()));
    }
    return result;
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

void MainWindow::on_pushButton_network_create_clicked()
{
    std::vector<QString> names = getNetworkNames();

    NetworkCreationDialog* dialog = new NetworkCreationDialog(this, &names);
    //connect(dialog, SIGNAL(signNetworkCreation(int,std::vector<int>,int,QString)), this, signNetworkCreationRecieved);
    connect(dialog, &NetworkCreationDialog::signNetworkCreation, this, &MainWindow::signRecievedNetworkCreation);

    dialog->show();
}

void MainWindow::signRecievedNetworkCreation(const int in,
                const std::vector<int> hidden, const int out,
                const QString name, const DataType type, const CostCalc calc)
{
    Cost* costCalculator;
    if(calc == CostCalc::Quadratic) costCalculator = new QuadraticCost();
    else if(calc == CostCalc::CrossEntropy) costCalculator = new CrossEntropyCost();

    Network* net = new Network(in, hidden, out, costCalculator, type, name.toStdString() );
    connect(net, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net->initNetwork();

    _networkList.push_back(net);
    _ui->listWidget_networkList->addItem(name);

}

void MainWindow::signRecievedNetworkEdit(const int index, const int in, const std::vector<int> hidden, const int out, const QString name, const DataType type, const CostCalc calc)
{
    Cost* costCalculator;
    if(calc == CostCalc::Quadratic) costCalculator = new QuadraticCost();
    else if(calc == CostCalc::CrossEntropy) costCalculator = new CrossEntropyCost();

    delete _networkList[index];
    _networkList[index] = new Network(in, hidden, out, costCalculator, type, name.toStdString());

     _ui->listWidget_networkList->item(index)->setText(name);
}

void MainWindow::signRecievedFileReadComplete(const QString &message)
{
    _ui->text_parseConsole->appendPlainText(message);
    _ui->consoleOutput->appendPlainText(message);
}

void MainWindow::signRecievedConsoleOutput(const QString &message)
{
    _ui->consoleOutput->appendPlainText(message);
}

void MainWindow::signRecievedEpochComplete(const int epoch, const double trainingError, const double trainingAccuracy, const double testingError, const double testingAccuracy)
{
    //TODO: PLOT INTO GRAPH
}

void MainWindow::on_listWidget_networkList_itemClicked(QListWidgetItem *item)
{
    _selectedNetwork = _ui->listWidget_networkList->row(item);
}

void MainWindow::on_pushButton_network_edit_clicked()
{
    if(_selectedNetwork != -1)
    {
        int input, output;
        std::vector<int> hidden;
        QString name;
        DataType type;
        CostCalc calc;

        input = _networkList[_selectedNetwork]->getInputCount();
        hidden = _networkList[_selectedNetwork]->getHiddenCount();
        output = _networkList[_selectedNetwork]->getOutputCount();
        name = QString::fromStdString(_networkList[_selectedNetwork]->getNetworkName());
        type = _networkList[_selectedNetwork]->getNetworkType();
        calc = _networkList[_selectedNetwork]->getNetworkCostCalc();

        NetworkCreationDialog* dialog = new NetworkCreationDialog(this);
        //connect(dialog, SIGNAL(signNetworkCreation(int,std::vector<int>,int,QString)), this, signNetworkCreationRecieved);
        connect(dialog, &NetworkCreationDialog::signNetworkEdit, this, &MainWindow::signRecievedNetworkEdit);
        dialog->editNetwork(_selectedNetwork, input, hidden, output, name, type, calc);
        dialog->show();
    }
}

void MainWindow::on_pushButton_network_delete_clicked()
{
    if(_selectedNetwork != -1)
    {
        std::cout << "Elements in list: " << _networkList.size() << std::endl;
        _ui->listWidget_networkList->model()->removeRow(_selectedNetwork);
        delete _networkList[_selectedNetwork];
        _networkList.erase(_networkList.begin() + _selectedNetwork);
        std::cout << "Elements in list: " << _networkList.size() << std::endl;
    }
}
