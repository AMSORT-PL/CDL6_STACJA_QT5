#-------------------------------------------------
#
# Project created by QtCreator 2021-10-27T10:44:08
#
#-------------------------------------------------

QT       += core gui network serialport

win32: LIBS += -lws2_32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CDL6_STACJA_QT5
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
    actionpanel.cpp \
    confighandler.cpp \
    fontmanager.cpp \
    layout1.cpp \
        main.cpp \
    mainwindow.cpp \
    phsbackend.cpp \
    phsbasemodel.cpp \
    startwindow.cpp \
    dialreasonreduct.cpp \
    popup.cpp

HEADERS += \
    actionpanel.h \
    confighandler.h \
    constants.h \
    fontmanager.h \
    layout1.h \
    layoutbase.h \
    mainwindow.h \
    phsbackend.h \
    phsbasemodel.h \
    startwindow.h \
    dialreasonreduct.h \
    popup.h

FORMS += \
    actionpanel.ui \
    layout1.ui \
    mainwindow.ui \
    startwindow.ui \
    dialreasonreduct.ui \
    popup.ui

RESOURCES += \
    zasoby.qrc

DISTFILES += \
    config.ini
