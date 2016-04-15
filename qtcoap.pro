TEMPLATE=subdirs

SUBDIRS += libqtcoap tests coap-cli

coap-cli.depends = libqtcoap
tests.depends = libqtcoap

test.commands = LD_LIBRARY_PATH=$$shadowed($$PWD)/libqtcoap: make check
QMAKE_EXTRA_TARGETS += test

QT += network

QMAKE_CXXFLAGS += -Werror -std=c++11
QMAKE_LFLAGS += -std=c++11
