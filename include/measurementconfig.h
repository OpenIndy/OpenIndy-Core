#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include <QString>
#include <QtXml>

#include "types.h"
#include "util.h"

namespace oi{

enum MeasurementTypes{
    eSinglePoint_MeasurementType = 0,
    eScanTimeDependent_MeasurementType,
    eScanDistanceDependent_MeasurementType,
    eLevel_MeasurementType,
    eDirection_MeasurementType,
    eDistance_MeasurementType,
    eTemperature_MeasurementType
};

enum MeasurementModes {
    eFast_MeasurementMode = 0,
    eStandard_MeasurementMode,
    ePrecise_MeasurementMode
};

/*!
 * \brief The MeasurementConfig class
 * Contains all the configuration parameters needed to start a measurement.
 */
class OI_CORE_EXPORT MeasurementConfig
{

public:
    MeasurementConfig();

    MeasurementConfig(const MeasurementConfig &copy);

    MeasurementConfig &operator=(const MeasurementConfig &copy);

    friend bool operator==(const MeasurementConfig &left, const MeasurementConfig &right){

        return left.getName() == right.getName()
                && left.getMeasurementType() == right.getMeasurementType()
                && left.getMeasurementMode() == right.getMeasurementMode()
                && left.getMeasureTwoSides() == right.getMeasureTwoSides()
                && left.getMaxObservations() == right.getMaxObservations()
                && left.getTimeInterval() == right.getTimeInterval()
                & almostEqual(left.getDistanceInterval(), right.getDistanceInterval(), 8);
                // TODO stablepoint properties
    }

    //########################################
    //get or set measurement config attributes
    //########################################

    const QString &getName() const;
    void setName(const QString &name);

    const bool &getIsSaved() const;
    void setIsSaved(const bool &isSaved);

    bool getIsValid() const;

    const bool &getMeasureTwoSides() const;
    void setMeasureTwoSides(const bool &measureTwoSides);

    const long &getTimeInterval() const;
    void setTimeInterval(const long &interval);

    const double &getDistanceInterval() const;
    void setDistanceInterval(const double &interval);

    const QVariant getTransientData(const QString key) const;
    void setTransientData(const QString key, const QVariant value);

    void setIsStablePoint(const bool isStablePoint);
    const bool &getIsStablePoint() const;

    void setStablePointMinDistance(const double &minDistance);
    const double &getStablePointMinDistance() const;

    void setStablePointThresholdRange(const double &threshold);
    const double &getStablePointThresholdRange() const;

    void setStablePointThresholdTime(const double &threshold);
    const double &getStablePointThresholdTime() const;

    void setMeasurementMode(const MeasurementModes mode);
    const MeasurementModes getMeasurementMode() const;

    void setMeasurementType(const MeasurementTypes type);
    const MeasurementTypes getMeasurementType() const;

    const int &getMaxObservations() const;
    void setMaxObservations(const int &maxObservations);

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool applicableFor(const ElementTypes elementType, QList<FeatureTypes> functionIsApplicableFor);

private:

    //#############################
    //measurement config attributes
    //#############################

    QString name;
    bool isSaved;

    bool measureTwoSides; //true if the sensor shall measure in both faces (false if not)
    long timeInterval; //time interval in which the sensor shall measure
    double distanceInterval; //distance interval in which the sensor shall measure [mm]

    bool isStablePoint; // is stable point measurement
    double stablePointMinDistance; // [mm]
    double stablePointThresholdRange; // [mm]
    double stablePointThresholdTime; // [second]

    QMap<QString, QVariant> transientData;

    MeasurementTypes measurementType;
    MeasurementModes measurementMode;
    int maxObservations;

};

}

Q_DECLARE_METATYPE( oi::MeasurementConfig )
Q_DECLARE_METATYPE( oi::MeasurementConfig* )

#endif // MEASUREMENTCONFIG_H
