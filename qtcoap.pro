TEMPLATE=subdirs

SUBDIRS += libqtcoap tests coap-cli

coap-cli.depends = libqtcoap
tests.depends = libqtcoap

QT += network

QMAKE_CXXFLAGS += -Werror -std=c++11
QMAKE_LFLAGS += -std=c++11
