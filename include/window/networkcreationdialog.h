#ifndef NETWORKCREATIONDIALOG_H
#define NETWORKCREATIONDIALOG_H

#include <QDialog>

#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>

#include "include/enum/datatype.h"
#include "include/cost/cost.h"

namespace Ui {
class NetworkCreationDialog;
}

class NetworkCreationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkCreationDialog(QWidget *parent = 0);
    ~NetworkCreationDialog();

signals:
    void signNetworkCreation(int input, std::vector<int> hidden,
                             int output, QString name, DataType type, CostCalc calc);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_buttonBox_accepted();

private:
    Ui::NetworkCreationDialog *ui;

    std::vector<QSpinBox*>   _hiddenLineEdit;
    std::vector<QLabel*>     _hiddenLabel;
    std::vector<QComboBox*>  _hiddenComboBox;

    int                 _numInput;
    std::vector<int>    _numHidden;
    int                 _numOutput;
    int                 _numHiddenLayers = 1;

    void    updateHiddenLayerSection();
};

#endif // NETWORKCREATIONDIALOG_H
