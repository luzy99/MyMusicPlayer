#-------------------------------------------------
#
# Project created by QtCreator 2019-08-23T19:41:44
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += multimedia
QT       += multimediawidgets
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
    absframelessautosize.cpp \
    addintosonglist.cpp \
    animatedwallwg.cpp \
    animationstackedwidget.cpp \
    audiotag.cpp \
    createsonglistdialog.cpp \
    diskwidget.cpp \
    downloadwindow.cpp \
    errorwindow.cpp \
    gesturecontrol.cpp \
    informationwindow.cpp \
    littlesongbar.cpp \
    login.cpp \
    lyricdownload.cpp \
    lyricspost.cpp \
    lyricwidget.cpp \
        main.cpp \
    mainwindow.cpp \
    minilyrics.cpp \
    musicplaybar.cpp \
    mvplayer.cpp \
    mylistlistwidget.cpp \
    mylistsongwidget.cpp \
    myprogressbar.cpp \
    renamesonglistdialog.cpp \
    resultwidget.cpp \
    searchdata.cpp \
    searchlocal.cpp \
    setting.cpp \
    showlocal.cpp \
    signup.cpp \
    skinchange.cpp \
    songinfoshow.cpp \
    songlist.cpp \
    suspensionwindow.cpp \
    titlebar.cpp

HEADERS += \
    absframelessautosize.h \
    addintosonglist.h \
    animatedwallwg.h \
    animationstackedwidget.h \
    audiotag.h \
    createsonglistdialog.h \
    diskwidget.h \
    downloadwindow.h \
    errorwindow.h \
    gesturecontrol.h \
    informationwindow.h \
    littlesongbar.h \
    login.h \
    lyricdownload.h \
    lyricspost.h \
    lyricwidget.h \
    mainwindow.h \
    minilyrics.h \
    musicplaybar.h \
    mvplayer.h \
    mylistlistwidget.h \
    mylistsongwidget.h \
    myprogressbar.h \
    renamesonglistdialog.h \
    resultwidget.h \
    searchdata.h \
    searchlocal.h \
    setting.h \
    showlocal.h \
    signup.h \
    skinchange.h \
    songinfo.h \
    songinfoshow.h \
    songlist.h \
    suspensionwindow.h \
    titlebar.h

FORMS +=

RESOURCES += \
    res.qrc

#引入openCV图像处理库
INCLUDEPATH += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\include
LIBS += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_core411.dll
LIBS += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_highgui411.dll
LIBS += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_imgproc411.dll
LIBS += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_calib3d411.dll
LIBS += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_imgcodecs411.dll
LIBS += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_video411.dll
LIBS += D:\OpenCV\OpenCV-MinGW-Build-OpenCV-4.1.1-x64\x64\mingw\bin\libopencv_videoio411.dll

