#-------------------------------------------------
#
# Project created by QtCreator 2019-01-11T20:33:31
#
#-------------------------------------------------

QT       += core gui
ICON      = Resources/SunGomokuMac.icns
RC_ICONS  = Resources/SunGomokuWin.ico


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SunGomoku
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
        main.cpp \
    xrUI/chessboard.cpp \
    xrUI/xrhall.cpp \
    xrUI/xrroom.cpp \
    xrUI/xrtemp.cpp \
    Kernel/board.c \
    Kernel/book.c \
    Kernel/pair.c \
    Kernel/search.c \
    Kernel/tree.c \
    Kernel/uiinc.c \
    Kernel/table.c

HEADERS += \
    xrUI/chessboard.h \
    xrUI/xrhall.h \
    xrUI/xrroom.h \
    xrUI/xrtemp.h \
    Kernel/board.h \
    Kernel/book.h \
    Kernel/key.h \
    Kernel/macro.h \
    Kernel/mvlist.h \
    Kernel/pair.h \
    Kernel/pattern.h \
    Kernel/search.h \
    Kernel/table.h \
    Kernel/tree.h \
    Kernel/uiinc.h

FORMS += \
    xrUI/xrhall.ui \
    xrUI/xrroom.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    opening/d11-1.lib \
    opening/d4-2.lib \
    opening/id12-1.lib \
    opening/id7-2.lib \
    opening/d10-1.lib \
    opening/d11-1.lib \
    opening/d12-1.lib \
    opening/d4-2.lib \
    opening/d5-1.lib \
    opening/d8-1.lib \
    opening/id10-2.lib \
    opening/id12-1.lib \
    opening/id4-2.lib \
    opening/id7-2.lib \
    opening/id9-1.lib \
    Opening/d10-1.lib \
    Opening/d11-1.lib \
    Opening/d12-1.lib \
    Opening/d4-2.lib \
    Opening/d5-1.lib \
    Opening/d8-1.lib \
    Opening/id10-2.lib \
    Opening/id12-1.lib \
    Opening/id4-2.lib \
    Opening/id9-1.lib \
    Resources/bigLogo.png \
    Resources/SunGomokuWin.ico \
    Resources/restartV2.icns \
    Resources/SunGomokuMac.icns \
    Resources/undoV2.icns

RESOURCES += \
    Resources/myicon.qrc

QMAKE_CXXFLAGS_RELEASE += -O3       # Release -O3

SUBDIRS += \
    SunGomoku.pro
