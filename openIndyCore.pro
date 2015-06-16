#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T14:25:09
#
#-------------------------------------------------

QT       += core svg serialport network xml

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = openIndyCore

TEMPLATE = lib

DEFINES += OI_CORE_LIB

#--------------------------
# get OpenIndy-Math version
#--------------------------

OpenIndyMath_VERSION = $$system(git --git-dir $$PWD/lib/OpenIndy-Math/.git --work-tree $$PWD describe --always --tags) # get git version
OpenIndyMath_VERSION = $$replace(OpenIndyMath_VERSION, "-g"{1}\w*, ) # remove commit hash after tag name
OpenIndyMath_VERSION = $$replace(OpenIndyMath_VERSION, "-", ".") # remove remaining hyphen
OpenIndyMath_VERSION = $$replace(OpenIndyMath_VERSION, "\b[0-9a-f]{5,40}\b", ) # remove commit hash (only if no tag has been set yet)

isEmpty(OpenIndyMath_VERSION){
    message("no math version")
}else{
    message(OpenIndy-Math version:)
    message($$OpenIndyMath_VERSION)
}

#--------------------------
# get OpenIndy-Core version
#--------------------------

OpenIndyCore_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags) # get git version
DEFINES += PLUGIN_INTERFACE_VERSION=\\\"$$OpenIndyCore_VERSION\\\"
OpenIndyCore_VERSION = $$replace(OpenIndyCore_VERSION, "-g"{1}\w*, ) # remove commit hash after tag name
OpenIndyCore_VERSION = $$replace(OpenIndyCore_VERSION, "-", ".") # remove remaining hyphen
OpenIndyCore_VERSION = $$replace(OpenIndyCore_VERSION, "\b[0-9a-f]{5,40}\b", ) # remove commit hash (only if no tag has been set yet)

isEmpty(OpenIndyCore_VERSION){
    message("no core version")
}else{
    message(OpenIndy-Core version:)
    message($$OpenIndyCore_VERSION)
    VERSION = $$OpenIndyCore_VERSION
}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

#-----------------------------------linux dependency---------------------------------------
linux: LIBS = -lGLU
#-----------------------------------Linear Algebra---------------------------------------

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/geometry
INCLUDEPATH += $$PWD/src/plugin
INCLUDEPATH += $$PWD/src/plugin/function
INCLUDEPATH += $$PWD/src/plugin/sensor
INCLUDEPATH += $$PWD/src/plugin/networkAdjustment
INCLUDEPATH += $$PWD/src/plugin/simulation
INCLUDEPATH += $$PWD/src/plugin/tool
INCLUDEPATH += $$PWD/src/plugin/exchange
INCLUDEPATH += $$PWD/src/util
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/geometry
INCLUDEPATH += $$PWD/include/plugin
INCLUDEPATH += $$PWD/include/plugin/function
INCLUDEPATH += $$PWD/include/plugin/sensor
INCLUDEPATH += $$PWD/include/plugin/networkAdjustment
INCLUDEPATH += $$PWD/include/plugin/simulation
INCLUDEPATH += $$PWD/include/plugin/tool
INCLUDEPATH += $$PWD/include/plugin/exchange
INCLUDEPATH += $$PWD/include/util

INCLUDEPATH += $$PWD/lib/OpenIndy-Math/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/release/ -lopenIndyMath1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/debug/ -lopenIndyMath1
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/debug -lopenIndyMath1
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/release -lopenIndyMath1

INCLUDEPATH += $$PWD/lib/OpenIndy-Math/bin/debug
DEPENDPATH += $$PWD/lib/OpenIndy-Math/bin/debug

INCLUDEPATH += $$PWD/lib/OpenIndy-Math/bin/release
DEPENDPATH += $$PWD/lib/OpenIndy-Math/bin/release

SOURCES += \
    src/geometry/circle.cpp \
    src/geometry/cone.cpp \
    src/geometry/cylinder.cpp \
    src/geometry/ellipse.cpp \
    src/geometry/ellipsoid.cpp \
    src/geometry/hyperboloid.cpp \
    src/geometry/line.cpp \
    src/geometry/nurbs.cpp \
    src/geometry/paraboloid.cpp \
    src/geometry/plane.cpp \
    src/geometry/point.cpp \
    src/geometry/pointcloud.cpp \
    src/geometry/scalarentityangle.cpp \
    src/geometry/scalarentitydistance.cpp \
    src/geometry/scalarentitymeasurementseries.cpp \
    src/geometry/scalarentitytemperature.cpp \
    src/geometry/slottedhole.cpp \
    src/geometry/sphere.cpp \
    src/geometry/torus.cpp \
    src/plugin/exchange/exchangedefinedformat.cpp \
    src/plugin/exchange/exchangeinterface.cpp \
    src/plugin/exchange/exchangesimpleascii.cpp \
    src/plugin/function/function.cpp \
    src/plugin/sensor/sensor.cpp \
    src/plugin/simulation/simulationmodel.cpp \
    src/plugin/tool/tool.cpp \
    src/util/util.cpp \
    src/coordinatesystem.cpp \
    src/direction.cpp \
    src/element.cpp \
    src/feature.cpp \
    src/featurecontainer.cpp \
    src/featurewrapper.cpp \
    src/geometry.cpp \
    src/measurementconfig.cpp \
    src/observation.cpp \
    src/oijob.cpp \
    src/position.cpp \
    src/radius.cpp \
    src/reading.cpp \
    src/sensorconfiguration.cpp \
    src/sensorcontrol.cpp \
    src/sensorlistener.cpp \
    src/station.cpp \
    src/statistic.cpp \
    src/trafoparam.cpp

HEADERS  += \
    include/geometry/circle.h \
    include/geometry/cone.h \
    include/geometry/cylinder.h \
    include/geometry/ellipse.h \
    include/geometry/ellipsoid.h \
    include/geometry/hyperboloid.h \
    include/geometry/line.h \
    include/geometry/nurbs.h \
    include/geometry/paraboloid.h \
    include/geometry/plane.h \
    include/geometry/point.h \
    include/geometry/pointcloud.h \
    include/geometry/scalarentityangle.h \
    include/geometry/scalarentitydistance.h \
    include/geometry/scalarentitymeasurementseries.h \
    include/geometry/scalarentitytemperature.h \
    include/geometry/slottedhole.h \
    include/geometry/sphere.h \
    include/geometry/torus.h \
    include/plugin/exchange/exchangedefinedformat.h \
    include/plugin/exchange/exchangeinterface.h \
    include/plugin/exchange/exchangesimpleascii.h \
    include/plugin/plugin.h \
    include/plugin/pluginmetadata.h \
    include/plugin/function/constructfunction.h \
    include/plugin/function/fitfunction.h \
    include/plugin/function/function.h \
    include/plugin/function/generatefeaturefunction.h \
    include/plugin/function/geodeticfunction.h \
    include/plugin/function/objecttransformation.h \
    include/plugin/function/systemtransformation.h \
    include/plugin/networkAdjustment/networkadjustment.h \
    include/plugin/sensor/lasertracker.h \
    include/plugin/sensor/sensor.h \
    include/plugin/sensor/totalstation.h \
    include/plugin/simulation/simulationmodel.h \
    include/plugin/tool/tool.h \
    include/util/types.h \
    include/util/util.h \
    include/coordinatesystem.h \
    include/direction.h \
    include/element.h \
    include/feature.h \
    include/featureattributes.h \
    include/featurecontainer.h \
    include/featurewrapper.h \
    include/geometry.h \
    include/measurementconfig.h \
    include/observation.h \
    include/oijob.h \
    include/oirequestresponse.h \
    include/position.h \
    include/radius.h \
    include/reading.h \
    include/sensorconfiguration.h \
    include/sensorcontrol.h \
    include/sensorlistener.h \
    include/station.h \
    include/statistic.h \
    include/trafoparam.h \
    lib/OpenIndy-Math/include/chooselalib.h \
    lib/OpenIndy-Math/include/global.h \
    lib/OpenIndy-Math/include/linearalgebra.h \
    lib/OpenIndy-Math/include/oimat.h \
    lib/OpenIndy-Math/include/oivec.h
