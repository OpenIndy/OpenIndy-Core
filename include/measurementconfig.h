#ifndef MEASUREMENTCONFIG_H
#define MEASUREMENTCONFIG_H

#include <QString>
#include <QtXml>

#include "types.h"

namespace oi{

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

        if(left.getName().compare(right.getName()) == 0){
            return true;
        }
        return false;

    }

    //########################################
    //get or set measurement config attributes
    //########################################

    const QString &getName() const;
    void setName(const QString &name);

    const bool &getIsSaved() const;
    void setIsSaved(const bool &isSaved);

    bool getIsValid() const;

    const int &getCount() const;
    void setCount(const int &count);

    const int &getIterations() const;
    void setIterations(const int &iterations);

    const bool &getMeasureTwoSides() const;
    void setMeasureTwoSides(const bool &measureTwoSides);

    const bool &getTimeDependent() const;
    void setTimeDependent(const bool &timeDependent);

    const bool &getDistanceDependent() const;
    void setDistanceDependent(const bool &distanceDependent);

    const long &getTimeInterval() const;
    void setTimeInterval(const long &interval);

    const double &getDistanceInterval() const;
    void setDistanceInterval(const double &interval);

    const ReadingTypes &getTypeOfReading() const;
    void setTypeOfReading(const ReadingTypes &type);

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

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //#############################
    //measurement config attributes
    //#############################

    QString name;
    bool isSaved;

    int count; //the number of measurements that the sensor shall bring together to a single reading
    int iterations; //the number of readings the sensor shall return
    bool measureTwoSides; //true if the sensor shall measure in both faces (false if not)
    bool timeDependent; //true if the sensor shall measure in a special timeInterval (false if not)
    bool distanceDependent; //true if the sensor shall measure in a special distanceInterval (false if not)
    long timeInterval; //time interval in which the sensor shall measure
    double distanceInterval; //distance interval in which the sensor shall measure [mm]
    ReadingTypes typeOfReading; //the type of reading which the sensor shall return

    bool isStablePoint; // is stable point measurement
    double stablePointMinDistance; // [mm]
    double stablePointThresholdRange; // [mm]
    double stablePointThresholdTime; // [second]

    QMap<QString, QVariant> transientData;

};

}

Q_DECLARE_METATYPE( oi::MeasurementConfig )
Q_DECLARE_METATYPE( oi::MeasurementConfig* )

#endif // MEASUREMENTCONFIG_H
