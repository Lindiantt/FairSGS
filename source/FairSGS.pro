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
    ui/mainwindow/mainwindowserver.cpp \
    game/cgame.cpp \
    card/ccard.cpp \
    game/cplayer.cpp \
    ui/dialog/dialogpersonalsettings.cpp \
    network/cserver.cpp \
    network/croom.cpp \
    ui/mainwindow/mainwindowserverlist.cpp \
    network/cserverlist.cpp \
    network/cslsockethandle.cpp \
    ui/dialog/dialogslsettings.cpp \
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
    ui/mainwindow/mainwindowclient.cpp \
    ui/dialog/dialogchooseroom.cpp \
    network/cclient.cpp \
    ui/dialog/dialogconnect.cpp \
    ui/widget/widgetroom.cpp \
    ui/widget/widgetroomplayer.cpp \
    thread/workerloadsource.cpp \
    thread/cimage.cpp \
    ui/widget/widgetroombackground.cpp \
    general/cplayerskill.cpp \
    game/coperation.cpp \
    ui/widget/widgetgame.cpp \
    ui/widget/widgetgameplayer.cpp \
    ui/widget/widgetbottom.cpp \
    game/cplayerclient.cpp \
    game/cgameclient.cpp \
    ui/dialog/dialogregister.cpp \
    ui/dialog/dialogchoosegeneral.cpp \
    ui/others/buttongeneralcard.cpp \
    game/cplayerclientme.cpp \
    ui/widget/widgettimecount.cpp \
    ui/dialog/dialogselect.cpp \
    qsanguoshautil/uiUtils.cpp \
    ui/others/labelclickable.cpp \
    ui/widget/widgetzhuangbeifull.cpp \
    ui/widget/widgetzhuangbeismall.cpp \
    card/cards/ccardwuqi.cpp \
    ui/widget/widgetcard.cpp \
    thread/cicon.cpp \
    thread/ccheckpointer.cpp \
    card/cards/ccardsha.cpp \
    card/cards/ccardtao.cpp \
    card/cards/ccardjiu.cpp \
    card/cards/ccardguohecaiqiao.cpp \
    card/cards/ccardshunshouqianyang.cpp \
    card/cards/ccardwuzhongshengyou.cpp \
    card/cards/ccardjiedaosharen.cpp \
    card/cards/ccardwugufengdeng.cpp \
    card/cards/ccardtaoyuanjieyi.cpp \
    card/cards/ccardhuogong.cpp \
    card/cards/ccardnanmanruqin.cpp \
    card/cards/ccardwanjianqifa.cpp \
    card/cards/ccardjuedou.cpp \
    card/cards/ccardtiesuolianhuan.cpp \
    card/cards/ccardshandian.cpp \
    card/cards/ccardlebusishu.cpp \
    card/cards/ccardbingliangcunduan.cpp

HEADERS  += mainwindow.h \
    general/cskill.h \
    general/cgeneral.h \
    ui/mainwindow/mainwindowserver.h \
    game/cgame.h \
    card/ccard.h \
    game/cplayer.h \
    ui/dialog/dialogpersonalsettings.h \
    network/cserver.h \
    network/croom.h \
    ui/mainwindow/mainwindowserverlist.h \
    network/cserverlist.h \
    network/cslsockethandle.h \
    network/define.h \
    ui/dialog/dialogslsettings.h \
    network/qtupnpportmapping.h \
    network/qtupnpportmappingsocket.h \
    network/cplayersocket.h \
    card/ccardtype.h \
    game/cgameserver.h \
    game/cplayerserver.h \
    game/cevent.h \
    game/cinjured.h \
    pch.h \
    ui/mainwindow/mainwindowclient.h \
    ui/dialog/dialogchooseroom.h \
    network/cclient.h \
    ui/dialog/dialogconnect.h \
    ui/widget/widgetroom.h \
    ui/widget/widgetroomplayer.h \
    thread/workerloadsource.h \
    thread/cimage.h \
    ui/widget/widgetroombackground.h \
    general/cplayerskill.h \
    game/coperation.h \
    ui/widget/widgetgame.h \
    ui/widget/widgetgameplayer.h \
    ui/widget/widgetbottom.h \
    game/cplayerclient.h \
    game/cgameclient.h \
    ui/dialog/dialogregister.h \
    ui/dialog/dialogchoosegeneral.h \
    ui/others/buttongeneralcard.h \
    game/cplayerclientme.h \
    ui/widget/widgettimecount.h \
    ui/dialog/dialogselect.h \
    qsanguoshautil/uiUtils.h \
    ui/others/labelclickable.h \
    ui/widget/widgetzhuangbeifull.h \
    ui/widget/widgetzhuangbeismall.h \
    ui/widget/widgetcard.h \
    thread/cicon.h \
    thread/ccheckpointer.h

FORMS    += mainwindow.ui \
    ui/mainwindow/mainwindowserver.ui \
    ui/dialog/dialogpersonalsettings.ui \
    ui/mainwindow/mainwindowserverlist.ui \
    ui/dialog/dialogslsettings.ui \
    ui/mainwindow/mainwindowclient.ui \
    ui/dialog/dialogchooseroom.ui \
    ui/dialog/dialogconnect.ui \
    ui/widget/widgetroom.ui \
    ui/widget/widgetroomplayer.ui \
    ui/widget/widgetgame.ui \
    ui/widget/widgetgameplayer.ui \
    ui/widget/widgetbottom.ui \
    ui/dialog/dialogregister.ui \
    ui/dialog/dialogchoosegeneral.ui \
    ui/widget/widgettimecount.ui \
    ui/dialog/dialogselect.ui \
    ui/widget/widgetcard.ui
