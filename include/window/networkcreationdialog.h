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
    /**
     * @brief signNetworkCreation
     * @param input     Number of input neurons
     * @param hidden    Number of hidden neurons by layer
     * @param output    Number of output neurons
     * @param name      Network name
     * @param type      Network type (see datatype.h)
     * @param calc      Error calculation method (see cost.h)
     *
     *  Signal to create a completely new network.
     */
    void signNetworkCreation(int input, std::vector<int> hidden,
                             int output, QString name, DataType type, CostCalc calc);
    /**
     * @brief signNetworkEdit
     * @param input     Number of input neurons
     * @param hidden    Number of hidden neurons by layer
     * @param output    Number of output neurons
     * @param name      Network name
     * @param type      Network type (see datatype.h)
     * @param calc      Error calculation method (see cost.h)
     *
     *  Signal to edit an existing network.
     */
    void signNetworkEdit(int index, int input, std::vector<int> hidden,
                         int output, QString name, DataType type, CostCalc calc);
    /**
     * @brief signNetworkCreationFromSubs
     * @param indexes   Indexes of the networks to take as input to the new network
     * @param hidden    Number of hidden neurons by layer
     * @param output    Number of output neurons
     * @param name      Network name
     * @param calc      Error calculation method (see cost.h)
     *
     *  Signal to create a new network with subnetworks as input. Network type is always UK.
     */
    void signNetworkCreationFromSubs(std::vector<int> indexes, std::vector<int> hidden,
                                     int output, QString name, CostCalc calc);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_addLayer2_clicked();
    void on_pushButton_removeLayer2_clicked();
    void on_buttonBox_accepted();
    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::NetworkCreationDialog *ui;

    //------------------------
    // UI Elements
    //------------------------
    std::vector<QSpinBox*>   _hiddenLineEdit;
    std::vector<QLabel*>     _hiddenLabel;
    std::vector<QComboBox*>  _hiddenComboBox;

    std::vector<QSpinBox*>   _hiddenLineEdit2;
    std::vector<QLabel*>     _hiddenLabel2;
    std::vector<QComboBox*>  _hiddenComboBox2;

    std::vector<QString>*    _networkNames;

    //------------------------
    // Misc variables
    //------------------------
    int                 _numHiddenLayers = 1;
    int                 _index = -1;
    int                 _activeTab = 0;
    bool                _isEditing;

    void                    updateHiddenLayerSection();
    void                    updateHiddenLayerValues(std::vector<int> hidden);
    void                    populateNetworkNames();
    std::vector<int>        getNetworkIndexes();
};

#endif // NETWORKCREATIONDIALOG_H
