#ifndef MASTERGEOMETRY_H
#define MASTERGEOMETRY_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QtXml>

#include "feature.h"
//#include "geometry.h"
#include "measurementconfig.h"
#include "statistic.h"
#include "simulationmodel.h"
#include "radius.h"
#include "direction.h"
#include "position.h"

namespace oi {

class Geometry;

class OI_CORE_EXPORT MasterGeometry : public Feature
{
    friend class MasterGeometry;
    friend class OiJob;
    Q_OBJECT

public:
    explicit MasterGeometry(QObject *parent = 0);
    MasterGeometry (const MasterGeometry &copy, QObject *parent = 0);

    MasterGeometry &operator = (const MasterGeometry &copy);

    virtual ~MasterGeometry();

    //nominals
    const QList<QPointer<Geometry> > &getNominals() const;
    bool addNominal(const QPointer<Geometry> &nominal);
    bool removeNominal(const QPointer<Geometry> &nominal);

    //actuals
    const QPointer<Geometry> &getActual() const;
    bool setActual(const QPointer<Geometry> &actual);
    bool removeActual(const QPointer<Geometry> &actual);

    //save and load
    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

    //###########################
    //reexecute the function list
    //###########################

    virtual void recalc();

    const MeasurementConfig &getMeasurementConfig() const;
    void setMeasurementConfig(const MeasurementConfig &myConfig);

    //current measurement config that is used to create readings until the user selects another one
    MeasurementConfig activeMeasurementConfig; //only for this geometry instance

signals:
    //TODO void geomNominalsChanged(const int &featureId);
    void geomMeasurementConfigChanged(const int &featureId, const QString &oldMConfig, bool oldIsSaved);

protected:
    QPointer<Geometry> myActual;
    QList<QPointer<Geometry> > myNominals;

    bool checkActualNominaltype(QPointer<Geometry>actual, QPointer<Geometry>nominal);
};

}

#endif // MASTERGEOMETRY_H
