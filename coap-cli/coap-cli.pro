QT += core network

TARGET = coap-cli

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include(../coap/coap.pri)

SOURCES += main.cpp \
    core.cpp

HEADERS += \
    core.h
