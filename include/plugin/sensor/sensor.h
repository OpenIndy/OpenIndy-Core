#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QPair>
#include <QJsonObject>

#include "pluginmetadata.h"
#include "sensorconfiguration.h"
#include "measurementconfig.h"
#include "types.h"
#include "util.h"
#include "reading.h"

namespace oi{

/*!
 * \brief The SensorAttributes class
 */
class OI_CORE_EXPORT SensorAttributes{
public:
    double moveAzimuth;
    double moveZenith;
    double moveDistance;
    bool moveIsRelative;
    double moveX;
    double moveY;
    double moveZ;
    QString action;
};

/*!
 * \brief The Sensor class
 */
class OI_CORE_EXPORT Sensor : public QObject
{
    Q_OBJECT

public:
    explicit Sensor(QObject *parent = 0);

    Sensor(const Sensor &copy, QObject *parent = 0);

    Sensor &operator=(const Sensor &copy);

    virtual ~Sensor();

    enum SensorMessage{
        eSensorOk,
        eSensorWarning,
        eSensorError
    };

    //############################
    //sensor initialization method
    //############################

    virtual void init();

    //####################################
    //get or set general sensor attributes
    //####################################

    const SensorConfiguration &getSensorConfiguration() const;
    virtual void setSensorConfiguration(const SensorConfiguration &sConfig);

    const QPair<ReadingTypes, QPointer<Reading> > &getLastReading() const;

    virtual bool isSensorAsync() const;

    //#########################################################
    //methods to get or set further information to use a sensor
    //#########################################################

    //reading types, sensor actions and connection types
    const QList<ReadingTypes> &getSupportedReadingTypes() const;
    const QList<SensorFunctions> &getSupportedSensorActions() const;
    const QList<ConnectionTypes> &getSupportedConnectionTypes() const;

    //meta data
    const PluginMetaData &getMetaData() const;

    //integer, double and string parameter
    const QMap<QString, int> &getIntegerParameter() const;
    const QMap<QString, double> &getDoubleParameter() const;
    const QMultiMap<QString, QString> &getStringParameter() const;

    //self deined actions
    const QStringList &getSelfDefinedActions() const;

    //accuracy
    const Accuracy &getDefaultAccuracy() const;

    //########################
    //sensor state and actions
    //########################

    //sensor actions
    virtual bool accept(const SensorFunctions &method, const SensorAttributes &sAttr);

    virtual QJsonObject performAsyncSensorCommand(const QJsonObject &request);

    //abort actions
    virtual bool abortAction();

    //connect or disconnect
    virtual bool connectSensor();
    virtual bool disconnectSensor();

    //measurements
    virtual QList<QPointer<Reading> > measure(const MeasurementConfig &mConfig);
    virtual QVariantMap readingStream(const ReadingTypes &streamFormat);

    //status information
    virtual bool getConnectionState();
    virtual bool getIsReadyForMeasurement();
    virtual bool getIsBusy();
    virtual QMap<QString, QString> getSensorStatus();

    //self defined actions
    virtual bool doSelfDefinedAction(const QString &action);

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

signals:

    //##############################################
    //signals to inform OpenIndy about sensor issues
    //##############################################

    void sensorMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);
    void asyncSensorResponse(const QJsonObject &response);
    void asyncMeasurementResult(const int &geomId, const QList<QPointer<Reading>> &measurements);
    void asyncSensorNotification(const QJsonObject &response);

protected:

    //#########################
    //general sensor attributes
    //#########################

    SensorConfiguration sensorConfiguration;

    QPair<ReadingTypes, QPointer<Reading> > lastReading; //the last reading produced by this sensor

    //###########################
    //input and output parameters
    //###########################

    //meta information about the sensor
    QList<ReadingTypes> supportedReadingTypes;
    QList<SensorFunctions> supportedSensorActions;
    QList<ConnectionTypes> supportedConnectionTypes;
    QStringList selfDefinedActions;
    Accuracy defaultAccuracy;
    PluginMetaData metaData;

    //the needed scalar input parameters including default values
    QMap<QString, int> integerParameters;
    QMap<QString, double> doubleParameters;
    QMultiMap<QString, QString> stringParameters;

};

}

#ifndef STR
#define STR(x) #x
#endif
#ifndef STR2
#define STR2(x) STR(x)
#endif

#ifndef CONCAT
#define CONCAT(a, b) a ## b
#endif
#ifndef CONCAT2
#define CONCAT2(a, b) CONCAT(a, b)
#endif

#define SENSOR_PREFIX de.openIndy.plugin.sensor.v
#define Sensor_iidd STR2(CONCAT2(SENSOR_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // SENSOR_H
