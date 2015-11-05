TEMPLATE=subdirs

SUBDIRS += libcoap client tests

client.depends = libcoap
tests.depends = libcoap

QT+= network

QMAKE_CXXFLAGS += -Werror
CONFIG += c++11
