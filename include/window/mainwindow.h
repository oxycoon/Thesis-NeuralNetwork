#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QFileSystemModel>

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
    Ui::MainWindow*     _ui;
    Network*            _network;
    CSVReader*          _reader;

    QString             _selectedFile;

    std::vector<DataCollection>     _collections;
    std::vector<QString>            _fileNames;

    QFileSystemModel*       _dirModel;
    QFileSystemModel*       _fileModel;


    void            initializeNetwork();

public slots:


protected slots:
    void            onButtonPress();

private slots:


    void on_dirtreeview_clicked(const QModelIndex &index);
    void on_filelistview_clicked(const QModelIndex &index);
    void on_button_addFile_clicked();
    void on_pushButton_clicked();

    void signNetworkCreationRecieved(const int in, const std::vector<int> hidden,
                                     const int out, const QString name, const DataType type,
                                     const CostCalc calc);

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
