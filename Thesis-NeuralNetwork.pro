#-------------------------------------------------
#
# Project created by QtCreator 2016-10-17T14:59:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thesis-NeuralNetwork
TEMPLATE = app


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
    src/quadraticcost.cpp \
    src/cost.cpp \
    src/crossentropycost.cpp \
    src/window/networkcreationdialog.cpp

HEADERS  += include/csvreader.h \
    include/window/mainwindow.h \
    include/dataentry.h \
    include/network.h \
    include/neuron.h \
    include/exercise.h \
    include/datacollection.h \
    include/datasegment.h \
    include/filewriter.h \
    include/dataresults.h \
    include/quadraticcost.h \
    include/cost.h \
    include/crossentropycost.h \
    include/window/networkcreationdialog.h

FORMS    += res/layout/mainwindow.ui \
    res/layout/networkcreationdialog.ui

win32 {
    INCLUDEPATH += C:/boost_1_62_0_msvc
    LIBS += -LC:/boost_1_62_0_msvc/lib64-msvc-12.0 \
            -lboost_regex-vc120-mt-gd-1_62
}
