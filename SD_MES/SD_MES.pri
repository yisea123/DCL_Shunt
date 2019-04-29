QT += core network

CONFIG *= c++11

HEADERS += \
    $$PWD/global_h.h \
    $$PWD/mesaddbackfixbox.h \
    $$PWD/mesremovebadbox.h \
    $$PWD/mesreqbadinfo.h \
    $$PWD/mesreqboxisempty.h \
    $$PWD/mesnettest.h \
    $$PWD/mesinstationinterface.h \
    $$PWD/sdmesmanager.h \
    $$PWD/sd_define.h

SOURCES += \
    $$PWD/mesaddbackfixbox.cpp \
    $$PWD/mesremovebadbox.cpp \
    $$PWD/mesreqbadinfo.cpp \
    $$PWD/mesreqboxisempty.cpp \
    $$PWD/mesnettest.cpp \
    $$PWD/mesinstationinterface.cpp \
    $$PWD/sdmesmanager.cpp
