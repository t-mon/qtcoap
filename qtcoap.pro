TEMPLATE=subdirs

SUBDIRS += libqtcoap client tests

client.depends = libqtcoap
tests.depends = libqtcoap

QT += network

QMAKE_CXXFLAGS += -Werror
CONFIG += c++11
