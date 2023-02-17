#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include <QString>
#include <QtXml>

#include "types.h"
#include "util.h"

namespace oi{

enum MeasurementTypes{
    eUnknown_MeasurementType = -1,
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

enum ConfigTypes {
    eUndefinded = -1,
    eUserConfig = 0,
    eProjectConfig,
    eStandardConfig
};

class Key {
public:
    Key(): configType(eUndefinded) {}
    Key(QString name, ConfigTypes configType) : name(name), configType(configType){}
    Key(const Key &copy) {
        this->name = copy.name;
        this->configType = copy.configType;
    }
    Key &operator=(const Key &copy) {
        this->name = copy.name;
        this->configType = copy.configType;
        return *this;
    }

    bool operator==(const Key &other) const {
        return this->name == other.name
                && this->configType == other.configType;
    }
    bool operator<(const Key &other) const {
        if (this->name < other.name) {
            return true;
        }
        return this->configType < other.configType;
    }

private:
    QString name;
    ConfigTypes configType;
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

        return left.getName() == right.getName()        // combined key
                && left.configType == right.configType  // combined key
                && left.editable == right.editable
                && left.getMeasurementType() == right.getMeasurementType()
                && left.getMeasurementMode() == right.getMeasurementMode()
                && left.getMeasureTwoSides() == right.getMeasureTwoSides()
                && left.getMaxObservations() == right.getMaxObservations()
                && left.getTimeInterval() == right.getTimeInterval()
                && almostEqual(left.getDistanceInterval(), right.getDistanceInterval(), 8);
                // TODO stablepoint properties
    }

    //########################################
    //get or set measurement config attributes
    //########################################

    const QString &getName() const;
    void setName(const QString &name);
    const Key getKey() const;

    const bool isUserConfig() const;
    const bool isProjectConfig() const;
    const bool isEditable() const;
    const bool isStandardConfig() const;
    const bool isValid() const;

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

    void makeUserConfig();
    void makeStandardConfig();
    void makeProjectConfig();


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
    ConfigTypes configType;

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

    bool editable;
};

}

Q_DECLARE_METATYPE( oi::MeasurementConfig )
Q_DECLARE_METATYPE( oi::MeasurementConfig* )

#endif // MEASUREMENTCONFIG_H
