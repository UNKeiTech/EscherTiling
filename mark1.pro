#-------------------------------------------------
#
# Project created by QtCreator 2017-10-03T18:40:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mark1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    renderarea.cpp \
    renderarea2.cpp \
    render.cpp \
    renderarea4.cpp \
    renderarea3.cpp \
    scenec.cpp

HEADERS += \
        mainwindow.h \
    renderarea.h \
    render.h \
    renderarea2.h \
    renderarea4.h \
    renderarea3.h \
    scenec.h

