TARGET = qtcoap
TEMPLATE = lib

QMAKE_CXXFLAGS += -Werror
CONFIG += c++11

include(../coap/coap.pri)

target.path = /usr/lib
INSTALLS += target
