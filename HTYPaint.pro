#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T09:09:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HTYPaint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagewidget.cpp

HEADERS  += mainwindow.h \
    imagewidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    filename.qrc
