#include "station.h"

#include "coordinatesystem.h"
#include "sensorcontrol.h"
#include "reading.h"
#include "sensor.h"
#include "oijob.h"
#include "featurewrapper.h"

using namespace oi;

/*!
 * \brief Station::Station
 * \param parent
 */
Station::Station(QObject *parent) : Feature(parent), isActiveStation(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //create a point object as the station's position
    this->position = new Point(false);

    //create a coordinate system object as the station's coordinate system
    this->stationSystem = new CoordinateSystem(QPointer<Station>(this));
    QObject::connect(this, SIGNAL(featureNameChanged(const int&, const QString&)), this, SLOT(stationNameChanged(const int&, const QString&)), Qt::DirectConnection);

    //create a sensor control object and connect it
    QPointer<Station> stationPointer(this);
    this->sensorControl = new SensorControl(stationPointer);
    this->connectSensorControl();

}

/*!
 * \brief Station::Station
 * \param name
 * \param parent
 */
Station::Station(const QString &name, QObject *parent) : Feature(parent), isActiveStation(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    this->name = name;

    //create a point object as the station's position
    this->position = new Point(false);
    this->position->setFeatureName(name);

    //create a coordinate system object as the station's coordinate system
    this->stationSystem = new CoordinateSystem(QPointer<Station>(this));
    this->stationSystem->setFeatureName(this->name);
    QObject::connect(this, SIGNAL(featureNameChanged(const int&, const QString&)), this, SLOT(stationNameChanged(const int&, const QString&)), Qt::DirectConnection);

    //create a sensor control object and connect it
    QPointer<Station> stationPointer(this);
    this->sensorControl = new SensorControl(stationPointer);
    this->connectSensorControl();

}

/*!
 * \brief Station::Station
 * \param copy
 * \param parent
 */
Station::Station(const Station &copy, QObject *parent) : Feature(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //copy general station attributes
    this->isActiveStation = copy.isActiveStation;
    if(!copy.position.isNull()){
        this->position = new Point(*copy.position.data());
    }

}

/*!
 * \brief Station::operator =
 * \param copy
 * \return
 */
Station &Station::operator=(const Station &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //copy general station attributes
    this->isActiveStation = copy.isActiveStation;
    if(!copy.position.isNull()){
        this->position = new Point(*copy.position.data());
    }

    return *this;

}

/*!
 * \brief Station::~Station
 */
Station::~Station(){

    //delete corresponding coordinate system with all observations made from this station
    if(!this->stationSystem.isNull()){
        delete this->stationSystem.data();
    }

    //delete position of this station
    if(!this->position.isNull()){
        delete this->position.data();
    }

    //delete sensor control
    if(!this->sensorControl.isNull()){
        delete this->sensorControl.data();
    }

}

/*!
 * \brief Station::getIsActiveStation
 * \return
 */
const bool &Station::getIsActiveStation() const{
    return this->isActiveStation;
}

/*!
 * \brief Station::setActiveStationState
 * \param isActiveStation
 */
void Station::setActiveStationState(const bool &isActiveStation){
    if(this->isActiveStation != isActiveStation){
        this->isActiveStation = isActiveStation;
        emit this->activeStationChanged(this->id);
    }
}

/*!
 * \brief Station::getPosition
 * \return
 */
const QPointer<Point> &Station::getPosition() const{
    return this->position;
}

/*!
* \brief Station::getXAxis
* \return
*/
const Direction &Station::getXAxis() const{
   return this->xAxis;
}

/*!
* \brief Station::getYAxis
* \return
*/
const Direction &Station::getYAxis() const{
   return this->yAxis;
}

/*!
* \brief Station::getZAxis
* \return
*/
const Direction &Station::getZAxis() const{
   return this->zAxis;
}

/*!
* \brief Station::setCoordinateSystem
* \param origin
* \param xAxis
* \param yAxis
* \param zAxis
*/
void Station::setCoordinateSystem(const Position &origin, const Direction &xAxis, const Direction &yAxis, const Direction &zAxis){
   this->position->setPoint(origin);
   this->xAxis = xAxis;
   this->yAxis = yAxis;
   this->zAxis = zAxis;
}

/*!
 * \brief Station::getCoordinateSystem
 * \return
 */
const QPointer<CoordinateSystem> &Station::getCoordinateSystem() const{
    return this->stationSystem;
}

/*!
 * \brief Station::getSensorConfiguration
 * \return
 */
SensorConfiguration Station::getSensorConfiguration(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return SensorConfiguration();
    }

    return this->sensorControl->getSensorConfiguration();

}

/*!
 * \brief Station::setSensorConfiguration
 * \param sConfig
 */
void Station::setSensorConfiguration(const SensorConfiguration &sConfig){

    //check sensor control
    if(this->sensorControl.isNull()){
        return;
    }

    this->sensorControl->setSensorConfiguration(sConfig);

}

/*!
 * \brief Station::getSensor
 * \return
 */
Sensor Station::getSensor() const{

    //check sensor control
    if(this->sensorControl.isNull()){
        return Sensor();
    }

    return this->sensorControl->getSensor();

}

/*!
 * \brief Station::setSensor
 * \param sensor
 */
void Station::setSensor(const QPointer<Sensor> &sensor){

    //check active station state
    if(!this->isActiveStation){
        return;
    }

    //check sensor
    if(sensor.isNull()){
        return;
    }

    //check sensor control
    if(this->sensorControl.isNull()){
        return;
    }

    this->sensorControl->setSensor(sensor);
    emit this->sensorChanged(this->id);

}

/*!
 * \brief Station::takeSensor
 * \return
 */
QPointer<Sensor> Station::takeSensor(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return QPointer<Sensor>(NULL);
    }

    QPointer<Sensor> sensor = this->sensorControl->takeSensor();

    emit this->sensorChanged(this->id);

    return sensor;

}

/*!
 * \brief Station::resetSensor
 */
void Station::resetSensor(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return;
    }

    this->sensorControl->resetSensor();
    emit this->sensorChanged(this->id);

}

/*!
 * \brief Station::getUsedSensors
 * \return
 */
const QList<Sensor> Station::getUsedSensors() const{

    //check sensor control
    if(this->sensorControl.isNull()){
        return QList<Sensor>();
    }

    return this->sensorControl->getUsedSensors();

}

/*!
 * \brief Station::getTargetGeometries
 * \return
 */
QList<QPointer<Geometry> > Station::getTargetGeometries() const{

    QList<QPointer<Geometry> > geometries;

    //get a list of all readings
    QList<QPointer<Reading> > readings;
    foreach(const QPointer<Reading> &reading, this->cartesianReadings){
        readings.append(reading);
    }
    foreach(const QPointer<Reading> &reading, this->directionReadings){
        readings.append(reading);
    }
    foreach(const QPointer<Reading> &reading, this->distanceReadings){
        readings.append(reading);
    }
    foreach(const QPointer<Reading> &reading, this->polarReadings){
        readings.append(reading);
    }
    foreach(const QPointer<Reading> &reading, this->levelReadings){
        readings.append(reading);
    }
    foreach(const QPointer<Reading> &reading, this->temperatureRadings){
        readings.append(reading);
    }
    foreach(const QPointer<Reading> &reading, this->undefinedReadings){
        readings.append(reading);
    }

    //get target geometries
    foreach(const QPointer<Reading> &reading, readings){

        //check reading
        if(reading.isNull() || reading->getObservation().isNull()){
            continue;
        }

        //add target geometries of the observation
        const QList<QPointer<Geometry> > &targetGeometries = reading->getObservation()->getTargetGeometries();
        foreach(const QPointer<Geometry> &geom, targetGeometries){
            if(!geom.isNull() && !geometries.contains(geom)){
                geometries.append(geom);
            }
        }

    }

    return geometries;

}

/*!
 * \brief Station::setUsedSensors
 * \param sensors
 */
void Station::setUsedSensors(const QList<Sensor> &sensors){

    //check sensor control
    if(this->sensorControl.isNull()){
        return;
    }

    return this->sensorControl->setUsedSensors(sensors);

}

/*!
 * \brief Station::getIsSensorSet
 * \return
 */
bool Station::getIsSensorSet(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return false;
    }

    return this->sensorControl->getIsSensorSet();

}

/*!
 * \brief Station::getIsSensorConnected
 * \return
 */
bool Station::getIsSensorConnected(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return false;
    }

    return this->sensorControl->getIsSensorConnected();

}

/*!
 * \brief Station::getIsReadyForMeasurement
 * \return
 */
bool Station::getIsReadyForMeasurement(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return false;
    }

    return this->sensorControl->getIsReadyForMeasurement();

}

/*!
 * \brief Station::getIsBusy
 * \return
 */
bool Station::getIsBusy(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return false;
    }

    return this->sensorControl->getIsBusy();

}

/*!
 * \brief Station::getSensorStatus
 * \return
 */
QMap<QString, QString> Station::getSensorStatus(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return QMap<QString, QString>();
    }

    return this->sensorControl->getSensorStatus();

}

/*!
 * \brief Station::getActiveSensorType
 * \return
 */
SensorTypes Station::getActiveSensorType() const{

    //check sensor control
    if(this->sensorControl.isNull()){
        return eUndefinedSensor;
    }

    return this->sensorControl->getActiveSensorType();

}

/*!
 * \brief Station::getSupportedReadingTypes
 * \return
 */
QList<ReadingTypes> Station::getSupportedReadingTypes() const{

    //check sensor control
    if(this->sensorControl.isNull()){
        return QList<ReadingTypes>();
    }

    return this->sensorControl->getSupportedReadingTypes();

}

/*!
 * \brief Station::getSupportedConnectionTypes
 * \return
 */
QList<ConnectionTypes> Station::getSupportedConnectionTypes() const{

    //check sensor control
    if(this->sensorControl.isNull()){
        return QList<ConnectionTypes>();
    }

    return this->sensorControl->getSupportedConnectionTypes();

}

/*!
 * \brief Station::getSupportedSensorActions
 * \return
 */
QList<SensorFunctions> Station::getSupportedSensorActions() const{

    //check sensor control
    if(this->sensorControl.isNull()){
        return QList<SensorFunctions>();
    }

    return this->sensorControl->getSupportedSensorActions();

}

/*!
 * \brief Station::getSelfDefinedActions
 * \return
 */
QStringList Station::getSelfDefinedActions() const{

    //check sensor control
    if(this->sensorControl.isNull()){
        return QStringList();
    }

    return this->sensorControl->getSelfDefinedActions();

}

/*!
 * \brief Station::getStreamFormat
 * \return
 */
ReadingTypes Station::getStreamFormat(){

    //check sensor control
    if(this->sensorControl.isNull()){
        return eUndefinedReading;
    }

    return this->sensorControl->getStreamFormat();

}

/*!
 * \brief Station::setStreamFormat
 * \param streamFormat
 */
void Station::setStreamFormat(ReadingTypes streamFormat){

    //check sensor control
    if(this->sensorControl.isNull()){
        return;
    }

    this->sensorControl->setStreamFormat(streamFormat);

}

/*!
 * \brief Station::recalc
 */
void Station::recalc(){
    Feature::recalc();
}

/*!
 * \brief Station::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Station::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement station = Feature::toOpenIndyXML(xmlDoc);

    if(station.isNull()){
        return station;
    }

    station.setTagName("station");

    //add station attributes
    station.setAttribute("activeStation", this->getIsActiveStation());

    //add used sensors
    if(!this->sensorControl.isNull() && this->sensorControl->getUsedSensors().size() > 0){
        QDomElement usedSensors = xmlDoc.createElement("usedSensors");
        foreach(const Sensor &s, this->sensorControl->getUsedSensors()){
            QDomElement sensor = s.toOpenIndyXML(xmlDoc);
            if(!sensor.isNull()){
                usedSensors.appendChild(sensor);
            }
        }
        station.appendChild(usedSensors);
    }

    //add active sensor
    if(!this->sensorControl.isNull()){
        QDomElement activeSensor = this->sensorControl->getSensor().toOpenIndyXML(xmlDoc);
        if(!activeSensor.isNull()){
            activeSensor.setTagName("activeSensor");
            station.appendChild(activeSensor);
        }
    }

    //add position
    if(!this->position.isNull()){
        QDomElement position = xmlDoc.createElement("position");
        position.setAttribute("ref", this->position->getId());
        station.appendChild(position);
    }

    //add coordinate system
    if(!this->stationSystem.isNull()){
        QDomElement stationSystem = xmlDoc.createElement("coordinateSystem");
        stationSystem.setAttribute("ref", this->stationSystem->getId());
        station.appendChild(stationSystem);
    }

    return station;

}

/*!
 * \brief Station::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Station::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        if(!xmlElem.hasAttribute("activeStation")){
            return false;
        }

        //set station attributes
        this->isActiveStation = xmlElem.attribute("activeStation").toInt();

    }

    return result;

}

/*!
 * \brief Station::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(!this->position.isNull()){
        return this->position->getDisplayX(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(!this->position.isNull()){
        return this->position->getDisplayY(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(!this->position.isNull()){
        return this->position->getDisplayZ(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Plane::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    return QString::number(this->zAxis.getVector().getAt(0), 'f', digits);
}

/*!
 * \brief Plane::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    return QString::number(this->zAxis.getVector().getAt(1), 'f', digits);
}

/*!
 * \brief Plane::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    return QString::number(this->zAxis.getVector().getAt(2), 'f', digits);
}

/*!
 * \brief Station::setUpFeatureId
 * Generate a new unique id when the current job was set
 */
void Station::setUpFeatureId(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //generate unique id for station's position
    if(!this->position.isNull()){
        this->position->id = this->job->generateUniqueId();
    }

    //generate unique id for station's coordinate system
    if(!this->stationSystem.isNull()){
        this->stationSystem->id = this->job->generateUniqueId();
    }

    //generate unique ids for station's readings
    foreach(const QPointer<Reading> &reading, this->cartesianReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->polarReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->directionReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->distanceReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->temperatureRadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->levelReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->undefinedReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }

    //feature specific
    Feature::setUpFeatureId();

}

/*!
 * \brief Station::stationNameChanged
 * Update the station system name whenever the station's name has changed
 * \param featureId
 * \param oldName
 */
void Station::stationNameChanged(const int &featureId, const QString &oldName){
    if(!this->stationSystem.isNull()){
        this->stationSystem->setFeatureName(this->name);
    }
}

/*!
 * \brief Station::connectSensorControl
 */
void Station::connectSensorControl(){

    //connect sensor actions
    QObject::connect(this, &Station::connectSensor, this->sensorControl.data(), &SensorControl::connectSensor, Qt::AutoConnection);
    QObject::connect(this, &Station::disconnectSensor, this->sensorControl.data(), &SensorControl::disconnectSensor, Qt::AutoConnection);
    QObject::connect(this, &Station::measure, this->sensorControl.data(), &SensorControl::measure, Qt::AutoConnection);
    void (Station:: *movePolarSignal)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative,
                                      const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &Station::move;
    void (SensorControl:: *movePolarSlot)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative,
                                          const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &SensorControl::move;
    QObject::connect(this, movePolarSignal, this->sensorControl.data(), movePolarSlot);
    void (Station:: *moveCartesianSignal)(const double &x, const double &y, const double &z,
                                          const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &Station::move;
    void (SensorControl:: *moveCartesianSlot)(const double &x, const double &y, const double &z,
                                              const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &SensorControl::move;
    QObject::connect(this, moveCartesianSignal, this->sensorControl.data(), moveCartesianSlot);
    QObject::connect(this, &Station::initialize, this->sensorControl.data(), &SensorControl::initialize, Qt::AutoConnection);
    QObject::connect(this, &Station::motorState, this->sensorControl.data(), &SensorControl::motorState, Qt::AutoConnection);
    QObject::connect(this, &Station::home, this->sensorControl.data(), &SensorControl::home, Qt::AutoConnection);
    QObject::connect(this, &Station::toggleSight, this->sensorControl.data(), &SensorControl::toggleSight, Qt::AutoConnection);
    QObject::connect(this, &Station::compensation, this->sensorControl.data(), &SensorControl::compensation, Qt::AutoConnection);
    QObject::connect(this, &Station::selfDefinedAction, this->sensorControl.data(), &SensorControl::selfDefinedAction, Qt::AutoConnection);
    QObject::connect(this, &Station::search, this->sensorControl.data(), &SensorControl::search, Qt::AutoConnection);

    //connect sensor streaming
    QObject::connect(this, &Station::startReadingStream, this->sensorControl.data(), &SensorControl::startReadingStream, Qt::AutoConnection);
    QObject::connect(this, &Station::stopReadingStream, this->sensorControl.data(), &SensorControl::stopReadingStream, Qt::AutoConnection);
    QObject::connect(this, &Station::startConnectionMonitoringStream, this->sensorControl.data(), &SensorControl::startConnectionMonitoringStream, Qt::AutoConnection);
    QObject::connect(this, &Station::stopConnectionMonitoringStream, this->sensorControl.data(), &SensorControl::stopConnectionMonitoringStream, Qt::AutoConnection);
    QObject::connect(this, &Station::startStatusMonitoringStream, this->sensorControl.data(), &SensorControl::startStatusMonitoringStream, Qt::AutoConnection);
    QObject::connect(this, &Station::stopStatusMonitoringStream, this->sensorControl.data(), &SensorControl::stopStatusMonitoringStream, Qt::AutoConnection);

    //connect sensor action results
    QObject::connect(this->sensorControl.data(), &SensorControl::commandFinished, this, &Station::commandFinished, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::measurementFinished, this, &Station::addReadings, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::measurementFinished, this, &Station::measurementFinished, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::measurementDone, this, &Station::measurementDone, Qt::QueuedConnection);

    //connect sensor streaming results
    QObject::connect(this->sensorControl.data(), &SensorControl::realTimeReading, this, &Station::realTimeReading, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::realTimeStatus, this, &Station::realTimeStatus, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::connectionLost, this, &Station::connectionLost, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::connectionReceived, this, &Station::connectionReceived, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::isReadyForMeasurement, this, &Station::isReadyForMeasurement, Qt::AutoConnection);

    //connect sensor messages
    QObject::connect(this->sensorControl.data(), &SensorControl::sensorMessage, this, &Station::sensorMessage, Qt::AutoConnection);

    QObject::connect(this, &Station::finishMeasurement, this->sensorControl.data(), &SensorControl::finishMeasurement, Qt::QueuedConnection);

}

/*!
 * \brief Station::disconnectSensorControl
 */
void Station::disconnectSensorControl(){

    //disconnect sensor actions
    QObject::disconnect(this, &Station::connectSensor, this->sensorControl.data(), &SensorControl::connectSensor);
    QObject::disconnect(this, &Station::disconnectSensor, this->sensorControl.data(), &SensorControl::disconnectSensor);
    QObject::disconnect(this, &Station::measure, this->sensorControl.data(), &SensorControl::measure);
    void (Station:: *movePolarSignal)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative,
                                      const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &Station::move;
    void (SensorControl:: *movePolarSlot)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative,
                                          const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &SensorControl::move;
    QObject::disconnect(this, movePolarSignal, this->sensorControl.data(), movePolarSlot);
    void (Station:: *moveCartesianSignal)(const double &x, const double &y, const double &z,
                                          const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &Station::move;
    void (SensorControl:: *moveCartesianSlot)(const double &x, const double &y, const double &z,
                                              const bool &measure, const int &geomId, const MeasurementConfig &mConfig) = &SensorControl::move;
    QObject::disconnect(this, moveCartesianSignal, this->sensorControl.data(), moveCartesianSlot);
    QObject::disconnect(this, &Station::initialize, this->sensorControl.data(), &SensorControl::initialize);
    QObject::disconnect(this, &Station::motorState, this->sensorControl.data(), &SensorControl::motorState);
    QObject::disconnect(this, &Station::home, this->sensorControl.data(), &SensorControl::home);
    QObject::disconnect(this, &Station::toggleSight, this->sensorControl.data(), &SensorControl::toggleSight);
    QObject::disconnect(this, &Station::compensation, this->sensorControl.data(), &SensorControl::compensation);
    QObject::disconnect(this, &Station::selfDefinedAction, this->sensorControl.data(), &SensorControl::selfDefinedAction);

    //disconnect sensor streaming
    QObject::disconnect(this, &Station::startReadingStream, this->sensorControl.data(), &SensorControl::startReadingStream);
    QObject::disconnect(this, &Station::stopReadingStream, this->sensorControl.data(), &SensorControl::stopReadingStream);
    QObject::disconnect(this, &Station::startConnectionMonitoringStream, this->sensorControl.data(), &SensorControl::startConnectionMonitoringStream);
    QObject::disconnect(this, &Station::stopConnectionMonitoringStream, this->sensorControl.data(), &SensorControl::stopConnectionMonitoringStream);
    QObject::disconnect(this, &Station::startStatusMonitoringStream, this->sensorControl.data(), &SensorControl::startStatusMonitoringStream);
    QObject::disconnect(this, &Station::stopStatusMonitoringStream, this->sensorControl.data(), &SensorControl::stopStatusMonitoringStream);

    //disconnect sensor action results
    QObject::disconnect(this->sensorControl.data(), &SensorControl::commandFinished, this, &Station::commandFinished);
    QObject::disconnect(this->sensorControl.data(), &SensorControl::measurementFinished, this, &Station::addReadings);
    QObject::disconnect(this->sensorControl.data(), &SensorControl::measurementFinished, this, &Station::measurementFinished);
    QObject::disconnect(this->sensorControl.data(), &SensorControl::measurementDone, this, &Station::measurementDone);

    //disconnect sensor streaming results
    QObject::disconnect(this->sensorControl.data(), &SensorControl::realTimeReading, this, &Station::realTimeReading);
    QObject::disconnect(this->sensorControl.data(), &SensorControl::realTimeStatus, this, &Station::realTimeStatus);
    QObject::disconnect(this->sensorControl.data(), &SensorControl::connectionLost, this, &Station::connectionLost);
    QObject::disconnect(this->sensorControl.data(), &SensorControl::connectionReceived, this, &Station::connectionReceived);
    QObject::disconnect(this->sensorControl.data(), &SensorControl::isReadyForMeasurement, this, &Station::isReadyForMeasurement);

    //connect sensor messages
    QObject::disconnect(this->sensorControl.data(), &SensorControl::sensorMessage, this, &Station::sensorMessage);


    QObject::disconnect(this, &Station::finishMeasurement, this->sensorControl.data(), &SensorControl::finishMeasurement);

}

/*!
 * \brief Station::addReadings
 * \param geomId
 * \param readings
 */
void Station::addReadings(const int &geomId, const QList<QPointer<Reading> > &readings){

    MeasurementConfig measurementConfig = this->job->getFeatureById(geomId)->getGeometry()->getMeasurementConfig();
    foreach(const QPointer<Reading> &reading, readings){

        //check reading
        if(reading.isNull()){
            continue;
        }

        //set sensor configuration of reading
        reading->setSensorConfiguration(this->getSensorConfiguration());
        reading->setMeasurementConfig(measurementConfig);

        switch(reading->getTypeOfReading()){
        case eCartesianReading:
            this->cartesianReadings.append(reading);
            break;
        case eDirectionReading:
            this->directionReadings.append(reading);
            break;
        case eDistanceReading:
            this->distanceReadings.append(reading);
            break;
        case ePolarReading:
            this->polarReadings.append(reading);
            break;
        case eLevelReading:
            this->levelReadings.append(reading);
            break;
        case eTemperatureReading:
            this->temperatureRadings.append(reading);
            break;
        }

    }

}

/*!
 * \brief Station::removeReading
 * \param reading
 */
void Station::removeReading(const QPointer<Reading> &reading){

    this->cartesianReadings.removeOne(reading);
    this->directionReadings.removeOne(reading);
    this->distanceReadings.removeOne(reading);
    this->polarReadings.removeOne(reading);
    this->levelReadings.removeOne(reading);
    this->temperatureRadings.removeOne(reading);
    this->undefinedReadings.removeOne(reading);

}

/*!
 * \brief Station::setJob
 * \param job
 */
void Station::setJob(const QPointer<OiJob> &job){

    //check and set job
    if(job.isNull()){
        return;
    }
    this->job = job;

    //pass job to the station system
    if(!this->stationSystem.isNull()){
        this->stationSystem->setJob(job);
    }

    //pass the job to the station point
    if(!this->position.isNull()){
        this->position->setJob(job);
    }

    //generate feature id
    this->setUpFeatureId();

}
void Station::setSensorWorkerThread(QPointer<QThread> t) {
    this->sensorControl->setSensorWorkerThread(t);
}
