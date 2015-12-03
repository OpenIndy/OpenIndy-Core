#ifndef SENSORWORKER_H
#define SENSORWORKER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QMap>
#include <QVariantMap>

#include "sensor.h"

namespace oi{

/*!
 * \brief The SensorListener class
 * The sensor listener handles all sensor streams and is controlled by SensorControl
 */
class OI_CORE_EXPORT SensorWorker : public QObject
{
    Q_OBJECT

public:
    SensorWorker(QObject *parent = 0);

    ~SensorWorker();

signals:

    //##############################
    //inform about streaming results
    //##############################

    //real time data
    void realTimeReading(QVariantMap reading);
    void realTimeStatus(QMap<QString, QString> status);

    //connection information
    void connectionLost();
    void connectionReceived();

    //ready state
    void isReadyForMeasurement(bool isReady);

    //##############################################
    //signals emitted after sensor actions were done
    //##############################################

    //sensor action callbacks
    void commandFinished(bool success, QString msg);
    void measurementFinished(int geomId, QList<QPointer<Reading> > readings);

    //#######################
    //special sensor messages
    //#######################

    void sensorMessage(QString msg, MessageTypes msgType, MessageDestinations msgDest = eConsoleMessage);

public slots:

    //##############
    //(re)set sensor
    //##############

    //get or set sensor
    Sensor getSensor() const;
    void setSensor(QPointer<Sensor> sensor);
    QPointer<Sensor> takeSensor();
    void resetSensor();

    //####################################################
    //get information about the currently connected sensor
    //####################################################

    //status information
    bool getIsSensorConnected();
    bool getIsReadyForMeasurement();
    bool getIsBusy();
    QMap<QString, QString> getSensorStatus();

    //sensor type
    SensorTypes getActiveSensorType() const;

    //reading and connection types
    QList<ReadingTypes> getSupportedReadingTypes() const;
    QList<ConnectionTypes> getSupportedConnectionTypes() const;

    //sensor actions
    QList<SensorFunctions> getSupportedSensorActions() const;
    QStringList getSelfDefinedActions() const;

    //sensor configuration
    SensorConfiguration getSensorConfiguration();
    void setSensorConfiguration(SensorConfiguration sConfig);

    //####################
    //start sensor actions
    //####################

    //connect or disconnect
    void connectSensor();
    void disconnectSensor();

    //measure
    void measure(int geomId, MeasurementConfig mConfig);

    //general sensor actions
    void move(double azimuth, double zenith, double distance, bool isRelative,
              bool measure, int geomId = -1, MeasurementConfig mConfig = MeasurementConfig());
    void move(double x, double y, double z,
              bool measure, int geomId = -1, MeasurementConfig mConfig = MeasurementConfig());
    void initialize();
    void motorState();
    void home();
    void toggleSight();
    void compensation();
    void selfDefinedAction(QString action);

    //###################
    //real time streaming
    //###################

    //stream format
    ReadingTypes getStreamFormat();
    void setStreamFormat(ReadingTypes streamFormat);

    //reading stream
    void startReadingStream();
    void stopReadingStream();

    //connection monitoring stream
    void startConnectionMonitoringStream();
    void stopConnectionMonitoringStream();

    //status monitoring stream
    void startStatusMonitoringStream();
    void stopStatusMonitoringStream();

private:

    //##############
    //helper methods
    //##############

    void streamReading();
    void monitorConnectionStatus();
    void streamStatus();

    //#################
    //helper attributes
    //#################

    //current sensor
    QPointer<Sensor> sensor;

    //reading stream format
    ReadingTypes streamFormat;

    //stream states
    bool isReadingStreamStarted;
    bool isConnectionStreamStarted;
    bool isStatusStreamStarted;

    //connection status
    bool isSensorConnected;

};

}

#endif // SENSORWORKER_H
