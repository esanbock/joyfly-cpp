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
mac: QMAKE_LFLAGS += -F/Library/Frameworks

mac: INCLUDEPATH += /opt/local/include
mac: QMAKE_LFLAGS += -L /opt/local/lib

mac: QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

SOURCES += main.cpp\
        mainwindow.cpp \
    c-joy-fly-controller.cpp \
    choppercontrol.cpp \
    c-joy-fly-view.cpp \
    c-joy-test.cc \
    c-terminal-view.cpp \
    qtjoyflyview.cpp \
    joystickinputer.cpp

HEADERS  += mainwindow.h \
    c-joy-fly-controller.h \
    choppercontrol.h \
    c-joy-fly-view.h \
    c-terminal-view.h \
    c-joy-test.h \
    qtjoyflyview.h \
    joystickinputer.h \
    controllerinputer.h \
    joystickexception.h

FORMS += mainwindow.ui

mac: LIBS += -framework SDL2
else:unix|win32: LIBS += -lSDL2

LIBS += -lserial
#LIBS += -/opt/local/lib/libserial.dylib
