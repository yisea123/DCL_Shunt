QT += core network

CONFIG *= c++11

HEADERS += \
    $$PWD/zauxdll2.h \
    $$PWD/zauxdll.h \
    $$PWD/sportcarddevice.h \
    $$PWD/sportcard_define.h \
    $$PWD/sportcardmanager.h

SOURCES += \
    $$PWD/sportcarddevice.cpp \
    $$PWD/sportcardmanager.cpp

LIBS += -L$$PWD -lzauxdll -lzmotion

INCLUDEPATH += $$PWD
LIBPATH     += $$PWD
