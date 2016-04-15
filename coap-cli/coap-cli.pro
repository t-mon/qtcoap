QT += core network

TARGET = coap-cli

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -Werror -std=c++11
QMAKE_LFLAGS += -std=c++11

include(../coap/coap.pri)

SOURCES += main.cpp \
    core.cpp

HEADERS += \
    core.h

target.path = /usr/bin
INSTALLS += target
