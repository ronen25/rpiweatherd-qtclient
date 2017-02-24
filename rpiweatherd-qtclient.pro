#-------------------------------------------------
#
# Project created by QtCreator 2016-06-12T17:40:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rpiweatherd-qtclient
TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp\
        src/WndMain.cpp \
    src/IndexesDisplay.cpp \
    src/MeasureDisplay.cpp \
    src/DlgConfiguration.cpp \
    src/Utils.cpp \
    src/RequestManager.cpp \
    src/ConfigurationManager.cpp \
    src/PhaseImageMap.cpp

HEADERS  += include/WndMain.h \
    include/IndexesDisplay.h \
    include/MeasureDisplay.h \
    include/DlgConfiguration.h \
    include/Utils.h \
    include/ConfigurationConstants.h \
    include/RequestManager.h \
    include/ConfigurationManager.h \
    include/PhaseImageMap.h

FORMS    += ui/WndMain.ui \
    ui/MeasureDisplay.ui \
    ui/DlgConfiguration.ui

# C++11 support
QMAKE_CXXFLAGS += -std=c++11

# Application version
VERSION = 1.0.1

# Define the preprocessor macro to get the application version in our application.
DEFINES += APPLICATION_VERSION=\\\"$$VERSION\\\"

RESOURCES += \
    Resources.qrc

DISTFILES += \
    LICENSE
