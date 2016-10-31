#include "include/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    /*CSVReader r;

    r.readCSVFile("../res/docs/01_1_1_1477041067745.csv", 10, ",");

    return 0;*/
}
