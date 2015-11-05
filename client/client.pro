QT += core network

TARGET = coapclient

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../libcoap

top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

LIBS += -L../libcoap/ -lcoap

SOURCES += main.cpp \
    core.cpp

HEADERS += \
    core.h
