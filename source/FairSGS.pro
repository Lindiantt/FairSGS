#-------------------------------------------------
#
# Project created by QtCreator 2015-01-19T09:10:32
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FairSGS
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    general/cskill.cpp \
    general/cgeneral.cpp \
    general/init.cpp \
    ui/mainwindowserver.cpp \
    game/cgame.cpp \
    card/ccard.cpp \
    game/cplayer.cpp \
    game/cruntimeevent.cpp \
    ui/dialogpersonalsettings.cpp \
    network/cserver.cpp \
    network/croom.cpp \
    ui/mainwindowserverlist.cpp \
    network/cserverlist.cpp \
    network/cslsockethandle.cpp \
    ui/dialogslsettings.cpp \
    network/qtupnpportmapping.cpp \
    network/qtupnpportmappingsocket.cpp \
    network/cplayersocket.cpp

HEADERS  += mainwindow.h \
    general/init.h \
    general/cskill.h \
    general/cgeneral.h \
    ui/mainwindowserver.h \
    game/cgame.h \
    card/ccard.h \
    game/cplayer.h \
    game/cruntimeevent.h \
    ui/dialogpersonalsettings.h \
    network/cserver.h \
    network/croom.h \
    ui/mainwindowserverlist.h \
    network/cserverlist.h \
    network/cslsockethandle.h \
    network/define.h \
    ui/dialogslsettings.h \
    network/qtupnpportmapping.h \
    network/qtupnpportmappingsocket.h \
    network/cplayersocket.h

FORMS    += mainwindow.ui \
    ui/mainwindowserver.ui \
    ui/dialogpersonalsettings.ui \
    ui/mainwindowserverlist.ui \
    ui/dialogslsettings.ui
