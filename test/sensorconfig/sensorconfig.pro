#-------------------------------------------------
#
# Project created by QtCreator 2019-06-12T15:53:07
#
#-------------------------------------------------
CONFIG += c++11
QT       += testlib

QT       += core gui widgets serialport xml

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_sensorconfig.cpp

DEFINES += SRCDIR=$$shell_quote($$PWD)

include(../../build/dependencies.pri)

include(../../build/includes.pri)


#INCLUDEPATH += \
#    $$PWD/../../include

CONFIG(debug, debug|release) {
    BUILD_DIR=debug
} else {
    BUILD_DIR=release
}

# libraries
win32: LIBS += -L$$PWD/../../bin/$$BUILD_DIR/ -lopenIndyCore1
else:linux:  LIBS += -L$$PWD/../../bin/$$BUILD_DIR/ -lopenIndyCore1

QMAKE_EXTRA_TARGETS += run-test
run-test.commands = \
   $$shell_quote($$OUT_PWD/$$BUILD_DIR/$$TARGET) -o $$system_path(../reports/$${TARGET}.xml),xml

