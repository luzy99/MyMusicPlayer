#-------------------------------------------------
#
# Project created by QtCreator 2019-08-27T13:39:28
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LyricWidget
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

CONFIG += c++11

SOURCES += \
        lyricdownload.cpp \
        main.cpp \
        lyricwidget.cpp

HEADERS += \
        lyricdownload.h \
        lyricwidget.h

FORMS += \
        lyricwidget.ui

#INCLUDEPATH  +=  D:\opencv\OpenCV-MinGW-Build-OpenCV-4.1.0\OpenCV-MinGW-Build-OpenCV-4.1.0\include


#LIBS +=D:\opencv\OpenCV-MinGW-Build-OpenCV-4.1.0\OpenCV-MinGW-Build-OpenCV-4.1.0\x86\mingw\bin\libopencv_core410.dll
#LIBS +=D:\opencv\OpenCV-MinGW-Build-OpenCV-4.1.0\OpenCV-MinGW-Build-OpenCV-4.1.0\x86\mingw\bin\libopencv_highgui410.dll
#LIBS +=D:\opencv\OpenCV-MinGW-Build-OpenCV-4.1.0\OpenCV-MinGW-Build-OpenCV-4.1.0\x86\mingw\bin\libopencv_imgcodecs410.dll
#LIBS +=D:\opencv\OpenCV-MinGW-Build-OpenCV-4.1.0\OpenCV-MinGW-Build-OpenCV-4.1.0\x86\mingw\bin\libopencv_imgproc410.dll
#LIBS +=D:\opencv\OpenCV-MinGW-Build-OpenCV-4.1.0\OpenCV-MinGW-Build-OpenCV-4.1.0\x86\mingw\bin\libopencv_features2d410.dll
#LIBS +=D:\opencv\OpenCV-MinGW-Build-OpenCV-4.1.0\OpenCV-MinGW-Build-OpenCV-4.1.0\x86\mingw\bin\libopencv_calib3d410.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
