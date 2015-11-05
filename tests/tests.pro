TARGET = coaptests

target.path = /usr/bin
INSTALLS += target

QMAKE_CXXFLAGS += -Werror

CONFIG += testcase c++11

QT += testlib network

INCLUDEPATH += ../libcoap
LIBS += -L../libcoap/ -lcoap

SOURCES += \
    coaptests.cpp

HEADERS += \
    coaptests.h

