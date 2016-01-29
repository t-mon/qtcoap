TEMPLATE=subdirs

SUBDIRS += libqtcoap tests examples

examples.depends = libqtcoap
tests.depends = libqtcoap

QT += network

QMAKE_CXXFLAGS += -Werror
CONFIG += c++11
