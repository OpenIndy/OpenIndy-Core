#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include <QObject>
#include <QPointer>
#include <QThread>
#include <QMutex>
#include <QTime>

#include "sensor.h"
#include "measurementconfig.h"
#include "sensorworker.h"

namespace oi{

typedef QMap<QString, QString> StringStringMap;

class Station;

/*!
 * \brief The SensorControl class
 * A sensor control object belongs to a station and executes all sensor actions on a seperate thread.
 */
class OI_CORE_EXPORT SensorControl : public QObject
{
    Q_OBJECT

public:
    SensorControl(QPointer<Station> &station, QObject *parent = 0);

    ~SensorControl();

    void setSensorWorkerThread(QPointer<QThread> t);
public slots:

    //####################################
    //get or set sensor control attributes
    //####################################

    //get or set sensor
    Sensor getSensor();
    void setSensor(const QPointer<Sensor> &sensor);
    QPointer<Sensor> takeSensor();
    void resetSensor();

    //previously used sensors
    const QList<Sensor> &getUsedSensors();
    void setUsedSensors(const QList<Sensor> &sensors);

    //stream format
    ReadingTypes getStreamFormat();
    void setStreamFormat(ReadingTypes streamFormat);

    //####################################################
    //get information about the currently connected sensor
    //####################################################

    //status information
    bool getIsSensorSet();
    bool getIsSensorConnected();
    bool getIsReadyForMeasurement();
    bool getIsBusy();
    QMap<QString, QString> getSensorStatus();

    //sensor type
    SensorTypes getActiveSensorType();

    //reading and connection types
    QList<ReadingTypes> getSupportedReadingTypes();
    QList<ConnectionTypes> getSupportedConnectionTypes();

    //sensor actions
    QList<SensorFunctions> getSupportedSensorActions();
    QStringList getSelfDefinedActions();

    //sensor configuration
    SensorConfiguration getSensorConfiguration();
    void setSensorConfiguration(const SensorConfiguration &sConfig);

    //####################
    //start sensor actions
    //####################

    //connect or disconnect
    void connectSensor();
    void disconnectSensor();

    //measure
    void measure(const int &geomId, const MeasurementConfig &mConfig);

    //general sensor actions
    void move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative,
              const bool &measure, const int &geomId = -1, const MeasurementConfig &mConfig = MeasurementConfig());
    void move(const double &x, const double &y, const double &z,
              const bool &measure, const int &geomId = -1, const MeasurementConfig &mConfig = MeasurementConfig());
    void initialize();
    void motorState();
    void home();
    void toggleSight();
    void compensation();
    void selfDefinedAction(const QString &action);
    void search();

    //################
    //sensor streaming
    //################

    //reading stream
    void startReadingStream();
    void stopReadingStream();

    //connection monitoring stream
    void startConnectionMonitoringStream();
    void stopConnectionMonitoringStream();

    //status monitoring stream
    void startStatusMonitoringStream();
    void stopStatusMonitoringStream();

    void finishMeasurement();
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
    void commandFinished(bool success, QString msg, SensorAction sensorAction = SensorAction::eSensorActionUndefind);
    void measurementFinished(int geomId, QList<QPointer<Reading> > readings);
    void measurementDone(bool success);

    //#######################
    //special sensor messages
    //#######################

    void sensorMessage(QString msg, MessageTypes msgType, MessageDestinations msgDest = eConsoleMessage);

private:

    //##############
    //helper methods
    //##############

    //connect or disconnect sensor worker
    void connectSensorWorker();
    void disconnectSensorWorker();

    //#################################
    //general sensor control attributes
    //#################################

    //corresponding station
    QPointer<Station> station;

    //previously used sensors
    QList<Sensor> usedSensors;

    //current sensor state
    bool sensorValid;

    //sensor worker
    QPointer<SensorWorker> worker;
};

}

#endif // SENSORCONTROL_H
