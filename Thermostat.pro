#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T10:18:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thermostat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    device.cpp \
    devicecontroller.cpp

HEADERS  += mainwindow.h \
    device.h \
    devicecontroller.h

FORMS    += mainwindow.ui \
    device.ui

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

RESOURCES += \
    resources.qrc
