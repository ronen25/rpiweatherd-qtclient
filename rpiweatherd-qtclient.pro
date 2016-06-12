#-------------------------------------------------
#
# Project created by QtCreator 2016-06-12T17:40:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rpiweatherd-qtclient
TEMPLATE = app


SOURCES += main.cpp\
        WndMain.cpp \
    IndexesDisplay.cpp \
    IndexesUtils.cpp

HEADERS  += WndMain.h \
    IndexesDisplay.h \
    IndexesUtils.h

FORMS    += WndMain.ui

QMAKE_CXXFLAGS += -std=c++11
