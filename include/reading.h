#ifndef READING_H
#define READING_H

#include <QObject>
#include <QDateTime>
#include <QtCore/qmath.h>
#include <QtXml>
#include <QPointer>

#include "element.h"
#include "measurementconfig.h"
#include "sensorconfiguration.h"
#include "oivec.h"
#include "oimat.h"
#include "types.h"
#include "util.h"

class ProjectExchanger;

namespace oi{

using namespace math;

class Observation;
class CoordinateSystem;
class Station;

//###########################
//definition of reading types
//###########################

/*!
 * \brief The ReadingPolar class
 */
class OI_CORE_EXPORT ReadingPolar{
public:
    ReadingPolar() : azimuth(0.0), zenith(0.0), distance(0.0),
        sigmaAzimuth(0.0), sigmaZenith(0.0), sigmaDistance(0.0), isValid(false){}

    double azimuth;
    double zenith;
    double distance;
    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;
    bool isValid;
};

/*!
 * \brief The ReadingDirection class
 */
class OI_CORE_EXPORT ReadingDirection{
public:
    ReadingDirection() : azimuth(0.0), zenith(0.0), sigmaAzimuth(0.0),
        sigmaZenith(0.0), isValid(false){}

    double azimuth;
    double zenith;
    double sigmaAzimuth;
    double sigmaZenith;
    bool isValid;
};

/*!
 * \brief The ReadingDistance class
 */
class OI_CORE_EXPORT ReadingDistance{
public:
    ReadingDistance() : distance(0.0), sigmaDistance(0.0), isValid(false){}

    double distance;
    double sigmaDistance;
    bool isValid;
};

/*!
 * \brief The ReadingCartesian class
 */
class OI_CORE_EXPORT ReadingCartesian{
public:
    ReadingCartesian() : xyz(3), sigmaXyz(3), isValid(false){}

    OiVec xyz;
    OiVec sigmaXyz;
    bool isValid;
};

/*!
 * \brief The ReadingTemperature class
 */
class OI_CORE_EXPORT ReadingTemperature{
public:
    ReadingTemperature() : temperature(0.0), sigmaTemperature(0.0), isValid(false){}

    double temperature;
    double sigmaTemperature;
    bool isValid;
};

/*!
 * \brief The ReadingUndefined class
 */
class OI_CORE_EXPORT ReadingUndefined{
public:
    ReadingUndefined() : isValid(false){}

    QMap<QString,double> values;
    QMap<QString,double> sigmaValues;
    bool isValid;
};

/*!
 * \brief The ReadingLevel class
 */
class OI_CORE_EXPORT ReadingLevel{
public:
    double i;
    double j;
    double k;
    double sigmaI;
    double sigmaJ;
    double sigmaK;
    bool isValid;
};

//########################
//the reading class itself
//########################

/*!
 * \brief The Reading class
 */
class OI_CORE_EXPORT Reading : public Element
{
    friend class Station;
    friend class CoordinateSystem;
    friend class Observation;
    friend class ProjectExchanger;
    Q_OBJECT

private:
    explicit Reading(QObject *parent = 0);

public:
    explicit Reading(const ReadingPolar &reading, QObject *parent = 0);
    explicit Reading(const ReadingCartesian &reading, QObject *parent = 0);
    explicit Reading(const ReadingDirection &reading, QObject *parent = 0);
    explicit Reading(const ReadingDistance &reading, QObject *parent = 0);
    explicit Reading(const ReadingTemperature &reading, QObject *parent = 0);
    explicit Reading(const ReadingLevel &reading, QObject *parent = 0);
    explicit Reading(const ReadingUndefined &reading, QObject *parent = 0);

    Reading(const Reading &copy, QObject *parent = 0);

    Reading &operator=(const Reading &copy);

    ~Reading();

    //###########################
    //get reading type and values
    //###########################

    const ReadingTypes &getTypeOfReading() const;

    const ReadingPolar &getPolarReading() const;
    const ReadingCartesian &getCartesianReading() const;
    const ReadingDirection &getDirectionReading() const;
    const ReadingDistance &getDistanceReading() const;
    const ReadingTemperature &getTemperatureReading() const;
    const ReadingLevel &getLevelReading() const;
    const ReadingUndefined &getUndefinedReading() const;

    //##############################
    //set polar reading (simulation)
    //##############################

    void setPolarReading(const ReadingPolar &rPolar);

    //################################
    //make and restore reading backups
    //################################

    void makeBackup();
    void restoreBackup();

    //############################################
    //convert between polar and cartesian readings
    //############################################

    static OiVec toCartesian(const double &azimuth, const double &zenith, const double &distance);
    static OiVec toPolar(const double &x, const double &y, const double &z);

    //#########################################
    //get general information about the reading
    //#########################################

    const MeasurementConfig &getMeasurementConfig();
    void setMeasurementConfig(const MeasurementConfig &mConfig);

    const QDateTime &getMeasuredAt() const;
    void setMeasuredAt(const QDateTime &measuredAt);

    const SensorFaces &getFace() const;
    void setSensorFace(const SensorFaces &face);

    //#################################################
    //get and set sensor configuration and observations
    //#################################################

    const SensorConfiguration &getSensorConfiguration() const;
    void setSensorConfiguration(const SensorConfiguration &sConfig);

    const QPointer<Observation> &getObservation() const;
    void setObservation(const QPointer<Observation> &observation);

    //###############
    //display methods
    //###############

    QString getDisplayId() const;
    QString getDisplayType() const;
    QString getDisplayTime() const;
    QString getDisplaySensor() const;
    QString getDisplayIsFrontside() const;
    QString getDisplayAzimuth(const UnitType &type, const int &digits) const;
    QString getDisplayZenith(const UnitType &type, const int &digits) const;
    QString getDisplayDistance(const UnitType &type, const int &digits) const;
    QString getDisplayX(const UnitType &type, const int &digits) const;
    QString getDisplayY(const UnitType &type, const int &digits) const;
    QString getDisplayZ(const UnitType &type, const int &digits) const;
    QString getDisplayI(const int &digits) const;
    QString getDisplayJ(const int &digits) const;
    QString getDisplayK(const int &digits) const;
    QString getDisplayTemperature(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaAzimuth(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaZenith(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaDistance(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaX(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaY(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaZ(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaI(const int &digits) const;
    QString getDisplaySigmaJ(const int &digits) const;
    QString getDisplaySigmaK(const int &digits) const;
    QString getDisplaySigmaTemperature(const UnitType &type, const int &digits) const;

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //#######################
    //reading type and values
    //#######################

    ReadingTypes typeOfReading;

    ReadingPolar rPolar;
    ReadingCartesian rCartesian;
    ReadingDirection rDirection;
    ReadingDistance rDistance;
    ReadingTemperature rTemperature;
    ReadingLevel rLevel;
    ReadingUndefined rUndefined;

    //###################################
    //backup readings used for simulation
    //###################################

    ReadingPolar backupPolar;
    ReadingCartesian backupCartesian;
    ReadingDirection backupDirection;
    ReadingDistance backupDistance;
    ReadingTemperature backupTemperature;
    ReadingLevel backupLevel;
    ReadingUndefined backupUndefined;

    //##########################
    //general reading attributes
    //##########################

    QDateTime measuredAt;
    MeasurementConfig mConfig; //the configuration that the sensor used to produce this reading
    SensorFaces face;

    //######################
    //sensor and observation
    //######################

    SensorConfiguration sConfig;
    QPointer<Observation> observation;

    //################
    //helper variables
    //################

    bool hasBackup; //true if a backup was made using makeBackup

private:

    //##############
    //helper methods
    //##############

    void toCartesian();
    void toPolar();

    OiVec errorPropagationPolarToCartesian();

};

}

Q_DECLARE_METATYPE( oi::Reading* )

#endif // READING_H
