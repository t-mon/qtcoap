TEMPLATE=subdirs

SUBDIRS += libqtcoap tests coap-cli

coap-cli.depends = libqtcoap
tests.depends = libqtcoap

QT += network

QMAKE_CXXFLAGS += -Werror
CONFIG += c++11
