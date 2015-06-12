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

#define NetworkAdjustment_iidd "de.openIndy.plugin.networkAdjustment.v001"

#endif // PI_NETWORKADJUSTMENT_H
