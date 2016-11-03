#include "include/mainwindow.h"
#include <QApplication>

/*#include "../include/dataentrytotal.h"
#include <iostream>*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    /*CSVReader reader;
    DataCollection collection(Exercise::WALKING);


    reader.readCSVFile("../res/docs/01_1_1_1477041067745.csv", 10, ",", collection);

    std::vector<DataEntry*> temp;

    collection.printCollection();
    collection.getDataSegment(2, 3, temp);

    DataEntryTotal test(temp);

    std::cout << test.getTotalAccelerometer() << std::endl;
    std::cout << test.getTotalGyroscope() << std::endl;
    std::cout << test.getTotalMagnetometer() << std::endl;


    return 0;*/
}
