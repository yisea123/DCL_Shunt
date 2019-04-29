QT += core network

CONFIG *= c++11

INCLUDEPATH += "$$PWD"
LIBPATH     += "$$PWD"
LIBS += -L"$$PWD" -lzauxdll -lzmotion

HEADERS += \
    $$PWD/zauxdll2.h
