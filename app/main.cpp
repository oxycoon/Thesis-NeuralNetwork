#include "include/mainwindow.h"
#include <QApplication>

#include "../include/datasegment.h"
#include "../include/network.h"
#include <iostream>

int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/

    CSVReader reader;
    DataCollection collection(Exercise::WALKING);


    reader.readCSVFile("../res/docs/01_1_1_1477041067745.csv", 10, ",", collection);

    std::vector<int> hidden = {16,4};

    Network net = Network(7, 1, hidden, DataType::ACCELEROMETER);


    return 0;
}
