#ifndef SENSORCONFIGURATION_H
#define SENSORCONFIGURATION_H

#include <QPointer>
#include <QtXml>
#include <QString>
#include <QMap>
#include <QtSerialPort/QSerialPort>

#include "types.h"
#include "util.h"
#include "oivec.h"

namespace oi{

using namespace math;

class Sensor;

//##############
//helper classes
//##############

/*!
 * \brief The ConnectionConfig class
 * Save all information needed to connect a sensor
 */
class OI_CORE_EXPORT ConnectionConfig{
public:
    ConnectionConfig(){}

    //type
    ConnectionTypes typeOfConnection;

    //network
    QString ip;
    QString port;

    //serial
    QString comPort;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;

};

/*!
 * \brief The Accuracy class
 * Save information about the accurycy of a sensor
 */
class OI_CORE_EXPORT Accuracy{
public:
    Accuracy() : sigmaXyz(OiVec(3)), sigmaAzimuth(0.0), sigmaZenith(0.0), sigmaDistance(0.0),
        sigmaTemp(0.0), sigmaRX(0.0), sigmaRY(0.0), sigmaRZ(0.0){}

    //polar
    double sigmaAzimuth;
    double sigmaZenith;
    double sigmaDistance;

    //cartesian
    OiVec sigmaXyz;

    //temperature
    double sigmaTemp;

    //level
    double sigmaRX;
    double sigmaRY;
    double sigmaRZ;

    //undefined
    QMap<QString, double> sigmaUndefined;

};

//###########################
//sensor configuration itself
//###########################

/*!
 * \brief The SensorConfiguration class
 */
class OI_CORE_EXPORT SensorConfiguration
{

public:
    SensorConfiguration();

    SensorConfiguration(const SensorConfiguration &copy);

    SensorConfiguration &operator=(const SensorConfiguration &copy);

    friend bool operator==(const SensorConfiguration &left, const SensorConfiguration &right){

        if(left.getName().compare(right.getName()) == 0){
            return true;
        }
        return false;

    }

    //##########################################
    //get or set sensor configuration attributes
    //##########################################

    const QString &getName() const;
    void setName(const QString &name);

    const bool &getIsSaved() const;
    void setIsSaved(const bool &isSaved);

    bool getIsValid() const;

    const SensorTypes &getTypeOfSensor() const;
    void setTypeOfSensor(const SensorTypes &type);

    const QString &getPluginName() const;
    void setPluginName(const QString &name);

    const QString &getSensorName() const;
    void setSensorName(const QString &name);

    const Accuracy &getAccuracy() const;
    void setAccuracy(const Accuracy &accuracy);

    const ConnectionConfig &getConnectionConfig() const;
    void setConnectionConfig(const ConnectionConfig &cConfig);

    const QMap<QString, int> &getIntegerParameter() const;
    void setIntegerParameter(const QMap<QString, int> &intParams);

    const QMap<QString, double> &getDoubleParameter() const;
    void setDoubleParameter(const QMap<QString, double> &doubleParams);

    const QMap<QString, QString> &getStringParameter() const;
    void setStringParameter(const QMap<QString, QString> &stringParams);

    const QMultiMap<QString, QString> &getAvailableStringParameter() const;
    void setAvailableStringParameter(const QMap<QString, QString> &stringParams);

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //###############################
    //sensor configuration attributes
    //###############################

    QString name;
    bool isSaved;

    QString pluginName;
    QString sensorName;
    SensorTypes typeOfSensor;

    Accuracy accuracy;
    ConnectionConfig cConfig;

    //parameter selection
    QMap<QString, int> integerParameter;
    QMap<QString, double> doubleParameter;
    QMap<QString, QString> stringParameter;

    //all available string parameter (specified by the sensor)
    QMultiMap<QString, QString> availableStringParameter;

};

}

Q_DECLARE_METATYPE( oi::SensorConfiguration )
Q_DECLARE_METATYPE( oi::SensorConfiguration* )

#endif // SENSORCONFIGURATION_H
