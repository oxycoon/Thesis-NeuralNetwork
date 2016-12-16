#-------------------------------------------------
#
# Project created by QtCreator 2016-10-17T14:59:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets datavisualization printsupport

TARGET = Thesis-NeuralNetwork
TEMPLATE = app
CONFIG += qt debug


SOURCES += app/main.cpp \
    src/window/mainwindow.cpp \
    src/csvreader.cpp \
    src/dataentry.cpp \
    src/network.cpp \
    src/neuron.cpp \
    src/datacollection.cpp \
    src/datasegment.cpp \
    src/filewriter.cpp \
    src/dataresults.cpp \
    src/qcustomplot/qcustomplot.cpp \
    src/cost/quadraticcost.cpp \
    src/cost/cost.cpp \
    src/cost/crossentropycost.cpp \
    src/window/networkcreationdialog.cpp

HEADERS  += include/csvreader.h \
    include/window/mainwindow.h \
    include/dataentry.h \
    include/network.h \
    include/neuron.h \
    include/datacollection.h \
    include/datasegment.h \
    include/filewriter.h \
    include/dataresults.h \
    include/qcustomplot/qcustomplot.h \
    include/cost/quadraticcost.h \
    include/cost/cost.h \
    include/cost/crossentropycost.h \
    include/window/networkcreationdialog.h \
    include/enum/exercise.h \
    include/enum/datatype.h \
    include/filename.h

FORMS    += res/layout/mainwindow.ui \
    res/layout/networkcreationdialog.ui

win32 {
    INCLUDEPATH += C:/boost_1_62_0_msvc
    LIBS += -LC:/boost_1_62_0_msvc/lib64-msvc-12.0 \
            -lboost_regex-vc120-mt-gd-1_62
}
