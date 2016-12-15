#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QFileSystemModel>
#include <QListWidgetItem>
#include <QtDataVisualization>
#include <QSpinBox>
#include <QCheckBox>
#include <QThreadPool>

#include "include/network.h"
#include "include/csvreader.h"
#include "include/filename.h"

#include "include/enum/datatype.h"
#include "include/qcustomplot/qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow*         _ui;
    CSVReader*              _reader;

    //-----------------------------
    // UI Elements
    //-----------------------------
    std::vector<QComboBox*>     _comboboxCollection;
    std::vector<QCheckBox*>     _checkboxCollection;
    std::vector<QLineEdit*>     _lineeditCollection;

    FileName            _selectedFile;
    int                 _selectedNetwork;

    std::vector<int>    _idsForNetworkGraphs;
    int                 _currentGraphNetwork = 0;

    std::vector<DataCollection*>    _collections;
    std::vector<FileName>           _fileNames;
    std::vector<Network*>           _networkList;

    QFileSystemModel*       _dirModel;
    QFileSystemModel*       _fileModel;

    QThreadPool*            _pool;

    std::vector<QString>    getNetworkNames();

public slots:

protected slots:

private slots:
    void on_dirtreeview_clicked(const QModelIndex &index);
    void on_filelistview_clicked(const QModelIndex &index);
    void on_button_addFile_clicked();
    void on_pushButton_network_create_clicked();
    void on_pushButton_network_edit_clicked();
    void on_pushButton_network_delete_clicked();

    void on_listWidget_networkList_itemClicked(QListWidgetItem *item);




    void signRecievedNetworkCreation(const int in, const std::vector<int> hidden,
                                     const int out, const QString name, const DataType type,
                                     const CostCalc calc);
    void signRecievedNetworkCreationFromSubs(std::vector<int> indexes, std::vector<int> hidden,
                                             int output, QString name, CostCalc calc);
    void signRecievedNetworkEdit(const int index, const int in, const std::vector<int> hidden,
                                     const int out, const QString name, const DataType type,
                                     const CostCalc calc);
    void signRecievedNetworkTrainingComplete();
    void signRecievedFileReadComplete(const QString &message);
    void signRecievedConsoleOutput(const QString &message);
    void signRecievedEpochComplete(const int id, const int epoch, const double trainingError, const double trainingAccuracy,
                                   const double testingError, const double testingAccuracy);


    void on_checkBox_enableNoise_toggled(bool checked);
    void on_pushButton_doc_parse_clicked();

    void on_pushButton_doc_createsets_clicked();

    void on_pushButton_training_start_clicked();

    void on_pushButton_training_reset_clicked();

    void on_pushButton_clicked();

    void on_horizontalScrollBar_error_valueChanged(int value);

signals:
    /*void      signViewportChanged( const QString& name, const QRectF& size );
    void      signMousePressed( const QString& name, QMouseEvent* event );
    void      signMouseReleased( const QString& name, QMouseEvent* event );
    void      signMouseDoubleClicked( const QString& name, QMouseEvent* event );
    void      signKeyPressed( const QString& name, QKeyEvent* event );
    void      signKeyReleased( const QString& name, QKeyEvent* event );
    void      signWheelEventOccurred( const QString& name, QWheelEvent* event);
    void      signMouseMoved( const QString& name, QMouseEvent* event );*/
    void    signStopTraining();
};

#endif // MAINWINDOW_H
