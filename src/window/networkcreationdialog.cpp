#include "../include/window/networkcreationdialog.h"
#include "ui_networkcreationdialog.h"

NetworkCreationDialog::NetworkCreationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkCreationDialog)
{
    ui->setupUi(this);
    updateHiddenLayerSection();
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
    _numInput = ui->spinBox_input->value();
    _numOutput = ui->spinBox_output->value();

    for(int i = 0; i < _numHiddenLayers; i++)
    {
        _numHidden.push_back(_hiddenLineEdit[i]->value());
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

    emit signNetworkCreation(_numInput, _numHidden, _numOutput, name, type, calc);
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
