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
    reader.readCSVFile("../res/docs/02_1_1_1477045014681.csv", 10, ",", collection);
    reader.readCSVFile("../res/docs/03_1_1_1477045972595.csv", 10, ",", collection);
    reader.readCSVFile("../res/docs/04_1_1_1477047900980.csv", 10, ",", collection);
    /*reader.readCSVFile("../res/docs/05_1_1_1477049323104.csv", 10, ",", collection);
    reader.readCSVFile("../res/docs/06_1_1_1477050706950.csv", 10, ",", collection);
    reader.readCSVFile("../res/docs/07_1_1_1477051538022.csv", 10, ",", collection);
    reader.readCSVFile("../res/docs/08_1_1_1477052157282.csv", 10, ",", collection);
    reader.readCSVFile("../res/docs/09_1_1_1477052835575.csv", 10, ",", collection);
    reader.readCSVFile("../res/docs/10_1_1_1477053563351.csv", 10, ",", collection);*/

    std::vector<int> hidden = {24,7};
    collection.createTrainingTestValidationSets(5, 0.1, 0.6);

    Network net = Network(16, 1, hidden, DataType::ACCELEROMETER);

    net.runTraining(collection);

    return 0;
}
