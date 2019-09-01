#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:41:44
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += multimedia
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMusicPlayer
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
    audiotag.cpp \
    diskwidget.cpp \
    downloadwindow.cpp \
    lyricdownload.cpp \
    lyricwidget.cpp \
        main.cpp \
        mainwidget.cpp \
    minilyrics.cpp \
    musicplaybar.cpp \
    myprogressbar.cpp \
    songinfoshow.cpp \
    songlist.cpp \
    suspensionwindow.cpp \
    titlebar.cpp

HEADERS += \
    audiotag.h \
    diskwidget.h \
    downloadwindow.h \
    lyricdownload.h \
    lyricwidget.h \
        mainwidget.h \
    minilyrics.h \
    musicplaybar.h \
    myprogressbar.h \
    songinfo.h \
    songinfoshow.h \
    songlist.h \
    suspensionwindow.h \
    titlebar.h

FORMS += \
        mainwidget.ui

RESOURCES += \
    res.qrc
