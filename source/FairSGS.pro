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
PRECOMPILED_HEADER = pch.h
DEFINES += USING_PCH

SOURCES += main.cpp\
        mainwindow.cpp \
    general/cskill.cpp \
    general/cgeneral.cpp \
    general/init.cpp \
    ui/mainwindowserver.cpp \
    game/cgame.cpp \
    card/ccard.cpp \
    game/cplayer.cpp \
    ui/dialogpersonalsettings.cpp \
    network/cserver.cpp \
    network/croom.cpp \
    ui/mainwindowserverlist.cpp \
    network/cserverlist.cpp \
    network/cslsockethandle.cpp \
    ui/dialogslsettings.cpp \
    network/qtupnpportmapping.cpp \
    network/qtupnpportmappingsocket.cpp \
    network/cplayersocket.cpp \
    card/ccardzhuangbei.cpp \
    card/ccardtype.cpp \
    game/cgameserver.cpp \
    game/cplayerserver.cpp \
    card/ctempcard.cpp \
    game/cevent.cpp \
    game/cinjured.cpp \
    ui/mainwindowclient.cpp \
    ui/dialogchooseroom.cpp \
    network/cclient.cpp \
    ui/dialogconnect.cpp \
    ui/widgetroom.cpp \
    ui/widgetroomplayer.cpp \
    thread/workerloadsource.cpp \
    thread/cimage.cpp \
    ui/widget/widgetroombackground.cpp

HEADERS  += mainwindow.h \
    general/init.h \
    general/cskill.h \
    general/cgeneral.h \
    ui/mainwindowserver.h \
    game/cgame.h \
    card/ccard.h \
    game/cplayer.h \
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
    network/cplayersocket.h \
    card/ccardtype.h \
    game/cgameserver.h \
    game/cplayerserver.h \
    game/cevent.h \
    game/cinjured.h \
    pch.h \
    ui/mainwindowclient.h \
    ui/dialogchooseroom.h \
    network/cclient.h \
    ui/dialogconnect.h \
    ui/widgetroom.h \
    ui/widgetroomplayer.h \
    thread/workerloadsource.h \
    thread/cimage.h \
    ui/widget/widgetroombackground.h

FORMS    += mainwindow.ui \
    ui/mainwindowserver.ui \
    ui/dialogpersonalsettings.ui \
    ui/mainwindowserverlist.ui \
    ui/dialogslsettings.ui \
    ui/mainwindowclient.ui \
    ui/dialogchooseroom.ui \
    ui/dialogconnect.ui \
    ui/widgetroom.ui \
    ui/widgetroomplayer.ui
