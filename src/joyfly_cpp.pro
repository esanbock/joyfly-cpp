#-------------------------------------------------
#
# Project created by QtCreator 2015-01-10T11:39:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = joyfly_cpp
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

mac: INCLUDEPATH += /Library/Frameworks/SDL2.framework/Headers
mac: INCLUDEPATH += ../../libserial-0.5.2/lib/include

SOURCES += main.cpp\
        mainwindow.cpp \
    c-joy-fly-controller.cpp \
    choppercontrol.cpp \
    c-joy-fly-view.cpp \
    c-joy-test.cc \
    c-terminal-view.cpp

HEADERS  += mainwindow.h \
    c-joy-fly-controller.h \
    choppercontrol.h \
    c-joy-fly-view.h \
    c-terminal-view.h \
    c-joy-test.h

FORMS += mainwindow.ui

mac: QMAKE_LFLAGS += -F/Library/Frameworks
mac: QMAKE_LFLAGS += -L../../libserial-0.5.2/lib/lib

LIBS += -lserial

mac: LIBS += -framework SDL2
else:unix|win32: LIBS += -lSDL2


