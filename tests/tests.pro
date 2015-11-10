TARGET = coaptests

target.path = /usr/bin
INSTALLS += target

QMAKE_CXXFLAGS += -Werror

CONFIG += testcase c++11

QT += testlib network

INCLUDEPATH += ../coap
LIBS += -L../libqtcoap/ -lqtcoap

SOURCES += \
    coaptests.cpp

HEADERS += \
    coaptests.h

