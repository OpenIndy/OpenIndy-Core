#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include <QObject>
#include <QList>
#include <QPointer>
#include <QJsonObject>

#include "feature.h"
#include "position.h"
#include "direction.h"

class TrafoController;
class ProjectExchanger;

namespace oi{

class Observation;
class TrafoParam;
class Geometry;
class Station;
class BundleAdjustment;

/*!
 * \brief The CoordinateSystem class
 */
class OI_CORE_EXPORT CoordinateSystem : public Feature
{
    friend class Station;
    friend class OiJob;
    friend class ::TrafoController;
    friend class ::ProjectExchanger;
    friend class Observation;
    Q_OBJECT

public:
    explicit CoordinateSystem(QObject *parent = 0);
    explicit CoordinateSystem(bool isBundleSystem, QObject *parent = 0);
    explicit CoordinateSystem(const QPointer<Station> &station, QObject *parent = 0);

    CoordinateSystem(const CoordinateSystem &copy, QObject *parent = 0);

    CoordinateSystem &operator=(const CoordinateSystem &copy);

    ~CoordinateSystem();

    //#######################################
    //get or set coordinate system attributes
    //#######################################

    //is active system
    const bool &getIsActiveCoordinateSystem() const;
    void setActiveCoordinateSystemState(const bool &isActiveCoordinateSystem);

    //is stations system
    const bool &getIsStationSystem() const;
    const QPointer<Station> &getStation() const;

    //is bundle system
    const bool &getIsBundleSystem() const;

    //system parameters
    const Position &getOrigin() const;
    const Direction &getXAxis() const;
    const Direction &getYAxis() const;
    const Direction &getZAxis() const;
    void setCoordinateSystem(const Position &origin, const Direction &xAxis, const Direction &yAxis, const Direction &zAxis);
    void setOrigin(const Position origin);
    void setDirection(const Direction ijk);
    const Direction &getDirection() const;

    //expansion origin
    const Position &getExpansionOrigin() const;
    void setExpansionOrigin(const Position &expansionOrigin);

    //##################################################
    //get or set observations, trafo params and nominals
    //##################################################

    //observations
    const QList<QPointer<Observation> > &getObservations() const;
    QPointer<Observation> getObservation(const int &observationId) const;
    bool addObservation(const QPointer<Observation> &observation);

    //transformation parameters
    const QList<QPointer<TrafoParam> > &getTransformationParameters() const;
    const QList<QPointer<TrafoParam> > getTransformationParameters(const QPointer<CoordinateSystem> &to) const;
    bool addTransformationParameter(const QPointer<TrafoParam> &trafoParam);
    bool removeTransformationParameter(const QPointer<TrafoParam> &trafoParam);

    //nominals
    const QList<QPointer<FeatureWrapper> > &getNominals() const;
    bool addNominal(const QPointer<FeatureWrapper> &nominal);
    bool addNominals(const QList< QPointer<FeatureWrapper> > &nominals);
    bool removeNominal(const QPointer<FeatureWrapper> &nominal);
    bool removeNominal(const int &featureId);

    //bundle template
    const QJsonObject &getBundleTemplate() const;
    void setBundleTemplate(const QJsonObject &bundleTemplate);

    //bundle adjustment
    const QPointer<BundleAdjustment> &getBundlePlugin() const;
    void setBundlePlugin(const QPointer<BundleAdjustment> &bundle);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryI(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryJ(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryK(const int &digits, const bool &showDiff = false) const;

    QString getDisplayExpansionOriginX(const UnitType &type, const int &digits) const;
    QString getDisplayExpansionOriginY(const UnitType &type, const int &digits) const;
    QString getDisplayExpansionOriginZ(const UnitType &type, const int &digits) const;
    QString getDisplayXAxisI(const int &digits) const;
    QString getDisplayXAxisJ(const int &digits) const;
    QString getDisplayXAxisK(const int &digits) const;
    QString getDisplayYAxisI(const int &digits) const;
    QString getDisplayYAxisJ(const int &digits) const;
    QString getDisplayYAxisK(const int &digits) const;
    QString getDisplayZAxisI(const int &digits) const;
    QString getDisplayZAxisJ(const int &digits) const;
    QString getDisplayZAxisK(const int &digits) const;

signals:

    //#################################################
    //signals to inform about coordinate system changes
    //#################################################

    void observationsChanged(const int &coordId, const int &obsId);
    void transformationParametersChanged(const int &coordId);
    void nominalsChanged(const int &coordId);
    void activeCoordinateSystemChanged(const int &coordId);
    void bundlePluginChanged(const int &coordId);

protected:

    //######################################
    //update feature id when the job was set
    //######################################

    void setUpFeatureId();

private slots:

    //##############
    //helper methods
    //##############

    void removeObservation(const QPointer<Observation> &obs);

private:

    //####################################
    //general coordinate system attributes
    //####################################

    //system parameters
    Position origin; //origin of the coordinate system in the current display coordinate system
    Direction ijk; // normal
    Direction xAxis; //x axis of the coordinate system in the current display coordinate system
    Direction yAxis; //y axis of the coordinate system in the current display coordinate system
    Direction zAxis; //z axis of the coordinate system in the current display coordinate system

    bool isStationSystem; //true if the coordinate system is a station system
    bool isBundleSystem; //true if the coordinate system is a bundle system
    bool isActiveCoordinateSystem;

    //represents the point from which the Part object expands with temperature
    Position expansionOrigin;

    //holds the station object (only if this is a station system)
    QPointer<Station> station;

    //the bundle plugin that is used to solve the bundle
    QJsonObject bundleTemplate;
    QPointer<BundleAdjustment> bundlePlugin;

    //#######################################
    //observations, trafo params and nominals
    //#######################################

    //observations
    QList< QPointer<Observation> > observationsList;
    QMap< int, QPointer<Observation> > observationsMap; //same observations but id as key

    //trafo params
    QList< QPointer<TrafoParam> > trafoParams;

    //nominals
    QList< QPointer<FeatureWrapper> > nominalsList;
    QMap< int, QPointer<FeatureWrapper> > nominalsMap; //same geometries but id as key
};

}

#endif // COORDINATESYSTEM_H
