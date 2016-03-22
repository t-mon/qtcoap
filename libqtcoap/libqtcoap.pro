TARGET = qtcoap
TEMPLATE = lib

QMAKE_CXXFLAGS += -Werror -std=c++11
QMAKE_LFLAGS += -std=c++11

include(../coap/coap.pri)

target.path = /usr/lib
INSTALLS += target
