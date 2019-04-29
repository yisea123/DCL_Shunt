QT += core network

CONFIG *= c++11

INCLUDEPATH += "$$PWD"
LIBPATH     += "$$PWD"
LIBS += -L"$$PWD" -lsnap7

DEFINES += _TTY_WIN_  WINVER=0x0501 \
    _USE_32BIT_TIME_T
LIBS += -Ldll -lsetupapi -lws2_32

HEADERS += \
    $$PWD/snap7.h \
    $$PWD/dcl_plc_define.h \
    $$PWD/siemensplcdevice.h

SOURCES += \
    $$PWD/siemensplcdevice.cpp
