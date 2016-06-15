#-------------------------------------------------
#
# Project created by QtCreator 2016-06-12T17:40:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rpiweatherd-qtclient
TEMPLATE = app


SOURCES += main.cpp\
        WndMain.cpp \
    IndexesDisplay.cpp \
    MeasureDisplay.cpp \
    DlgConfiguration.cpp \
    Utils.cpp \
    RequestManager.cpp

HEADERS  += WndMain.h \
    IndexesDisplay.h \
    MeasureDisplay.h \
    DlgConfiguration.h \
    Utils.h \
    ConfigurationConstants.h \
    RequestManager.h

FORMS    += WndMain.ui \
    MeasureDisplay.ui \
    DlgConfiguration.ui

# C++11 support
QMAKE_CXXFLAGS += -std=c++11

# Application version
VERSION = 1.0.0

# Define the preprocessor macro to get the application version in our application.
DEFINES += APPLICATION_VERSION=\\\"$$VERSION\\\"
