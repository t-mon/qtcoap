TARGET = coaptests

target.path = /usr/bin
INSTALLS += target

CONFIG += testcase

QMAKE_CXXFLAGS += -Werror -std=c++11
QMAKE_LFLAGS += -std=c++11

QT += testlib network

INCLUDEPATH += ../coap
LIBS += -L../libqtcoap/ -lqtcoap

SOURCES += \
    coaptests.cpp

HEADERS += \
    coaptests.h

