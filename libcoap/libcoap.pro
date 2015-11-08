TARGET = coap
TEMPLATE = lib

QMAKE_CXXFLAGS += -Werror
CONFIG += c++11

QT += network

target.path = /usr/lib
INSTALLS += target

SOURCES += \
    coap.cpp \
    coappdu.cpp \
    coapoption.cpp \
    coaprequest.cpp \
    coapreply.cpp \
    coappdublock.cpp

HEADERS += \
    coap.h \
    coappdu.h \
    coapoption.h \
    coaprequest.h \
    coapreply.h \
    coappdublock.h
