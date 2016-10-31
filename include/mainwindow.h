#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "network.h"
#include "csvreader.h"

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

    std::vector<DataCollection>     _collections;
    std::vector<std::string>        _fileNames;


    void            initializeNetwork();
    void            findAllFiles();

public slots:


protected slots:
    void            onButtonPress();

private slots:


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
