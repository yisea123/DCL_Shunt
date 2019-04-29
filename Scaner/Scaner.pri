QT += core network

CONFIG *= c++11

HEADERS += \
    $$PWD/scanerdevice.h \
    $$PWD/scandatamanage.h \
    $$PWD/scanermanager.h

SOURCES += \
    $$PWD/scanerdevice.cpp \
    $$PWD/scandatamanage.cpp \
    $$PWD/scanermanager.cpp
