#ifndef SENSORFACADE_H
#define SENSORFACADE_H

#include <QObject>
#include <QJsonObject>
#include "sensor.h"


namespace oi{

class OI_CORE_EXPORT SensorFacade : public Sensor
{

    Q_OBJECT

public:
    SensorFacade(SensorInterface *sensor, QObject *parent = 0);
    // explicit SensorFacade(const SensorFacade &copy, QObject *parent = 0);
    SensorFacade &operator=(const SensorFacade &copy);
    ~SensorFacade();


    //############################
    //sensor initialization method
    //############################

    void init();

    //####################################
    //get or set general sensor attributes
    //####################################

    const SensorConfiguration &getSensorConfiguration() const;
    void setSensorConfiguration(const SensorConfiguration &sConfig);

    void setMeasurementConfig(const MeasurementConfig &mConfig);

    const QPair<ReadingTypes, QPointer<Reading> > &getLastReading() const;

    bool isSensorAsync() const;

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
    bool accept(const SensorFunctions &method, const SensorAttributes &sAttr);

    QJsonObject performAsyncSensorCommand(const QJsonObject &request);

    //abort actions
    bool abortAction();

    //connect or disconnect
    bool connectSensor();
    bool disconnectSensor();

    //measurements
    QList<QPointer<Reading> > measure(const MeasurementConfig &mConfig);
    QVariantMap readingStream(const ReadingTypes &streamFormat);

    //status information
    bool getConnectionState();
    bool getIsReadyForMeasurement();
    bool getIsBusy();
    QMap<QString, QString> getSensorStatus();

    //self defined actions
    bool doSelfDefinedAction(const QString &action);

    bool search();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);


private:
    SensorInterface *inner;
};
}
#endif // SENSORFACADE_H
