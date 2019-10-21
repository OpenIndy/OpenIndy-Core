#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QtXml>

#include "feature.h"
#include "measurementconfig.h"
#include "statistic.h"
#include "simulationmodel.h"
#include "radius.h"
#include "direction.h"
#include "position.h"

namespace oi{

class Observation;
class CoordinateSystem;

/*!
 * \brief The Geometry class
 *
 */
class OI_CORE_EXPORT Geometry : public Feature
{
    friend class OiJob;
    Q_OBJECT

public:
    explicit Geometry(const bool &isNominal, QObject *parent = 0);

    Geometry(const Geometry &copy, QObject *parent = 0);

    Geometry &operator=(const Geometry &copy);

    virtual ~Geometry();

    //##############################
    //get or set geometry attributes
    //##############################

    const bool &getIsCommon() const;
    void setCommonState(const bool &isCommon);

    const bool &getIsNominal() const;

    const QList<QPointer<Geometry> > &getNominals() const;
    bool addNominal(const QPointer<Geometry> &nominal);
    bool removeNominal(const QPointer<Geometry> &nominal);

    const QPointer<Geometry> &getActual() const;
    bool setActual(const QPointer<Geometry> &actual);

    const QList<QPointer<Observation> > &getObservations() const;
    void addObservation(const QPointer<Observation> &obs);
    void removeObservation(const QPointer<Observation> &obs);

    const QPointer<CoordinateSystem> &getNominalSystem() const;
    bool setNominalSystem(const QPointer<CoordinateSystem> &nomSys);
	
    const MeasurementConfig &getMeasurementConfig() const;
    void setMeasurementConfig(const MeasurementConfig &myConfig);

    const Statistic &getStatistic() const;
    void setStatistic(const Statistic &myStatistic);

    const SimulationData &getSimulationData();
    void setSimulationData(const SimulationData &s);

    const QList<ReadingTypes> &getUsedReadingTypes() const;

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    virtual bool hasDirection() const;
    virtual bool hasPosition() const;
    virtual bool hasRadius() const;

    //#############################################################
    //getter to access radius, direction and position of a geometry
    //#############################################################

    virtual const Radius &getRadius() const;
    virtual const Direction &getDirection() const;
    virtual const Position &getPosition() const;

    //#############################
    //get or set unknown parameters
    //#############################

    virtual QMap<GeometryParameters, QString> getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits,
                                                          const QMap<DimensionType, int> &displayDigits) const;
    virtual void setUnknownParameters(const QMap<GeometryParameters, double> &parameters);

    //###########################
    //reexecute the function list
    //###########################

    virtual void recalc();

    //#################
    //save and load XML
    //#################

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayStDev(const UnitType &type, const int &digits) const;

    QString getDisplayMeasurementConfig() const;
    QString getDisplayObservations() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsActual() const;

signals:

    //########################################
    //signals to inform about geometry changes
    //########################################

    void geomIsCommonChanged(const int &featureId);
    void geomNominalsChanged(const int &featureId);
    void geomActualChanged(const int &featureId);
    void geomObservationsChanged(const int &featureId);
    void geomNominalSystemChanged(const int &featureId);
    void geomStatisticChanged(const int &featureId);
    void geomSimulationDataChanged(const int &featureId);
    void geomMeasurementConfigChanged(const int &featureId, const QString &oldMConfig, bool oldIsSaved);
    void geomParametersChanged(const int &featureId);

protected:

    //###################
    //geometry attributes
    //###################

    //common
    bool isCommon;

    //nominals
    bool isNominal;
    QList< QPointer<Geometry> > nominals;

    //actual / nominal
    QPointer<Geometry> actual;
    QPointer<CoordinateSystem> nominalSystem;

    //observations
    QList<QPointer<Observation> > observations;

    //statistic
    Statistic statistic;
    SimulationData simulationData;

    //reading types
    QList<ReadingTypes> usedReadingTypes;

    //current measurement config that is used to create readings until the user selects another one
    MeasurementConfig activeMeasurementConfig; //only for this geometry instance

private:

    //############################################################################
    //helper variables used for geometries without a radius, direction or position
    //############################################################################

    Position dummyPosition;
    Direction dummyDirection;
    Radius dummyRadius;

};

}

#endif // GEOMETRY_H
