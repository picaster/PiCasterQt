#-------------------------------------------------
#
# Project created by QtCreator 2019-05-13T21:36:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PiCasterQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
win32: DEFINES += TAGLIB_STATIC

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        jack/jackclient.cpp \
        jack/jackfadermodule.cpp \
        jack/jackmodule.cpp \
        jack/jackport.cpp \
        jack/jacktrackplayer.cpp \
        main.cpp \
        mainwindow.cpp \
        playlistitem.cpp \
        playlistitemmodel.cpp \
        ui/exitbutton.cpp \
        ui/jackbutton.cpp \
        ui/micbutton.cpp \
        ui/micdial.cpp \
        ui/playbutton.cpp \
        ui/stopbutton.cpp \
        ui/tracklistview.cpp \
        utils/mediaplayer.cpp \
        utils/signalbus.cpp

RESOURCES += \
        resources/picaster.astylerc \
        CONTRIBUTORS.txt

HEADERS += \
        jack/jackclient.h \
        jack/jackfadermodule.h \
        jack/jackmodule.h \
        jack/jackport.h \
        jack/jacktrackplayer.h \
        mainwindow.h \
        playlistitem.h \
        playlistitemmodel.h \
        ui/exitbutton.h \
        ui/jackbutton.h \
        ui/micbutton.h \
        ui/micdial.h \
        ui/playbutton.h \
        ui/stopbutton.h \
        ui/tracklistview.h \
        utils/mediaplayer.h \
        utils/signalbus.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += taglib alsa jack libavformat libavcodec libavresample libavutil

win32: LIBS += -L"D:\usr\local\lib" -ltag
win32: INCLUDEPATH += "D:\usr\local\include\taglib"

DISTFILES += \
    CONTRIBUTORS.txt \
    resources/picaster.astylerc
