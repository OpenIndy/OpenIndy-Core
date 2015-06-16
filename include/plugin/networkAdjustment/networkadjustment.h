#ifndef PI_NETWORKADJUSTMENT_H
#define PI_NETWORKADJUSTMENT_H

#include <QObject>
#include <QString>

#include "pluginmetadata.h"

#include "types.h"

namespace oi{

class CoordinateSystem;
class Statistic;

/*!
 * \brief The NetworkAdjustment class
 * Interface for implementing network adjustment plugins.
 */
class OI_CORE_EXPORT NetworkAdjustment : public QObject
{
    Q_OBJECT

public:
    NetworkAdjustment(QObject *parent = 0) : QObject(parent){}

    virtual ~NetworkAdjustment(){}

    QList<CoordinateSystem*> coordSys;
    Statistic *stats;

    virtual PluginMetaData* getMetaData(){ return NULL; }

    virtual void recalc(){}

};

}

#ifndef STR
#define STR(x) #x
#endif

#define NETWORKADJUSTMENT_PREFIX de.openIndy.plugin.networkAdjustment.v
#define NetworkAdjustment_iidd STR(NETWORKADJUSTMENT_PREFIX ## PLUGIN_INTERFACE_VERSION)

#endif // PI_NETWORKADJUSTMENT_H
