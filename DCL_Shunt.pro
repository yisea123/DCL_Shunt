#-------------------------------------------------
#
# Project created by QtCreator 2019-04-19T17:32:56
#
#-------------------------------------------------

QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DCL_Shunt
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

include( $$PWD/Scaner/Scaner.pri )
include( $$PWD/SiemensPLC/SiemensPLC.pri )
include( $$PWD/SD_MES/SD_MES.pri )
include( $$PWD/SportsCard/SportsCard.pri )

PRECOMPILED_HEADER = stable.h


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    maincontrol.cpp \
    UI/deviceip.cpp \
    UI/mesdebugview.cpp \
    UI/plcdebugview.cpp \
    UI/scanerdebugview.cpp \
    UI/sportcarddebugview.cpp \
    ConfigFile/datafile.cpp \
    UI/logview.cpp \
    UI/logviewinstancegetter.cpp

HEADERS += \
        mainwindow.h \
    maincontrol.h \
    dcl_shunt_define.h \
    UI/deviceip.h \
    UI/mesdebugview.h \
    UI/plcdebugview.h \
    UI/scanerdebugview.h \
    UI/sportcarddebugview.h \
    ConfigFile/datafile.h \
    UI/logview.h \
    UI/logviewinstancegetter.h \
    UI/ui_define.h


FORMS += \
        mainwindow.ui \
    UI/deviceip.ui \
    UI/mesdebugview.ui \
    UI/plcdebugview.ui \
    UI/scanerdebugview.ui \
    UI/sportcarddebugview.ui


