QT += core network

TARGET = coapclient

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

# if you don't want to use the lib, replace following two lines
# with 'include(../coap/coap.pri)' and add the coap/ to the include
# lines i.e. '#include "coap/coap.h"'
LIBS += -L../libqtcoap/ -lqtcoap
INCLUDEPATH += ../coap

SOURCES += main.cpp \
    core.cpp

HEADERS += \
    core.h
