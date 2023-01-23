#-------------------------------------------------
#
# Project created by QtCreator 2019-06-12T15:53:07
#
#-------------------------------------------------
CONFIG += c++11
QT       += testlib

QT       += core xml

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_reading.cpp

DEFINES += SRCDIR=$$shell_quote($$PWD)

include(../../include.pri)

include(../../build/dependencies.pri)

include(../../build/version.pri)

CONFIG(debug, debug|release) {
    BUILD_DIR=debug
} else {
    BUILD_DIR=release
}

QMAKE_EXTRA_TARGETS += run-test
run-test.commands = \
   $$shell_quote($$OUT_PWD/$$BUILD_DIR/$$TARGET) -o $$system_path(../reports/$${TARGET}.xml),xml

