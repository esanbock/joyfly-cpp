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

mac: INCLUDEPATH += /opt/local/include
mac: QMAKE_LFLAGS += -L /opt/local/lib

mac: QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

SOURCES += main.cpp\
        mainwindow.cpp \
    c-joy-fly-controller.cpp \
    choppercontrol.cpp \
    c-terminal-view.cpp \
    joystickinputer.cpp \
    simulatedchopper.cpp \
    abstractchopper.cpp \
    sdljoystick.cpp \
    attitude_indicator.cpp

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
    attitude_indicator.h

FORMS += mainwindow.ui

mac: LIBS += -framework SDL2
else:unix|win32: LIBS += -lSDL2

LIBS += -lserial

mac: LIBS += -framework qwt
