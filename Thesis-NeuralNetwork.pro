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
    src/mainwindow.cpp \
    src/csvreader.cpp \
    src/dataentry.cpp \
    src/network.cpp \
    src/neuron.cpp

HEADERS  += include/mainwindow.h \
    include/csvreader.h \
    include/dataentry.h \
    include/network.h \
    include/neuron.h \
    include/exercise.h

FORMS    += res/layout/mainwindow.ui
