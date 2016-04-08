#ifndef PLUGIN_H
#define PLUGIN_H

#include <QPointer>
#include <QList>

#include "function.h"
#include "sensor.h"
#include "simulationmodel.h"
//#include "bundleadjustment.h"
#include "tool.h"
#include "exchangesimpleascii.h"
#include "exchangedefinedformat.h"

namespace oi{

/*!
 * \brief The Plugin class
 */
class Plugin
{

public:
    virtual ~Plugin(){}

    //##########################################################
    //create instances of all available plugin types in a plugin
    //##########################################################

    virtual QList<QPointer<Sensor> > createSensors() = 0;
    virtual QList<QPointer<Function> > createFunctions() = 0;
    //virtual QList<QPointer<BundleAdjustment> > createBundleAdjustments() = 0;
    virtual QList<QPointer<SimulationModel> > createSimulations() = 0;
    virtual QList<QPointer<Tool> > createTools() = 0;
    virtual QList<QPointer<ExchangeSimpleAscii> > createSimpleAsciiExchanges() = 0;
    virtual QList<QPointer<ExchangeDefinedFormat> > createDefinedFormatExchanges() = 0;

    //###################################################
    //create instances of a plugin type with a given name
    //###################################################

    virtual QPointer<Sensor> createSensor(const QString &name) = 0;
    virtual QPointer<Function> createFunction(const QString &name) = 0;
    //virtual QPointer<BundleAdjustment> createBundleAdjustment(const QString &name) = 0;
    virtual QPointer<SimulationModel> createSimulation(const QString &name) = 0;
    virtual QPointer<Tool> createTool(const QString &name) = 0;
    virtual QPointer<ExchangeSimpleAscii> createSimpleAsciiExchange(const QString &name) = 0;
    virtual QPointer<ExchangeDefinedFormat> createDefinedFormatExchange(const QString &name) = 0;

};

}

#ifndef STR
#define STR(x) #x
#endif
#ifndef STR2
#define STR2(x) STR(x)
#endif

#ifndef CONCAT
#define CONCAT(a, b) a ## b
#endif
#ifndef CONCAT2
#define CONCAT2(a, b) CONCAT(a, b)
#endif

#define PLUGIN_PREFIX de.openIndy.plugin.v
#define Plugin_iidd STR2(CONCAT2(PLUGIN_PREFIX, PLUGIN_INTERFACE_VERSION))

using namespace oi;
Q_DECLARE_INTERFACE(Plugin, Plugin_iidd)

#endif // PLUGIN_H
