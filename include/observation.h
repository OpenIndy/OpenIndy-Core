#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <QObject>
#include <QList>
#include <math.h>
#include <QPointer>

#include "element.h"
#include "geometry.h"
#include "oivec.h"
#include "statistic.h"

class TrafoController;

namespace oi{

using namespace math;

class Reading;
class Station;
class CoordinateSystem;

/*!
 * \brief The Observation class
 */
class OI_CORE_EXPORT Observation : public Element
{
    friend class Reading;
    friend class CoordinateSystem;
    friend class ::TrafoController;
    Q_OBJECT

public:
    explicit Observation(QObject *parent = 0);

    Observation(const Observation &copy, QObject *parent = 0);
    Observation(const OiVec &xyz, bool isValid, QObject *parent = 0);
    Observation(const OiVec &xyz, int id, bool isValid, QObject *parent = 0);

    Observation &operator=(const Observation &copy);

    ~Observation();

    //##################################
    //get or set observations attributes
    //##################################

    const OiVec &getXYZ() const;
    void setXYZ(const OiVec &xyz);

    const OiVec &getOriginalXYZ() const;

    const OiVec &getSigmaXYZ() const;
    void setSigmaXyz(const OiVec &sigmaXyz);

    const OiVec &getOriginalSigmaXyz() const;

    const OiVec &getIJK() const;
    void setIJK(const OiVec &ijk);

    const OiVec &getOriginalIJK() const;

    const OiVec &getSigmaIJK() const;
    void setSigmaIjk(const OiVec &sigmaIjk);

    const OiVec &getOriginalSigmaIjk() const;

    //##################################################
    //get and set station, reading and target geometries
    //##################################################

    const QPointer<Station> &getStation() const;
    void setStation(const QPointer<Station> &station);

    const QList<QPointer<Geometry> > &getTargetGeometries() const;
    void addTargetGeometry(const QPointer<Geometry> &targetGeometry);
    void removeTargetGeometry(const QPointer<Geometry> &targetGeometry);
    void removeTargetGeometry(const int &geomId);
    const QPointer<Geometry> &getMeasuredTargetGeometry() const;
    void setMeasuredTargetGeometry(const QPointer<Geometry> &measuredTargetGeom);

    const QPointer<Reading> &getReading() const;

    //##########################
    //get and set state booleans
    //##########################

    const bool &getIsValid() const;

    void setIsSolved(const bool &isSolved);
    const bool &getIsSolved() const;

    const bool &getHasDirection() const;

    //###############
    //display methods
    //###############

    QString getDisplayId() const;
    QString getDisplayStation() const;
    QString getDisplayTargetGeometries() const;
    QString getDisplayX(const UnitType &type, const int &digits) const;
    QString getDisplayY(const UnitType &type, const int &digits) const;
    QString getDisplayZ(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaX(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaY(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaZ(const UnitType &type, const int &digits) const;
    QString getDisplayI(const int &digits) const;
    QString getDisplayJ(const int &digits) const;
    QString getDisplayK(const int &digits) const;
    QString getDisplaySigmaI(const int &digits) const;
    QString getDisplaySigmaJ(const int &digits) const;
    QString getDisplaySigmaK(const int &digits) const;
    QString getDisplayIsValid() const;
    QString getDisplayIsSolved() const;

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //####################################################
    //references to station, reading and target geometries
    //####################################################

    QPointer<Station> station;

    QList< QPointer<Geometry> > targetGeometriesList;
    QMap<int, QPointer<Geometry> > targetGeometriesMap;

    QPointer<Geometry> measuredTargetGeometry;

    QPointer<Reading> reading;

    //###################################################
    //booleans that indicate the state of the observation
    //###################################################

    bool isValid; //defines if the reading can be transformed, without any function, to a xyz/ijk observation
    bool isSolved; //defines wether this observation is valid in current coordinate system
    bool hasDirection; //defines if the observation has a valid ijk direction

    //#############################
    //xyz coordinates and statistic
    //#############################

    OiVec xyz; //the homogeneous coordinate vector in the current display coordinate system
    OiVec originalXyz; //the homogeneous coordinate vector in the station system that produced this observation

    OiVec sigmaXyz; //the homogeneous stdev vector in the current display coordinate system
    OiVec originalSigmaXyz; //the homogeneous stdev vector in the station system that produced this observation

    //###############################
    //ijk - unit vector and statistic
    //###############################

    OiVec ijk; // unit vector
    OiVec originalIjk; // orignal unit vector

    OiVec sigmaIjk; // stdev unit vector
    OiVec originalSigmaIjk; // orignal sigma unit vector

};

}

Q_DECLARE_METATYPE( oi::Observation* )

#endif // OBSERVATION_H
