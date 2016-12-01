#include "../include/window/networkcreationdialog.h"
#include "ui_networkcreationdialog.h"

NetworkCreationDialog::NetworkCreationDialog(QWidget *parent,std::vector<QString>* networkNames) :
    QDialog(parent),
    ui(new Ui::NetworkCreationDialog),
    _networkNames(networkNames)
{
    ui->setupUi(this);
    updateHiddenLayerSection();
    populateNetworkNames();
    _isEditing = false;
}

NetworkCreationDialog::~NetworkCreationDialog()
{
    for(QSpinBox* f: _hiddenLineEdit)
    {
        delete f;
    }
    for(QLabel* f: _hiddenLabel)
    {
        delete f;
    }
    for(QComboBox* f: _hiddenComboBox)
    {
         delete f;
    }
    _hiddenComboBox.clear();
    _hiddenLabel.clear();
    _hiddenLineEdit.clear();

    delete ui;
}

void NetworkCreationDialog::editNetwork(int index, int input, std::vector<int> hidden, int output, QString name, DataType type, CostCalc calc)
{
    _isEditing = true;
    _index = index;

    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabText(0, "Edit network");

    ui->spinBox_input->setValue(input);
    ui->spinBox_output->setValue(output);
    ui->lineEdit_name->setText(name);

    _numHiddenLayers = hidden.size();
    updateHiddenLayerSection();
    updateHiddenLayerValues(hidden);

    switch(type)
    {
    case DataType::ACCELEROMETER:
        ui->comboBox_netType->setCurrentIndex(0);
        break;
    case DataType::GYRO:
        ui->comboBox_netType->setCurrentIndex(1);
        break;
    case DataType::COMPASS:
        ui->comboBox_netType->setCurrentIndex(2);
        break;
    case DataType::UK:
        ui->comboBox_netType->setCurrentIndex(3);
        break;
    }

    index = ui->comboBox_cost->currentIndex();
    switch(calc)
    {
    case CostCalc::Quadratic:
        ui->comboBox_cost->setCurrentIndex(0);
        break;
    case CostCalc::CrossEntropy:
        ui->comboBox_cost->setCurrentIndex(1);
        break;
    }

}

void NetworkCreationDialog::on_pushButton_clicked()
{
    _numHiddenLayers++;
    updateHiddenLayerSection();
}

void NetworkCreationDialog::on_pushButton_2_clicked()
{
    if(_numHiddenLayers > 1)
    {
        _numHiddenLayers--;
        updateHiddenLayerSection();
    }
}

void NetworkCreationDialog::on_buttonBox_accepted()
{
    int numInput = ui->spinBox_input->value();
    int numOutput = ui->spinBox_output->value();

    std::vector<int> numHidden;

    for(int i = 0; i < _numHiddenLayers; i++)
    {
        numHidden.push_back(_hiddenLineEdit[i]->value());
    }

    QString name = ui->lineEdit_name->text();

    DataType type;

    int index = ui->comboBox_netType->currentIndex();
    switch(index)
    {
    case 0:
        type = DataType::ACCELEROMETER;
        break;
    case 1:
        type = DataType::GYRO;
        break;
    case 2:
        type = DataType::COMPASS;
        break;
    case 3:
        type = DataType::UK;
        break;
    }

    CostCalc calc;

    index = ui->comboBox_cost->currentIndex();
    switch(index)
    {
    case 0:
        calc = CostCalc::Quadratic;
        break;
    case 1:
        calc = CostCalc::CrossEntropy;
        break;
    }

    if(_activeTab == 0)
    {
        if(!_isEditing)
            emit signNetworkCreation(numInput, numHidden, numOutput, name, type, calc);
        else
            emit signNetworkEdit(_index, numInput, numHidden, numOutput, name, type, calc);
    }
    else if(_activeTab == 1)
    {

    }
}

void NetworkCreationDialog::updateHiddenLayerSection()
{
    if(_hiddenLabel.size() < _numHiddenLayers)
    {
        for(int i = _hiddenLabel.size(); i < _numHiddenLayers; i++)
        {
            QString layerNumString = QString::number(i+1);
            QString labelName = "label_"+i;
            QString labelText = "Hidden layer "+layerNumString;
            QString boxName = "spinBox_hidden_"+layerNumString;
            QString comboName = "comboBox_hidden_"+layerNumString;

            QLabel* label = new QLabel(this);
            label->setObjectName(labelName);
            label->setText(labelText);

            QSpinBox* box = new QSpinBox(this);
            box->setObjectName(boxName);
            box->setMinimum(1);

            QComboBox* cbox = new QComboBox(this);
            cbox->setObjectName(comboName);
            cbox->insertItem(0,"Sigmoid");

            ui->gridLayout_2->addWidget(label,i,0,1,1);
            ui->gridLayout_2->addWidget(box,i,1,1,1);
            ui->gridLayout_2->addWidget(cbox,i,2,1,1);

            _hiddenLabel.push_back(label);
            _hiddenLineEdit.push_back(box);
            _hiddenComboBox.push_back(cbox);
        }
    }
    else if(_hiddenLabel.size() > _numHiddenLayers)
    {
        ui->gridLayout_2->removeWidget(_hiddenLabel.back());
        ui->gridLayout_2->removeWidget(_hiddenLineEdit.back());
        ui->gridLayout_2->removeWidget(_hiddenComboBox.back());
        delete _hiddenComboBox.back();
        delete _hiddenLineEdit.back();
        delete _hiddenLabel.back();
        _hiddenComboBox.pop_back();
        _hiddenLabel.pop_back();
        _hiddenLineEdit.pop_back();
    }
    this->adjustSize();
}

void NetworkCreationDialog::updateHiddenLayerValues(std::vector<int> hidden)
{
    for(int i = 0; i < hidden.size(); i++)
    {
        QString layerNumString = QString::number(i+1);
        QString boxName = "spinBox_hidden_"+layerNumString;
        QSpinBox *box = this->findChild<QSpinBox*>(boxName);
        Q_ASSERT(box);
        box->setValue(hidden[i]);
    }
}

void NetworkCreationDialog::populateNetworkNames()
{
    if(_networkNames != nullptr)
    {
        if(_networkNames->size() > 0)
        {
            for(int i = 0; i < _networkNames->size(); i++)
            {
                QListWidgetItem *item = new QListWidgetItem(_networkNames->at(i));
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                item->setCheckState(Qt::Unchecked);
                ui->listWidget_networks->addItem(item);
            }
        }
        else
        {
            ui->listWidget_networks->addItem(QString("NO NETWORKS FOUND"));
        }
    }
    else
    {
        ui->listWidget_networks->addItem(QString("NO NETWORKS FOUND"));
    }
}

void NetworkCreationDialog::on_tabWidget_tabBarClicked(int index)
{
    _activeTab = index;
}
