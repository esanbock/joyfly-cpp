#-------------------------------------------------
#
# Project created by QtCreator 2015-01-10T11:39:47
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = joyfly_cpp
TEMPLATE = app

CONFIG += thread

QMAKE_CXXFLAGS += -std=c++11
#QMAKE_CXXFLAGS += -std=gnu++11

mac: INCLUDEPATH += /Library/Frameworks/qwt.framework/Headers
mac: QMAKE_LFLAGS += -F/Library/Frameworks
mac: QMAKE_LFLAGS += -F/usr/local/lib
mac: LIBS += -L/usr/local/lib
mac: INCLUDEPATH += /usr/local/include
mac: QMAKE_LFLAGxcodeS += -L /usr/local/lib
mac: QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

mac: INCLUDEPATH += /usr/local/Cellar/qwt/6.1.3_4/lib/qwt.framework/Versions/6/Headers
linux: INCLUDEPATH += /usr/include/qwt

SOURCES += main.cpp\
        mainwindow.cpp \
    joystickinputer.cpp \
    simulatedchopper.cpp \
    abstractchopper.cpp \
    sdljoystick.cpp \
    attitude_indicator.cpp \
    teensychopper.cpp \
    plotwindow.cpp \
    joyflyguicontroller.cpp \
    terminalview.cpp

HEADERS  += mainwindow.h \
    joystickinputer.h \
    joystickexception.h \
    simulatedchopper.h \
    abstractchopper.h \
    sdljoystick.h \
    attitude_indicator.h \
    teensychopper.h \
    plotwindow.h \
    joyflycontroller.h \
    joyflyguicontroller.h \
    joyflyview.h \
    terminalview.h \
    timeseries.h

FORMS += mainwindow.ui \
    plotwindow.ui

mac: LIBS += -framework qwt
else: LIBS += -lqwt-qt5

LIBS += -lboost_system -lSDL2 -lserial

