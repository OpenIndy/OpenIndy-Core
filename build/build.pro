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

# version number from git
include($$PWD/version.pri)

# general build configuration
include($$PWD/config.pri)

# external libraries
include($$PWD/dependencies.pri)

# include paths
INCLUDEPATH += $$PWD/../src
INCLUDEPATH += $$PWD/../src/geometry
INCLUDEPATH += $$PWD/../src/plugin
INCLUDEPATH += $$PWD/../src/plugin/function
INCLUDEPATH += $$PWD/../src/plugin/sensor
INCLUDEPATH += $$PWD/../src/plugin/networkAdjustment
INCLUDEPATH += $$PWD/../src/plugin/simulation
INCLUDEPATH += $$PWD/../src/plugin/tool
INCLUDEPATH += $$PWD/../src/plugin/exchange
INCLUDEPATH += $$PWD/../src/util
INCLUDEPATH += $$PWD/../include
INCLUDEPATH += $$PWD/../include/geometry
INCLUDEPATH += $$PWD/../include/plugin
INCLUDEPATH += $$PWD/../include/plugin/function
INCLUDEPATH += $$PWD/../include/plugin/sensor
INCLUDEPATH += $$PWD/../include/plugin/networkAdjustment
INCLUDEPATH += $$PWD/../include/plugin/simulation
INCLUDEPATH += $$PWD/../include/plugin/tool
INCLUDEPATH += $$PWD/../include/plugin/exchange
INCLUDEPATH += $$PWD/../include/util

# source files
SOURCES += \
    $$PWD/../src/geometry/circle.cpp \
    $$PWD/../src/geometry/cone.cpp \
    $$PWD/../src/geometry/cylinder.cpp \
    $$PWD/../src/geometry/ellipse.cpp \
    $$PWD/../src/geometry/ellipsoid.cpp \
    $$PWD/../src/geometry/hyperboloid.cpp \
    $$PWD/../src/geometry/line.cpp \
    $$PWD/../src/geometry/nurbs.cpp \
    $$PWD/../src/geometry/paraboloid.cpp \
    $$PWD/../src/geometry/plane.cpp \
    $$PWD/../src/geometry/point.cpp \
    $$PWD/../src/geometry/pointcloud.cpp \
    $$PWD/../src/geometry/scalarentityangle.cpp \
    $$PWD/../src/geometry/scalarentitydistance.cpp \
    $$PWD/../src/geometry/scalarentitymeasurementseries.cpp \
    $$PWD/../src/geometry/scalarentitytemperature.cpp \
    $$PWD/../src/geometry/slottedhole.cpp \
    $$PWD/../src/geometry/sphere.cpp \
    $$PWD/../src/geometry/torus.cpp \
    $$PWD/../src/plugin/exchange/exchangedefinedformat.cpp \
    $$PWD/../src/plugin/exchange/exchangeinterface.cpp \
    $$PWD/../src/plugin/exchange/exchangesimpleascii.cpp \
    $$PWD/../src/plugin/function/function.cpp \
    $$PWD/../src/plugin/sensor/sensor.cpp \
    $$PWD/../src/plugin/simulation/simulationmodel.cpp \
    $$PWD/../src/plugin/tool/tool.cpp \
    $$PWD/../src/util/util.cpp \
    $$PWD/../src/coordinatesystem.cpp \
    $$PWD/../src/direction.cpp \
    $$PWD/../src/element.cpp \
    $$PWD/../src/feature.cpp \
    $$PWD/../src/featurecontainer.cpp \
    $$PWD/../src/featurewrapper.cpp \
    $$PWD/../src/geometry.cpp \
    $$PWD/../src/measurementconfig.cpp \
    $$PWD/../src/observation.cpp \
    $$PWD/../src/oijob.cpp \
    $$PWD/../src/position.cpp \
    $$PWD/../src/radius.cpp \
    $$PWD/../src/reading.cpp \
    $$PWD/../src/sensorconfiguration.cpp \
    $$PWD/../src/sensorcontrol.cpp \
    $$PWD/../src/sensorworker.cpp \
    $$PWD/../src/station.cpp \
    $$PWD/../src/statistic.cpp \
    $$PWD/../src/trafoparam.cpp \
    $$PWD/../src/plugin/networkAdjustment/bundleadjustment.cpp

# header files
HEADERS  += \
    $$PWD/../include/geometry/circle.h \
    $$PWD/../include/geometry/cone.h \
    $$PWD/../include/geometry/cylinder.h \
    $$PWD/../include/geometry/ellipse.h \
    $$PWD/../include/geometry/ellipsoid.h \
    $$PWD/../include/geometry/hyperboloid.h \
    $$PWD/../include/geometry/line.h \
    $$PWD/../include/geometry/nurbs.h \
    $$PWD/../include/geometry/paraboloid.h \
    $$PWD/../include/geometry/plane.h \
    $$PWD/../include/geometry/point.h \
    $$PWD/../include/geometry/pointcloud.h \
    $$PWD/../include/geometry/scalarentityangle.h \
    $$PWD/../include/geometry/scalarentitydistance.h \
    $$PWD/../include/geometry/scalarentitymeasurementseries.h \
    $$PWD/../include/geometry/scalarentitytemperature.h \
    $$PWD/../include/geometry/slottedhole.h \
    $$PWD/../include/geometry/sphere.h \
    $$PWD/../include/geometry/torus.h \
    $$PWD/../include/plugin/exchange/exchangedefinedformat.h \
    $$PWD/../include/plugin/exchange/exchangeinterface.h \
    $$PWD/../include/plugin/exchange/exchangesimpleascii.h \
    $$PWD/../include/plugin/plugin.h \
    $$PWD/../include/plugin/pluginmetadata.h \
    $$PWD/../include/plugin/function/constructfunction.h \
    $$PWD/../include/plugin/function/fitfunction.h \
    $$PWD/../include/plugin/function/function.h \
    $$PWD/../include/plugin/function/generatefeaturefunction.h \
    $$PWD/../include/plugin/function/geodeticfunction.h \
    $$PWD/../include/plugin/function/objecttransformation.h \
    $$PWD/../include/plugin/function/systemtransformation.h \
    $$PWD/../include/plugin/sensor/lasertracker.h \
    $$PWD/../include/plugin/sensor/sensor.h \
    $$PWD/../include/plugin/sensor/totalstation.h \
    $$PWD/../include/plugin/simulation/simulationmodel.h \
    $$PWD/../include/plugin/tool/tool.h \
    $$PWD/../include/util/types.h \
    $$PWD/../include/util/util.h \
    $$PWD/../include/coordinatesystem.h \
    $$PWD/../include/direction.h \
    $$PWD/../include/element.h \
    $$PWD/../include/feature.h \
    $$PWD/../include/featureattributes.h \
    $$PWD/../include/featurecontainer.h \
    $$PWD/../include/featurewrapper.h \
    $$PWD/../include/geometry.h \
    $$PWD/../include/measurementconfig.h \
    $$PWD/../include/observation.h \
    $$PWD/../include/oijob.h \
    $$PWD/../include/oirequestresponse.h \
    $$PWD/../include/position.h \
    $$PWD/../include/radius.h \
    $$PWD/../include/reading.h \
    $$PWD/../include/sensorconfiguration.h \
    $$PWD/../include/sensorcontrol.h \
    $$PWD/../include/sensorworker.h \
    $$PWD/../include/station.h \
    $$PWD/../include/statistic.h \
    $$PWD/../include/trafoparam.h \
    $$PWD/../include/plugin/networkAdjustment/bundleadjustment.h
