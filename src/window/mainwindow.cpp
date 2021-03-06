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
    _pool = QThreadPool::globalInstance();

    setupGUIElements();
    loadDataCollectionFiles();
    loadFirstNetworks();
    loadRefinedNetworks();
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < _networkList.size(); i++)
    {
        _networkList[i]->doTraining(false);
        removeGraphElements(i);
    }
    _pool->waitForDone();

    for(int i = 0; i < _networkList.size(); i++)
    {
        delete _networkList[i];
    }
    _networkList.clear();

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

    for(int i = 0; i < _collections.size(); i++)
    {
        delete _collections[i];
    }
    _collections.clear();
    _pool->clear();

    delete _ui;
    delete _reader;
}

//==========================================================
//  Private functions
//==========================================================

std::vector<QString> MainWindow::getNetworkNames()
{
    std::vector<QString> result;

    for(int i = 0; i < _networkList.size(); i++)
    {
        result.push_back(QString::fromStdString(_networkList.at(i)->getNetworkName()));
    }
    return result;
}

void MainWindow::setupGUIElements()
{
    QString sPath = "C:/";
    _dirModel = new QFileSystemModel(this);
    _dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    _fileModel = new QFileSystemModel(this);
    _fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    _ui->filelistview->setSelectionMode( QAbstractItemView::ExtendedSelection );

    _ui->dirtreeview->setModel(_dirModel);
    _ui->filelistview->setModel(_fileModel);
    _dirModel->setRootPath(sPath);
    //_ui->dirtreeview->setRootIndex(_dirModel->setRootPath(sPath));
    //_ui->dirtreeview->expand(_dirModel->index(QString("/")));
    _ui->dirtreeview->scrollTo(_dirModel->setRootPath("/"));
    _ui->filelistview->setRootIndex(_fileModel->setRootPath(QString("docs/")));


    _ui->customplot_accuracy->yAxis->setRange(100.0, 0.0);
    _ui->customplot_error->yAxis->setRange(100.0, 0.0);
    _ui->customplot_accuracy->xAxis->setRange(0.0, 100);
    _ui->customplot_error->xAxis->setRange(0.0, 100);
    _ui->customplot_accuracy->yAxis->setLabel("Accuracy%");
    _ui->customplot_accuracy->xAxis->setLabel("Epoch");
    _ui->customplot_error->yAxis->setLabel("Error%");
    _ui->customplot_error->xAxis->setLabel("Epoch");

    _ui->horizontalScrollBar_error->setRange(0,100);
}

void MainWindow::loadDataCollectionFiles()
{
    DataCollection* collection = new DataCollection();
    connect(collection, &DataCollection::signDataCollectionConsoleOutput,
            this, &MainWindow::signRecievedConsoleOutput);
    collection->setName("First training run");

    if(!IS_RELEASE)
    {
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
    }
    else
    {
        _reader->readCSVFile("docs/01_1_1_1477041067745.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/02_1_1_1477045014681.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/03_1_1_1477045972595.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/04_1_1_1477047900980.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/05_1_1_1477049323104.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/06_1_1_1477050706950.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/07_1_1_1477051538022.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/08_1_1_1477052157282.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/09_1_1_1477052835575.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/10_1_1_1477053563351.csv", 10, ",", Exercise::WALKING,collection);
        _reader->readCSVFile("docs/01_2_1_1477041118424.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/02_2_1_1477045059709.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/03_2_1_1477046007930.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/04_2_1_1477047930945.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/05_2_2_1477049591901.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/06_2_1_1477050768609.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/07_2_1_1477051569123.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/08_2_1_1477052185066.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/09_2_1_1477052872435.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
        _reader->readCSVFile("docs/10_2_1_1477053609606.csv", 10, ",", Exercise::FALLING_FORWARD,collection);
    }

    collection->createTrainingTestSets(5, 15);
    _collections.push_back(collection);
    _ui->listWidget_training_collections->addItem(QString::fromStdString(_collections[_collections.size()-1]->getName()));

    if(IS_RELEASE)
    {
        //Refined network bracket
        {
            DataCollection* collection2 = new DataCollection();
            connect(collection2, &DataCollection::signDataCollectionConsoleOutput,
                    this, &MainWindow::signRecievedConsoleOutput);
            collection2->setName("Refined network");

            _reader->readCSVFile("docs/01_1_1_1477041067745.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/02_1_1_1477045014681.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/03_1_1_1477045972595.csv", 10, ",", Exercise::WALKING,collection2);
            /*_reader->readCSVFile("docs/04_1_1_1477047900980.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/05_1_1_1477049323104.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/06_1_1_1477050706950.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/07_1_1_1477051538022.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/08_1_1_1477052157282.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/09_1_1_1477052835575.csv", 10, ",", Exercise::WALKING,collection2);
            _reader->readCSVFile("docs/10_1_1_1477053563351.csv", 10, ",", Exercise::WALKING,collection2);*/

            _reader->readCSVFile("docs/01_2_1_1477041118424.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/02_2_1_1477045059709.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/03_2_1_1477046007930.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            /*_reader->readCSVFile("docs/04_2_1_1477047930945.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/05_2_2_1477049591901.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/06_2_1_1477050768609.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/07_2_1_1477051569123.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/08_2_1_1477052185066.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/09_2_1_1477052872435.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);
            _reader->readCSVFile("docs/10_2_1_1477053609606.csv", 10, ",", Exercise::FALLING_FORWARD,collection2);*/

            _reader->readCSVFile("docs/01_3_1_1477041220590.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/02_3_1_1477045328924.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/03_3_1_1477046058881.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            /*_reader->readCSVFile("docs/04_3_1_1477047981075.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/05_3_1_1477049424551.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/06_3_1_1477050833494.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/07_3_1_1477051612410.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/08_3_1_1477052231621.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/09_3_1_1477052929641.csv", 10, ",", Exercise::STAIRS_UP,collection2);
            _reader->readCSVFile("docs/10_3_1_1477053667140.csv", 10, ",", Exercise::STAIRS_UP,collection2);*/

            _reader->readCSVFile("docs/01_4_1_1477041271435.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/02_4_1_1477045386208.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/03_4_1_1477046109686.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            /*_reader->readCSVFile("docs/04_4_1_1477048025319.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/05_4_1_1477049488661.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/06_4_1_1477050878249.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/07_4_1_1477051658876.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/08_4_1_1477052282044.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/09_4_1_1477052986450.csv", 10, ",", Exercise::SITTING_DOWN,collection2);
            _reader->readCSVFile("docs/10_4_1_1477053712577.csv", 10, ",", Exercise::SITTING_DOWN,collection2);*/

            _reader->readCSVFile("docs/01_5_1_1477041349163.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/02_5_1_1477045200480.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/03_5_1_1477046135953.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            /*_reader->readCSVFile("docs/04_5_1_1477048045533.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/05_5_1_1477049514138.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/06_5_1_1477050900170.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/07_5_1_1477051679326.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/08_5_1_1477052302370.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/09_5_1_1477053010324.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);
            _reader->readCSVFile("docs/10_5_1_1477053735599.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection2);*/

            _reader->readCSVFile("docs/01_6_1_1477041324858.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/02_6_1_1477045231262.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/03_6_1_1477046167179.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            /*_reader->readCSVFile("docs/04_6_1_1477048070060.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/05_6_1_1477049538067.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/06_6_1_1477050926698.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/07_6_1_1477051707590.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/08_6_1_1477052326376.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/09_6_1_1477053033307.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);
            _reader->readCSVFile("docs/10_6_1_1477053754755.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection2);*/

            collection2->createTrainingTestSets(5, 15);
            _collections.push_back(collection2);
            _ui->listWidget_training_collections->addItem(QString::fromStdString(_collections[_collections.size()-1]->getName()));
        }

        //ALL DATA BRACKET
        {
            DataCollection* collection3 = new DataCollection();
            connect(collection3, &DataCollection::signDataCollectionConsoleOutput,
                    this, &MainWindow::signRecievedConsoleOutput);
            collection3->setName("All experiments");

            _reader->readCSVFile("docs/01_1_1_1477041067745.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/01_1_2_1477041373851.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/01_1_3_1477041642758.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/01_2_1_1477041118424.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/01_2_2_1477041409610.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/01_2_3_1477041677536.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/01_3_1_1477041220590.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/01_3_2_1477041500980.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/01_3_3_1477041728380.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/01_4_1_1477041271435.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/01_4_2_1477041556972.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/01_4_3_1477041774187.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/01_5_1_1477041349163.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/01_5_2_1477041592254.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/01_5_3_1477041796471.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/01_6_1_1477041324858.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/01_6_2_1477041619147.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/01_6_3_1477041816852.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/02_1_1_1477045014681.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/02_1_2_1477045414491.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/02_1_3_1477045649781.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/02_2_1_1477045059709.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/02_2_2_1477045442440.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/02_2_3_1477045738776.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/02_3_1_1477045328924.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/02_3_2_1477045528637.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/02_3_3_1477045781829.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/02_4_1_1477045386208.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/02_4_2_1477045575769.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/02_4_3_1477045827621.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/02_5_1_1477045200480.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/02_5_2_1477045600691.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/02_5_3_1477045859736.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/02_6_1_1477045231262.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/02_6_2_1477045625034.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/02_6_3_1477045885243.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/03_1_1_1477045972595.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/03_1_2_1477046194366.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/03_1_3_1477046515117.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/03_2_1_1477046007930.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/03_2_2_1477046305590.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/03_2_3_1477046580120.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/03_3_1_1477046058881.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/03_3_2_1477046353556.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/03_3_3_1477046635202.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/03_4_1_1477046109686.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/03_4_2_1477046416593.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/03_4_3_1477046690063.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/03_5_1_1477046135953.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/03_5_2_1477046440039.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/03_5_3_1477046714582.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/03_6_1_1477046167179.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/03_6_2_1477046464098.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/03_6_3_1477046755793.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/04_1_1_1477047900980.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/04_1_2_1477048097661.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/04_1_3_1477048794314.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/04_2_1_1477047930945.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/04_2_2_1477048129281.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/04_2_3_1477048909995.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/04_3_1_1477048319055.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/04_3_2_1477048178894.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/04_3_3_1477048949083.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/04_4_1_1477048025319.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/04_4_2_1477048228784.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/04_4_3_1477048992225.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/04_5_1_1477048045533.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/04_5_2_1477048260035.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/04_5_3_1477049014027.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/04_6_1_1477048070060.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/04_6_2_1477048281907.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/04_6_3_1477049041717.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/05_1_1_1477049323104.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/05_1_2_1477049561661.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/05_1_3_1477049746406.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/05_2_2_1477049591901.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/05_2_3_1477049773169.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/05_3_1_1477049424551.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/05_3_2_1477049639224.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/05_3_3_1477049825829.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/05_4_1_1477049488661.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/05_4_2_1477049690857.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/05_4_3_1477049869306.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/05_5_1_1477049514138.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/05_5_2_1477049710714.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/05_5_3_1477049887256.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/05_6_1_1477049538067.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/05_6_2_1477049728322.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/05_6_3_1477049905366.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/06_1_1_1477050706950.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/06_1_2_1477050962434.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/06_1_3_1477051184293.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/06_2_1_1477050768609.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/06_2_2_1477050990639.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/06_2_3_1477051252254.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/06_3_1_1477050833494.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/06_3_2_1477051051452.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/06_3_3_1477051305299.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/06_4_1_1477050878249.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/06_4_2_1477051100934.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/06_4_3_1477051351384.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/06_5_1_1477050900170.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/06_5_2_1477051123341.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/06_5_3_1477051373403.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/06_6_1_1477050926698.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/06_6_2_1477051154858.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/06_6_3_1477051405354.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/07_1_1_1477051538022.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/07_1_1_1477051538022.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/07_1_3_1477051913923.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/07_2_1_1477051569123.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/07_2_2_1477051757654.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/07_2_3_1477051939931.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/07_3_1_1477051612410.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/07_3_2_1477051796715.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/07_3_3_1477051991634.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/07_4_1_1477051658876.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/07_4_2_1477051844660.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/07_4_3_1477052041996.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/07_5_1_1477051679326.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/07_5_2_1477051869951.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/07_5_3_1477052068625.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/07_6_1_1477051707590.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/07_6_2_1477051893545.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/07_6_3_1477052091582.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/08_1_1_1477052157282.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/08_1_2_1477052345201.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/08_1_3_1477052567151.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/08_2_1_1477052185066.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/08_2_2_1477052378593.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/08_2_3_1477052599421.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/08_3_1_1477052231621.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/08_3_2_1477052429756.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/08_3_3_1477052650651.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/08_4_1_1477052282044.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/08_4_2_1477052485147.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/08_4_3_1477052699573.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/08_5_1_1477052302370.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/08_5_2_1477052509510.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/08_5_3_1477052722150.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/08_6_1_1477052326376.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/08_6_2_1477052542899.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/08_6_3_1477052739874.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/09_1_1_1477052835575.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/09_1_2_1477053053952.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/09_1_3_1477053269953.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/09_2_1_1477052872435.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/09_2_2_1477053106636.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/09_2_3_1477053306515.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/09_3_1_1477052929641.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/09_3_2_1477053161415.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/09_3_3_1477053358165.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/09_4_1_1477052986450.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/09_4_2_1477053207798.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/09_4_3_1477053442443.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/09_5_1_1477053010324.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/09_5_2_1477053229534.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/09_5_3_1477053463101.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/09_6_1_1477053033307.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/09_6_2_1477053251133.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/09_6_3_1477053491923.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            _reader->readCSVFile("docs/10_1_1_1477053563351.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/10_1_2_1477053774772.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/10_1_3_1477053970534.csv", 10, ",", Exercise::WALKING,collection3);
            _reader->readCSVFile("docs/10_2_1_1477053609606.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/10_2_2_1477053816220.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/10_2_3_1477054000762.csv", 10, ",", Exercise::FALLING_FORWARD,collection3);
            _reader->readCSVFile("docs/10_3_1_1477053667140.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/10_3_2_1477053875250.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/10_3_3_1477054056884.csv", 10, ",", Exercise::STAIRS_UP,collection3);
            _reader->readCSVFile("docs/10_4_1_1477053712577.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/10_4_2_1477053913912.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/10_4_3_1477054120840.csv", 10, ",", Exercise::SITTING_DOWN,collection3);
            _reader->readCSVFile("docs/10_5_1_1477053735599.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/10_5_2_1477053934806.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/10_5_3_1477054150663.csv", 10, ",", Exercise::PICKUP_ITEM_SITTING,collection3);
            _reader->readCSVFile("docs/10_6_1_1477053754755.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/10_6_2_1477053952409.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);
            _reader->readCSVFile("docs/10_6_3_1477054170574.csv", 10, ",", Exercise::PICKUP_ITEM_STANDING,collection3);

            collection3->createTrainingTestSets(5, 15);
            _collections.push_back(collection3);
            _ui->listWidget_training_collections->addItem(QString::fromStdString(_collections[_collections.size()-1]->getName()));
        }
    }
}

void MainWindow::loadRefinedNetworks()
{
    std::vector<Network*> subNetworks;

    QuadraticCost* cost = new QuadraticCost();
    Network* net = new Network(16, {16,6}, 2, cost, DataType::ACCELEROMETER, "RefinedAccelerometer");
    connect(net, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net->initNetwork();
    net->setAutoDelete(false);

    _networkList.push_back(net);
    subNetworks.push_back(net);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net->getNetworkName()));

    QuadraticCost* cost2 = new QuadraticCost();
    Network* net2 = new Network(16, {16,6}, 2, cost2, DataType::GYRO, "RefinedGyroscope");
    connect(net2, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net2, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net2->initNetwork();
    net2->setAutoDelete(false);

    _networkList.push_back(net2);
    subNetworks.push_back(net2);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net2->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net2->getNetworkName()));

    QuadraticCost* cost3 = new QuadraticCost();
    Network* net3 = new Network(16, {2,3}, 2, cost3, DataType::COMPASS, "RefinedCompass");
    connect(net3, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net3, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net3->initNetwork();
    net3->setAutoDelete(false);

    _networkList.push_back(net3);
    subNetworks.push_back(net3);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net3->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net3->getNetworkName()));

    QuadraticCost* cost4 = new QuadraticCost();
    Network* net4 = new Network(subNetworks, {2,4}, 2, cost4, "RefinedTopNetwork");
    connect(net4, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net4, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net4->initNetworkFromSub(subNetworks);
    net4->setAutoDelete(false);

    _networkList.push_back(net4);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net4->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net4->getNetworkName()));
}

void MainWindow::loadFirstNetworks()
{
    std::vector<Network*> subNetworks;

    QuadraticCost* cost = new QuadraticCost();
    Network* net = new Network(16, {16,6}, 2, cost, DataType::ACCELEROMETER, "FirstAccelerometer");
    connect(net, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net->initNetwork();
    net->setAutoDelete(false);

    _networkList.push_back(net);
    subNetworks.push_back(net);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net->getNetworkName()));

    QuadraticCost* cost2 = new QuadraticCost();
    Network* net2 = new Network(16, {16,6}, 2, cost2, DataType::GYRO, "FirstGyroscope");
    connect(net2, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net2, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net2->initNetwork();
    net2->setAutoDelete(false);

    _networkList.push_back(net2);
    subNetworks.push_back(net2);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net2->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net2->getNetworkName()));

    QuadraticCost* cost3 = new QuadraticCost();
    Network* net3 = new Network(16, {16,6}, 2, cost3, DataType::COMPASS, "FirstCompass");
    connect(net3, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net3, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net3->initNetwork();
    net3->setAutoDelete(false);

    _networkList.push_back(net3);
    subNetworks.push_back(net3);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net3->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net3->getNetworkName()));

    QuadraticCost* cost4 = new QuadraticCost();
    Network* net4 = new Network(subNetworks, {16,6}, 2, cost4, "FirstTopNetwork");
    connect(net4, &Network::signNetworkConsoleOutput, this, &MainWindow::signRecievedConsoleOutput);
    connect(net4, &Network::signNetworkEpochComplete, this, &MainWindow::signRecievedEpochComplete);
    net4->initNetworkFromSub(subNetworks);
    net4->setAutoDelete(false);

    _networkList.push_back(net4);
    _ui->listWidget_networkList->addItem(QString::fromStdString(net4->getNetworkName()));
    _ui->listWidget_training_networks->addItem(QString::fromStdString(net4->getNetworkName()));
}

//==========================================================
//  Directory tree tab functions and slots
//==========================================================

void MainWindow::on_dirtreeview_clicked(const QModelIndex &index)
{
    QString sPath = _dirModel->fileInfo(index).absoluteFilePath();
    _ui->filelistview->setRootIndex(_fileModel->setRootPath(sPath));
}

void MainWindow::on_button_addFile_clicked()
{
    QModelIndexList list = _ui->filelistview->selectionModel()->selectedIndexes();

    foreach(const QModelIndex &index, list)
    {
        if(_fileModel->fileInfo(index).completeSuffix() == QString("csv"))
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
}

//==========================================================
//  Data collection tab functions and slots
//==========================================================

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

void MainWindow::signRecievedFileReadComplete(const QString &message)
{
    _ui->text_parseConsole->appendPlainText(message);
}

//==========================================================
//  Network Creation tab functions and slots
//==========================================================

void MainWindow::on_listWidget_networkList_itemClicked(QListWidgetItem *item)
{
    _selectedNetwork = _ui->listWidget_networkList->row(item);
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
    if(_selectedNetwork != -1 && _networkList.size() > 0
            && _selectedNetwork < _networkList.size())
    {
        _ui->listWidget_networkList->model()->removeRow(_selectedNetwork);
        _ui->listWidget_training_networks->model()->removeRow(_selectedNetwork);

        removeGraphElements(_selectedNetwork);

        delete _networkList[_selectedNetwork];
        _networkList.erase(_networkList.begin() + _selectedNetwork);
    }
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
    _ui->listWidget_networkList->item(index)->setText(name);
}

//==========================================================
//  Network Training tab functions and slots
//==========================================================

void MainWindow::on_checkBox_enableNoise_toggled(bool checked)
{
    _ui->spinBox_noiselevel->setEnabled(checked);
}

void MainWindow::on_pushButton_training_start_clicked()
{
    if(_ui->listWidget_training_collections->count() > 0 &&
            _ui->listWidget_training_networks->count() > 0 &&
            _ui->listWidget_training_collections->selectedItems().size() != 0 &&
             _ui->listWidget_training_networks->selectedItems().size() != 0 )
    {
        double momentum = _ui->doubleSpinBox_momentum->value();
        double learningrate = _ui->doubleSpinBox_learningRate->value();
        double targetaccuracy = _ui->doubleSpinBox_targetaccuracy->value();
        double gaussiandeviation = _ui->spinBox_noiselevel->value() * 0.01;
        int epoch = _ui->spinBox_maxepochs->value();
        bool usenoise = _ui->checkBox_enableNoise->isChecked();
        bool trainsubs = _ui->checkBox_trainsubsfirst->isChecked();
        int i = _ui->listWidget_training_networks->currentRow();

        _networkList[i]->setLearningParameters(learningrate, momentum);
        _networkList[i]->setMaxEpochs(epoch);
        _networkList[i]->setTargetAccuracy(targetaccuracy);
        _networkList[i]->enableNoise(usenoise);
        _networkList[i]->setNoiseParameters(gaussiandeviation);
        _networkList[i]->setTrainSubNetworksFirst(trainsubs);
        _networkList[i]->setDataCollection(_collections[_ui->listWidget_training_collections->currentRow()]);
        _networkList[i]->doTraining(true);
        _networkList[i]->doTesting(false);

        createGraph(i, _networkList[i]->getNetworkID());


        _pool->start(_networkList[i]);
    }
}

void MainWindow::on_pushButton_training_reset_clicked()
{
    int networkIndex = _ui->listWidget_training_networks->currentRow();
    _networkList[networkIndex]->resetNetwork();
    removeGraphElements(networkIndex);
}

void MainWindow::on_pushButton_training_stop_clicked()
{
    int i = _ui->listWidget_training_networks->currentRow();
    _networkList[i]->doTraining(false);
}

void MainWindow::on_pushButton_training_test_clicked()
{
    if(_ui->listWidget_training_collections->count() > 0 &&
            _ui->listWidget_training_networks->count() > 0 &&
            _ui->listWidget_training_collections->selectedItems().size() != 0 &&
             _ui->listWidget_training_networks->selectedItems().size() != 0 )
    {
        double momentum = _ui->doubleSpinBox_momentum->value();
        double learningrate = _ui->doubleSpinBox_learningRate->value();
        double targetaccuracy = _ui->doubleSpinBox_targetaccuracy->value();
        double gaussiandeviation = _ui->spinBox_noiselevel->value() * 0.01;
        int epoch = _ui->spinBox_maxepochs->value();
        bool usenoise = _ui->checkBox_enableNoise->isChecked();
        bool trainsubs = _ui->checkBox_trainsubsfirst->isChecked();
        int i = _ui->listWidget_training_networks->currentRow();

        _networkList[i]->setLearningParameters(learningrate, momentum);
        _networkList[i]->setMaxEpochs(epoch);
        _networkList[i]->setTargetAccuracy(targetaccuracy);
        _networkList[i]->enableNoise(usenoise);
        _networkList[i]->setNoiseParameters(gaussiandeviation);
        _networkList[i]->setTrainSubNetworksFirst(trainsubs);
        _networkList[i]->setDataCollection(_collections[_ui->listWidget_training_collections->currentRow()]);
        _networkList[i]->doTraining(true);
        _networkList[i]->doTesting(true);

        _pool->start(_networkList[i]);
    }
}

void MainWindow::on_pushButton_graphdisplaysettings_clicked()
{
    for(int i = 0; i < _checkboxGraphDisplay.size(); i++)
    {
        _ui->customplot_accuracy->graph(i)->setVisible(_checkboxGraphDisplay[i]->isChecked());
        _ui->customplot_error->graph(i)->setVisible(_checkboxGraphDisplay[i]->isChecked());
    }

    for(int i = 0; i < _comboboxGraphDisplay.size(); i++)
    {
        int index = _comboboxGraphDisplay[i]->currentIndex();
        QColor colour;

        switch(index)
        {
        case 0: colour = Qt::red; break;
        case 1: colour = Qt::green; break;
        case 2: colour = Qt::blue; break;
        case 3: colour = Qt::cyan; break;
        case 4: colour = Qt::magenta; break;
        case 5: colour = Qt::yellow; break;
        default: colour = Qt::black; break;
        }
        _ui->customplot_accuracy->graph(i)->setPen(colour);
        _ui->customplot_error->graph(i)->setPen(colour);
    }
    _ui->customplot_error->replot();
    _ui->customplot_accuracy->replot();
}

void MainWindow::on_horizontalScrollBar_error_valueChanged(int value)
{
    _ui->customplot_error->xAxis->setRange(value, 100, Qt::AlignLeft);
    _ui->customplot_error->replot();
    /*_ui->customplot_accuracy->xAxis->setRange(value, 100, Qt::AlignLeft);
    _ui->customplot_accuracy->replot();*/
    _ui->horizontalScrollBar_accuracy->setValue(value);
}

void MainWindow::on_horizontalScrollBar_accuracy_valueChanged(int value)
{
    _ui->customplot_accuracy->xAxis->setRange(value, 100, Qt::AlignLeft);
    _ui->customplot_accuracy->replot();
    /*_ui->customplot_error->xAxis->setRange(value, 100, Qt::AlignLeft);
    _ui->customplot_error->replot();*/
    _ui->horizontalScrollBar_error->setValue(value);
}

void MainWindow::removeGraphElements(int networkIndex)
{
    if(_idsForNetworkGraphs.size() > 0)
    {
        int graph_index = 0;
        int networkId = _networkList[networkIndex]->getNetworkID();

        bool hasGraph = false;
        for(int i = 0; i < _idsForNetworkGraphs.size(); i++)
        {
            if(_idsForNetworkGraphs[i] == networkId)
            {
                hasGraph = true;
                graph_index = 2*i;
                _idsForNetworkGraphs.erase(_idsForNetworkGraphs.begin() + i);
                break;
            }
        }
        if(!hasGraph)
        {
            return;
        }

        _ui->gridLayout_graphdisplay->removeWidget(_checkboxGraphDisplay[graph_index]);
        _ui->gridLayout_graphdisplay->removeWidget(_checkboxGraphDisplay[graph_index+1]);
        _ui->gridLayout_graphdisplay->removeWidget(_comboboxGraphDisplay[graph_index]);
        _ui->gridLayout_graphdisplay->removeWidget(_comboboxGraphDisplay[graph_index+1]);
        _ui->gridLayout_graphdisplay->removeWidget(_lineeditGraphDisplay[graph_index]);
        _ui->gridLayout_graphdisplay->removeWidget(_lineeditGraphDisplay[graph_index+1]);

        delete _lineeditGraphDisplay[graph_index];
        delete _comboboxGraphDisplay[graph_index];
        delete _checkboxGraphDisplay[graph_index];
        delete _lineeditGraphDisplay[graph_index+1];
        delete _comboboxGraphDisplay[graph_index+1];
        delete _checkboxGraphDisplay[graph_index+1];

        _lineeditGraphDisplay.erase(_lineeditGraphDisplay.begin() + graph_index);
        _comboboxGraphDisplay.erase(_comboboxGraphDisplay.begin() + graph_index);
        _checkboxGraphDisplay.erase(_checkboxGraphDisplay.begin() + graph_index);
        _lineeditGraphDisplay.erase(_lineeditGraphDisplay.begin() + graph_index);
        _comboboxGraphDisplay.erase(_comboboxGraphDisplay.begin() + graph_index);
        _checkboxGraphDisplay.erase(_checkboxGraphDisplay.begin() + graph_index);

        _ui->customplot_accuracy->removeGraph(graph_index+1);
        _ui->customplot_accuracy->removeGraph(graph_index);
        _ui->customplot_error->removeGraph(graph_index+1);
        _ui->customplot_error->removeGraph(graph_index);
        if(_ui->customplot_accuracy->graphCount() == 0)
        {
            _ui->customplot_accuracy->xAxis->setRange(0, 100);
            _ui->horizontalScrollBar_accuracy->setRange(0,100);
        }
        if(_ui->customplot_error->graphCount() == 0)
        {
            _ui->customplot_error->xAxis->setRange(0, 100);
            _ui->horizontalScrollBar_error->setRange(0,100);
        }
        _ui->customplot_accuracy->replot();
        _ui->customplot_error->replot();
    }
}

void MainWindow::createGraph(int networkIndex, int networkId)
{
    if(networkIndex == -1)
    {
        for(int i = 0; i < _networkList.size(); i++)
        {
            if(_networkList[i]->getNetworkID() == networkId)
            {
                networkIndex = i;
                break;
            }
        }
    }

    if(_networkList[networkIndex]->hasSubNetworks())
    {
        std::vector<Network*> temp = _networkList[networkIndex]->getSubNetworks();

        for(int i = 0; i < temp.size(); i++)
        {
            int exists = hasGraph(temp[i]->getNetworkID());

            if(exists == -1)
            {
                createGraph(-1, temp[i]->getNetworkID());
            }
        }
    }

    if(hasGraph(networkId) != -1)
    {
        return;
    }

    _idsForNetworkGraphs.push_back(networkId);
    int index_graph = 2 * (_idsForNetworkGraphs.size() - 1);

    QString accTraName = QString::fromStdString(_networkList[networkIndex]->getNetworkName()) +
                        " training accuracy";
    QString accTesName = QString::fromStdString(_networkList[networkIndex]->getNetworkName()) +
                        " testing accuracy";

    _ui->customplot_accuracy->addGraph();
    _ui->customplot_accuracy->addGraph();
    _ui->customplot_accuracy->graph(index_graph)->setPen(QPen(Qt::red));
    _ui->customplot_accuracy->graph(index_graph)->setName(accTraName);
    _ui->customplot_accuracy->graph(index_graph+1)->setName(accTesName);

    QString errTraName = QString::fromStdString(_networkList[networkIndex]->getNetworkName()) +
                        " training error";
    QString errTesName = QString::fromStdString(_networkList[networkIndex]->getNetworkName()) +
                        " testing error";
    _ui->customplot_error->addGraph();
    _ui->customplot_error->addGraph();
    _ui->customplot_error->graph(index_graph)->setPen(QPen(Qt::red));
    _ui->customplot_error->graph(index_graph)->setName(errTraName);
    _ui->customplot_error->graph(index_graph+1)->setName(errTesName);

    QStringList colours = {"Red", "Green", "Blue", "Cyan", "Magenta", "Yellow", "Black"};

    QString checkboxNameTest = "checkbox_test_" + QString::fromStdString(_networkList[networkIndex]->getNetworkName());
    QString comboboxNameTest = "combobox_test_" + QString::fromStdString(_networkList[networkIndex]->getNetworkName());
    QString lineeditNameTest = "lineedit_test_" + QString::fromStdString(_networkList[networkIndex]->getNetworkName());
    QString checkboxNameTrai = "checkbox_training_" + QString::fromStdString(_networkList[networkIndex]->getNetworkName());
    QString comboboxNameTrai = "combobox_training_" + QString::fromStdString(_networkList[networkIndex]->getNetworkName());
    QString lineeditNameTrai = "lineedit_training_" + QString::fromStdString(_networkList[networkIndex]->getNetworkName());


    QCheckBox* checkbox_test = new QCheckBox(_ui->scrollAreaWidgetContents_graphdisplaysettings);
    checkbox_test->setObjectName(checkboxNameTest);
    checkbox_test->setText("");
    checkbox_test->setChecked(true);

    QLineEdit* lineedit_test = new QLineEdit(_ui->scrollAreaWidgetContents_graphdisplaysettings);
    lineedit_test->setObjectName(lineeditNameTest);
    lineedit_test->setReadOnly(true);
    lineedit_test->setText(QString::fromStdString(_networkList[networkIndex]->getNetworkName()) + " Testing");

    QComboBox* combobox_test = new QComboBox(_ui->scrollAreaWidgetContents_graphdisplaysettings);
    combobox_test->setObjectName(comboboxNameTest);
    combobox_test->insertItems(0, colours);
    combobox_test->setCurrentIndex(2);

    QCheckBox* checkbox_trai = new QCheckBox(_ui->scrollAreaWidgetContents_graphdisplaysettings);
    checkbox_trai->setObjectName(checkboxNameTrai);
    checkbox_trai->setText("");
    checkbox_trai->setChecked(true);

    QLineEdit* lineedit_trai = new QLineEdit(_ui->scrollAreaWidgetContents_graphdisplaysettings);
    lineedit_trai->setObjectName(lineeditNameTrai);
    lineedit_trai->setReadOnly(true);
    lineedit_trai->setText(QString::fromStdString(_networkList[networkIndex]->getNetworkName()) + " Training");

    QComboBox* combobox_trai = new QComboBox(_ui->scrollAreaWidgetContents_graphdisplaysettings);
    combobox_trai->setObjectName(comboboxNameTrai);
    combobox_trai->insertItems(0, colours);

    _ui->gridLayout_graphdisplay->addWidget(lineedit_trai, index_graph+1, 0, 1, 1);
    _ui->gridLayout_graphdisplay->addWidget(combobox_trai, index_graph+1, 1, 1, 1);
    _ui->gridLayout_graphdisplay->addWidget(checkbox_trai, index_graph+1, 2, 1, 1);
    _ui->gridLayout_graphdisplay->addWidget(lineedit_test, index_graph+2, 0, 1, 1);
    _ui->gridLayout_graphdisplay->addWidget(combobox_test, index_graph+2, 1, 1, 1);
    _ui->gridLayout_graphdisplay->addWidget(checkbox_test, index_graph+2, 2, 1, 1);

    _comboboxGraphDisplay.push_back(combobox_trai);
    _comboboxGraphDisplay.push_back(combobox_test);
    _lineeditGraphDisplay.push_back(lineedit_trai);
    _lineeditGraphDisplay.push_back(lineedit_test);
    _checkboxGraphDisplay.push_back(checkbox_trai);
    _checkboxGraphDisplay.push_back(checkbox_test);
}

int MainWindow::hasGraph(int networkId)
{
    if(_idsForNetworkGraphs.size() > 0)
    {
        int result = -1;
        for(int i = 0; i < _idsForNetworkGraphs.size(); i++)
        {
            if(_idsForNetworkGraphs[i] == networkId)
            {
                result = i;
                break;
            }
        }
        return result;
    }
    else
    {
        return -1;
    }
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
        createGraph(index_network, id);
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

//==========================================================
//  Console tab functions and slots
//==========================================================

void MainWindow::signRecievedConsoleOutput(const QString &message)
{
    _ui->consoleOutput->appendPlainText(message);
}

void MainWindow::on_pushButton_console_clear_clicked()
{
    _ui->consoleOutput->clear();
}
