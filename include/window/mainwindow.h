#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QFileSystemModel>
#include <QListWidgetItem>
#include <QtDataVisualization>

#include "include/network.h"
#include "include/csvreader.h"

#include "include/enum/datatype.h"

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
    std::vector<Network*>   _networkList;
    CSVReader*              _reader;

    QString             _selectedFile;
    int                 _selectedNetwork;

    std::vector<DataCollection>     _collections;
    std::vector<QString>            _fileNames;

    QFileSystemModel*       _dirModel;
    QFileSystemModel*       _fileModel;

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
    void signRecievedNetworkEdit(const int index, const int in, const std::vector<int> hidden,
                                     const int out, const QString name, const DataType type,
                                     const CostCalc calc);
    void signRecievedFileReadComplete(const QString &message);



signals:
    /*void      signViewportChanged( const QString& name, const QRectF& size );
    void      signMousePressed( const QString& name, QMouseEvent* event );
    void      signMouseReleased( const QString& name, QMouseEvent* event );
    void      signMouseDoubleClicked( const QString& name, QMouseEvent* event );
    void      signKeyPressed( const QString& name, QKeyEvent* event );
    void      signKeyReleased( const QString& name, QKeyEvent* event );
    void      signWheelEventOccurred( const QString& name, QWheelEvent* event);
    void      signMouseMoved( const QString& name, QMouseEvent* event );*/
};

#endif // MAINWINDOW_H
