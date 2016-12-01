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
    explicit NetworkCreationDialog(QWidget *parent = 0, std::vector<QString>* networkNames = nullptr);
    ~NetworkCreationDialog();

    void editNetwork(int index, int input, std::vector<int> hidden,
                     int output, QString name, DataType type, CostCalc calc);
signals:
    void signNetworkCreation(int input, std::vector<int> hidden,
                             int output, QString name, DataType type, CostCalc calc);
    void signNetworkEdit(int index, int input, std::vector<int> hidden,
                         int output, QString name, DataType type, CostCalc calc);
    void signNetworkCreationFromSubs(std::vector<int> indexes, std::vector<int> hidden,
                                     int output, QString name, CostCalc calc);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_buttonBox_accepted();

    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::NetworkCreationDialog *ui;

    std::vector<QSpinBox*>   _hiddenLineEdit;
    std::vector<QLabel*>     _hiddenLabel;
    std::vector<QComboBox*>  _hiddenComboBox;
    std::vector<QString>*    _networkNames;

    int                 _numHiddenLayers = 1;
    int                 _index = -1;
    int                 _activeTab = 0;
    bool                _isEditing;

    void    updateHiddenLayerSection();
    void    updateHiddenLayerValues(std::vector<int> hidden);
    void    populateNetworkNames();
};

#endif // NETWORKCREATIONDIALOG_H
