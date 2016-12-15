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
    connect(_reader, &CSVReader::signFileReadComplete, this, &MainWindow::signRecievedFileReadComplete);
    connect(_reader, &CSVReader::signFileReadConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);

    QString sPath = "../";
    _dirModel = new QFileSystemModel(this);
    _dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    _fileModel = new QFileSystemModel(this);
    _fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    _ui->filelistview->setSelectionMode( QAbstractItemView::ExtendedSelection );

    _ui->dirtreeview->setModel(_dirModel);
    _ui->filelistview->setModel(_fileModel);
    _ui->dirtreeview->setRootIndex(_dirModel->setRootPath(sPath));
    _ui->filelistview->setRootIndex(_fileModel->setRootPath(sPath));

    _pool = QThreadPool::globalInstance();

    _ui->customplot_accuracy->yAxis->setRange(100.0, 0.0);
    _ui->customplot_error->yAxis->setRange(100.0, 0.0);
    _ui->customplot_accuracy->xAxis->setRange(0.0, 100);
    _ui->customplot_error->xAxis->setRange(0.0, 100);
    _ui->customplot_accuracy->yAxis->setLabel("Accuracy%");
    _ui->customplot_accuracy->xAxis->setLabel("Epoch");
    _ui->customplot_error->yAxis->setLabel("Error%");
    _ui->customplot_error->xAxis->setLabel("Epoch");

    _ui->horizontalScrollBar_error->setRange(0,100);

    DataCollection* collection = new DataCollection();
    connect(collection, &DataCollection::signDataCollectionConsoleOutput,
            this, &MainWindow::signRecievedConsoleOutput);
    collection->setName("Default data collection");

    _reader->readCSVFile("../res/docs/01_1_1_1477041067745.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/02_1_1_1477045014681.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/03_1_1_1477045972595.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/04_1_1_1477047900980.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/05_1_1_1477049323104.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/06_1_1_1477050706950.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/07_1_1_1477051538022.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/08_1_1_1477052157282.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/09_1_1_1477052835575.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/10_1_1_1477053563351.csv", 10, ",", Exercise::WALKING,collection);
    _reader->readCSVFile("../res/docs/01_2_1_1477041118424.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/02_2_1_1477045059709.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/03_2_1_1477046007930.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/04_2_1_1477047930945.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/05_2_2_1477049591901.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/06_2_1_1477050768609.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/07_2_1_1477051569123.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/08_2_1_1477052185066.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/09_2_1_1477052872435.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    _reader->readCSVFile("../res/docs/10_2_1_1477053609606.csv", 10, ",", Exercise::FALLING_FORWARD,collection);



    QuadraticCost* cost = new QuadraticCost();
    Network* net = new Network(16, {16,6}, 2, cost, DataType::ACCELEROMETER, "DefaultAccelerometer");
    connect(net, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net->initNetwork();
    net->setAutoDelete(false);

    _networkList.push_back(net);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net->getNetworkName()));



    collection->createTrainingTestSets(5, 15);
    _collections.push_back(collection);

    _ui->listWidget_training_collections->addItem(QString::fromStdString(_collections[_collections.size()-1]->getName()));
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < _checkboxCollection.size(); i++)
    {
        delete _checkboxCollection[i];
    }
    _checkboxCollection.clear();

    for(int i = 0; i < _comboboxCollection.size(); i++)
    {
        delete _comboboxCollection[i];
    }
    _comboboxCollection.clear();

    for(int i = 0; i < _lineeditCollection.size(); i++)
    {
        delete _lineeditCollection[i];
    }
    _lineeditCollection.clear();

    /*for(int i = 0; i < _collections.size(); i++)
    {
        delete _collections[i];
    }
    _collections.clear();*/

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
    //_selectedFile._fpath = _fileModel->fileInfo(index).absoluteFilePath();
    //_selectedFile._fname = _fileModel->fileInfo(index).fileName();
}

void MainWindow::on_button_addFile_clicked()
{
    QModelIndexList list = _ui->filelistview->selectionModel()->selectedIndexes();

    foreach(const QModelIndex &index, list)
    {
        _selectedFile._fpath = _fileModel->fileInfo(index).absoluteFilePath();
        _selectedFile._fname = _fileModel->fileInfo(index).fileName();

        bool checker = false;
        for(int i = 0; i < _fileNames.size(); i++)
        {
            if(_selectedFile._fpath == _fileNames[i]._fpath)
                checker = true;
        }
        if(!checker)
        {
            _fileNames.push_back(_selectedFile);

            QString numString = QString::number(_fileNames.size()-1);
            QString comboName = "comboBox_collection_" + numString;
            QString checkName = "checkbox_collection_" + numString;
            QString lineeName = "lineedit_collection_" + numString;

            QComboBox *box = new QComboBox(this);
            box->setObjectName(comboName);
            box->insertItem(0,"Walking");
            box->insertItem(1,"Falling Forward");
            box->insertItem(2,"Walk up stairs");
            box->insertItem(3,"Sitting down");
            box->insertItem(4,"Pick up item sitting");
            box->insertItem(5,"Pick up item standing");

            QCheckBox *check = new QCheckBox(this);
            check->setObjectName(checkName);

            QLineEdit *line = new QLineEdit(this);
            line->setObjectName(lineeName);
            line->setReadOnly(true);
            line->setText(_selectedFile._fname);

            _ui->gridLayout_2->addWidget(check, _fileNames.size()-1, 0, 1, 1);
            _ui->gridLayout_2->addWidget(line, _fileNames.size()-1, 1, 1, 1);
            _ui->gridLayout_2->addWidget(box, _fileNames.size()-1, 2, 1, 1);

            _comboboxCollection.push_back(box);
            _checkboxCollection.push_back(check);
            _lineeditCollection.push_back(line);
        }
    }
}

void MainWindow::on_pushButton_network_create_clicked()
{
    std::vector<QString> names = getNetworkNames();

    NetworkCreationDialog* dialog = new NetworkCreationDialog(this, &names);
    //connect(dialog, SIGNAL(signNetworkCreation(int,std::vector<int>,int,QString)), this, signNetworkCreationRecieved);
    connect(dialog, &NetworkCreationDialog::signNetworkCreation, this, &MainWindow::signRecievedNetworkCreation);
    connect(dialog, &NetworkCreationDialog::signNetworkCreationFromSubs, this, &MainWindow::signRecievedNetworkCreationFromSubs);
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
    net->setAutoDelete(false);

    _networkList.push_back(net);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net->getNetworkName()));
}

void MainWindow::signRecievedNetworkCreationFromSubs(std::vector<int> indexes, std::vector<int> hidden, int output, QString name, CostCalc calc)
{
    Cost* costCalculator;
    if(calc == CostCalc::Quadratic) costCalculator = new QuadraticCost();
    else if(calc == CostCalc::CrossEntropy) costCalculator = new CrossEntropyCost();

    std::vector<Network*> subNetworks;

    for(int i = 0; i < indexes.size(); i++)
    {
        subNetworks.push_back(_networkList[i]);
    }

    Network* net = new Network(subNetworks, hidden, output, costCalculator, name.toStdString());

    connect(net, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net->initNetworkFromSub(subNetworks);
    net->setAutoDelete(false);

    _networkList.push_back(net);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net->getNetworkName()));
}

void MainWindow::signRecievedNetworkEdit(const int index, const int in, const std::vector<int> hidden, const int out, const QString name, const DataType type, const CostCalc calc)
{
    Cost* costCalculator;
    if(calc == CostCalc::Quadratic) costCalculator = new QuadraticCost();
    else if(calc == CostCalc::CrossEntropy) costCalculator = new CrossEntropyCost();

    _networkList[index]->editNetwork(in, hidden, out, name.toStdString(), type, costCalculator);

    /*delete _networkList[index];
    _networkList[index] = new Network(in, hidden, out, costCalculator, type, name.toStdString());
    _networkList[index]->initNetwork();
    _networkList[index]->setAutoDelete(false);

    connect(_networkList[index], &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(_networkList[index], &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);*/

    _ui->listWidget_networkList->item(index)->setText(name);
}

void MainWindow::signRecievedNetworkTrainingComplete()
{
    _currentGraphNetwork++;
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

void MainWindow::signRecievedEpochComplete(const int id, const int epoch, const double trainingError, const double trainingAccuracy, const double testingError, const double testingAccuracy)
{
    bool graphExists = false;
    int index_graph; // graph index
    if(_idsForNetworkGraphs.size() > 0)
    {
        for(int i = 0; i < _idsForNetworkGraphs.size(); i++)
        {
            if(_idsForNetworkGraphs[i] == id)
            {
                graphExists = true;
                index_graph = 2*i;
                break;
            }
        }
    }

    int index_network; //network index
    for(int i = 0; i < _networkList.size(); i++)
    {
        if(_networkList[i]->getNetworkID() == id)
        {
            index_network = i;
            break;
        }
    }

    if(!graphExists)
    {
        _idsForNetworkGraphs.push_back(id);
        index_graph = 2 * (_idsForNetworkGraphs.size() - 1);

        QString accTraName = QString::fromStdString(_networkList[index_network]->getNetworkName()) +
                            " training accuracy";
        QString accTesName = QString::fromStdString(_networkList[index_network]->getNetworkName()) +
                            " testing accuracy";

        _ui->customplot_accuracy->addGraph();
        _ui->customplot_accuracy->addGraph();
        _ui->customplot_accuracy->graph(index_graph)->setPen(QPen(Qt::red));
        _ui->customplot_accuracy->graph(index_graph)->setName(accTraName);
        _ui->customplot_accuracy->graph(index_graph+1)->setName(accTesName);

        QString errTraName = QString::fromStdString(_networkList[index_network]->getNetworkName()) +
                            " training error";
        QString errTesName = QString::fromStdString(_networkList[index_network]->getNetworkName()) +
                            " testing error";
        _ui->customplot_error->addGraph();
        _ui->customplot_error->addGraph();
        _ui->customplot_error->graph(index_graph)->setPen(QPen(Qt::red));
        _ui->customplot_error->graph(index_graph)->setName(errTraName);
        _ui->customplot_error->graph(index_graph+1)->setName(errTesName);
    }

    if(epoch > _ui->customplot_accuracy->xAxis->range().upper)
    {
         _ui->customplot_accuracy->xAxis->setRange(0.0, epoch);

         int minRange = 0;
         if(epoch - 100 > 0)
         {
             minRange = epoch - 100;
         }
          _ui->customplot_accuracy->xAxis->setRange(minRange, epoch);
          _ui->horizontalScrollBar_accuracy->setRange(0,epoch);
    }
    _ui->horizontalScrollBar_accuracy->setValue(epoch-100);

    if(epoch > _ui->customplot_error->xAxis->range().upper)
    {
        int minRange = 0;
        if(epoch - 100 > 0)
        {
            minRange = epoch - 100;
        }
         _ui->customplot_error->xAxis->setRange(minRange, epoch);
         _ui->horizontalScrollBar_error->setRange(0,epoch);

    }
    _ui->horizontalScrollBar_error->setValue(epoch-100);

    _ui->customplot_accuracy->graph(index_graph)->addData(epoch, trainingAccuracy);
    _ui->customplot_accuracy->graph(index_graph+1)->addData(epoch, testingAccuracy);

    _ui->customplot_error->graph(index_graph)->addData(epoch, trainingError*100);
    _ui->customplot_error->graph(index_graph+1)->addData(epoch, testingError*100);

    _ui->customplot_accuracy->replot();
    _ui->customplot_error->replot();

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

void MainWindow::on_checkBox_enableNoise_toggled(bool checked)
{
    _ui->spinBox_noiselevel->setEnabled(checked);
}

void MainWindow::on_pushButton_doc_parse_clicked()
{
    std::vector<Exercise> exercises;
    std::vector<QString> paths;

    QString name = _ui->lineEdit_doc_name->text();

    for(int i = 0; i < _comboboxCollection.size(); i++)
    {
        if(_checkboxCollection[i]->isChecked())
        {
            int tempEx = _comboboxCollection[i]->currentIndex();
            Exercise ex;

            switch(tempEx)
            {
            case 0: ex = Exercise::WALKING; break;
            case 1: ex = Exercise::FALLING_FORWARD; break;
            case 2: ex = Exercise::STAIRS_UP; break;
            case 3: ex = Exercise::SITTING_DOWN; break;
            case 4: ex = Exercise::PICKUP_ITEM_SITTING; break;
            case 5: ex = Exercise::PICKUP_ITEM_STANDING; break;
            default: ex = Exercise::UNKNOWN; break;
            }
            exercises.push_back(ex);
            paths.push_back(_fileNames[i]._fpath);
        }
    }

    DataCollection* collection = new DataCollection();
    connect(collection, &DataCollection::signDataCollectionConsoleOutput,
            this, &MainWindow::signRecievedConsoleOutput);
    collection->setName(name.toStdString());
    for(int i = 0; i < paths.size(); i++)
    {
        _reader->readCSVFile(paths[i].toStdString().c_str(), _ui->spinBox_doc_entries->value(),
                             _ui->lineEdit_doc_separator->text().toStdString().c_str(), exercises[i], collection );
    }
    _collections.push_back(collection);
}

void MainWindow::on_pushButton_doc_createsets_clicked()
{
    int setSize = _ui->spinBox_doc_setSize->value();
    double trainingSize = _ui->spinBox_doc_training->value();

    _collections[_collections.size()-1]->createTrainingTestSets(setSize, trainingSize);
    _ui->listWidget_training_collections->addItem(QString::fromStdString(_collections[_collections.size()-1]->getName()));
}

void MainWindow::on_pushButton_training_start_clicked()
{
    double momentum = _ui->doubleSpinBox_momentum->value();
    double learningrate = _ui->doubleSpinBox_learningRate->value();
    double targetaccuracy = _ui->doubleSpinBox_targetaccuracy->value();
    double gaussiandeviation = _ui->spinBox_noiselevel->value() * 0.01;
    int epoch = _ui->spinBox_maxepochs->value();
    bool usenoise = _ui->checkBox_enableNoise->isChecked();
    bool trainsubs = _ui->checkBox_trainsubsfirst->isChecked();

    /*_ui->customplot_accuracy->xAxis->setRange(0.0, epoch);
    _ui->customplot_error->xAxis->setRange(0.0, epoch);*/
    _ui->customplot_accuracy->replot();
    _ui->customplot_error->replot();

    _currentGraphNetwork = 0;

    if(_ui->listWidget_training_collections->count() > 0 &&
            _ui->listWidget_training_networks->count() > 0 &&
            _ui->listWidget_training_collections->selectedItems().size() != 0 &&
             _ui->listWidget_training_networks->selectedItems().size() != 0 )
    {
        int i = _ui->listWidget_training_networks->currentRow();

        _networkList[i]->setLearningParameters(learningrate, momentum);
        _networkList[i]->setMaxEpochs(epoch);
        _networkList[i]->setTargetAccuracy(targetaccuracy);
        _networkList[i]->enableNoise(usenoise);
        _networkList[i]->setNoiseParameters(gaussiandeviation);
        _networkList[i]->setTrainSubNetworksFirst(trainsubs);

        _networkList[i]->setDataCollection(_collections[_ui->listWidget_training_collections->currentRow()]);
        _networkList[i]->doTraining(true);

        _currentGraphNetwork = 0;
        _pool->start(_networkList[i]);
    }

}

void MainWindow::on_pushButton_training_reset_clicked()
{
    int networkIndex = _ui->listWidget_training_networks->currentRow();
    _networkList[networkIndex]->resetNetwork();

    if(_idsForNetworkGraphs.size() > 0)
    {
        int graph_index = 0;
        int networkId = _networkList[networkIndex]->getNetworkID();

        for(int i = 0; i < _idsForNetworkGraphs.size(); i++)
        {
            if(_idsForNetworkGraphs[i] == networkId)
            {
                graph_index = 2*i;
                _idsForNetworkGraphs.erase(_idsForNetworkGraphs.begin() + i);
                break;
            }
        }

        _ui->customplot_accuracy->removeGraph(graph_index+1);
        _ui->customplot_accuracy->removeGraph(graph_index);
        _ui->customplot_error->removeGraph(graph_index+1);
        _ui->customplot_error->removeGraph(graph_index);

        if(_ui->customplot_accuracy->graphCount() == 0)
        {
            _ui->customplot_accuracy->xAxis->setRange(0, 100);
        }
        if(_ui->customplot_error->graphCount() == 0)
        {
            _ui->customplot_error->xAxis->setRange(0, 100);
        }

        _ui->customplot_accuracy->replot();
        _ui->customplot_error->replot();
    }
}

void MainWindow::on_pushButton_clicked()
{
    int i = _ui->listWidget_training_networks->currentRow();

    _networkList[i]->doTraining(false);
}

void MainWindow::on_horizontalScrollBar_error_valueChanged(int value)
{
    //if(qAbs(_ui->customplot_error->xAxis->range().lower - (value)) > 1)
    //{
        _ui->customplot_error->xAxis->setRange(value, 100, Qt::AlignLeft);
        _ui->customplot_error->replot();
    //}
}

void MainWindow::on_horizontalScrollBar_accuracy_valueChanged(int value)
{
    //if(qAbs(_ui->customplot_error->xAxis->range().lower - (value)) > 10)
    //{
        _ui->customplot_accuracy->xAxis->setRange(value, 100, Qt::AlignLeft);
        _ui->customplot_accuracy->replot();
    //}
}
