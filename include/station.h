#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QList>
#include <QPointer>

#include "sensorcontrol.h"
#include "feature.h"
#include "point.h"

class ProjectExchanger;

namespace oi{

class Reading;
class CoordinateSystem;
class Sensor;

/*!
 * \brief The Station class
 */
class OI_CORE_EXPORT Station : public Feature
{
    friend class OiJob;
    friend class ::ProjectExchanger;
    friend class Observation;
    Q_OBJECT

public:
    explicit Station(QObject *parent = 0);
    explicit Station(const QString &name, QObject *parent = 0);

    Station(const Station &copy, QObject *parent = 0);

    Station &operator=(const Station &copy);

    ~Station();

    //#############################
    //get or set station attributes
    //#############################

    //active state
    const bool &getIsActiveStation() const;
    void setActiveStationState(const bool &isActiveStation);

    //position
    const QPointer<Point> &getPosition() const;
    void setPosition(const Position xyz);
    void setDirection(const Direction ijk);

    //coordinate system
    const QPointer<CoordinateSystem> &getCoordinateSystem() const;

    //get or set sensor configuration
    SensorConfiguration getSensorConfiguration();
    void setSensorConfiguration(const SensorConfiguration &sConfig);

    //get or set sensor
    Sensor getSensor() const;
    void setSensor(const QPointer<Sensor> &sensor);
    QPointer<Sensor> takeSensor();
    void resetSensor();

    //previously used sensors
    const QList<Sensor> getUsedSensors() const;

    //get geometries measured from this station
    QList<QPointer<Geometry> > getTargetGeometries() const;

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
    SensorTypes getActiveSensorType() const;

    //reading and connection types
    QList<ReadingTypes> getSupportedReadingTypes() const;
    QList<ConnectionTypes> getSupportedConnectionTypes() const;

    //sensor actions
    QList<SensorFunctions> getSupportedSensorActions() const;
    QStringList getSelfDefinedActions() const;

    //stream format
    ReadingTypes getStreamFormat();
    void setStreamFormat(ReadingTypes streamFormat);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryI(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryJ(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryK(const int &digits, const bool &showDiff = false) const;

signals:

    //#######################################
    //signals to inform about station changes
    //#######################################

    void activeStationChanged(const int &stationId);
    void sensorChanged(const int &featureId);

    //###############################
    //signals to start sensor actions
    //###############################

    //connect or disconnect
    void connectSensor();
    void disconnectSensor();

    //measure
    void measure(const int &geomId, const MeasurementConfig &mConfig);

    void measurementDone(bool success);

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

protected:

    //######################################
    //update feature id when the job was set
    //######################################

    void setUpFeatureId();

private:

    //##########################
    //general station attributes
    //##########################

    bool isActiveStation;

    //the position of the station
    QPointer<Point> position;
    Direction ijk;

    //sensor communication
    QPointer<SensorControl> sensorControl;

    //the corresponding coordinate system
    QPointer<CoordinateSystem> stationSystem;

    //#############################
    //readings made by this station
    //#############################

    QList<QPointer<Reading> > cartesianReadings;
    QList<QPointer<Reading> > directionReadings;
    QList<QPointer<Reading> > distanceReadings;
    QList<QPointer<Reading> > polarReadings;
    QList<QPointer<Reading> > levelReadings;
    QList<QPointer<Reading> > temperatureRadings;
    QList<QPointer<Reading> > undefinedReadings;

private slots:

    //##############
    //helper methods
    //##############

    //station attribute changes
    void stationNameChanged(const int &featureId, const QString &oldName);

    //connect or disconnect sensor control
    void connectSensorControl();
    void disconnectSensorControl();

    //add or remove readings
    void addReadings(const int &geomId, const QList<QPointer<Reading> > &readings);
    void removeReading(const QPointer<Reading> &reading);

    //set previously used sensors
    void setUsedSensors(const QList<Sensor> &sensors);

protected:

    //#############################################
    //pointer to OiJob that this feature belongs to
    //#############################################

    void setJob(const QPointer<OiJob> &job);

};

}

#endif // STATION_H
