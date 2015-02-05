#-------------------------------------------------
#
# Project created by QtCreator 2015-01-10T11:39:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = joyfly_cpp
TEMPLATE = app

CONFIG += thread

QMAKE_CXXFLAGS += -std=c++11
#QMAKE_CXXFLAGS += -std=gnu++11

mac: INCLUDEPATH += /Library/Frameworks/SDL2.framework/Headers
mac: INCLUDEPATH += /Library/Frameworks/qwt.framework/Headers
mac: QMAKE_LFLAGS += -F/Library/Frameworks
mac: INCLUDEPATH += /usr/local/include
mac: QMAKE_LFLAGS += -L /usr/local/lib
mac: QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

linux: INCLUDEPATH += /usr/local/qwt-6.1.0/include

SOURCES += main.cpp\
        mainwindow.cpp \
    c-joy-fly-controller.cpp \
    choppercontrol.cpp \
    c-terminal-view.cpp \
    joystickinputer.cpp \
    simulatedchopper.cpp \
    abstractchopper.cpp \
    sdljoystick.cpp \
    attitude_indicator.cpp \
    serialstream.cpp

HEADERS  += mainwindow.h \
    c-joy-fly-controller.h \
    choppercontrol.h \
    c-joy-fly-view.h \
    c-terminal-view.h \
    joystickinputer.h \
    controllerinputer.h \
    joystickexception.h \
    simulatedchopper.h \
    abstractchopper.h \
    sdljoystick.h \
    attitude_indicator.h \
    serialstream.h

FORMS += mainwindow.ui

linux: LIBS += -L/usr/local/qwt-6.1.0/lib

mac: LIBS += -framework SDL2
else:unix|win32: LIBS += -lSDL2 -lqwt

mac: LIBS += -framework qwt

LIBS += -lboost_system
