QT += core network

TARGET = observe

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -L../../libqtcoap/ -lqtcoap
INCLUDEPATH += ../../coap

SOURCES += main.cpp \
    core.cpp

HEADERS += \
    core.h
