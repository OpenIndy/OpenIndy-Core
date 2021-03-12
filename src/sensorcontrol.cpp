#include "sensorcontrol.h"

using namespace oi;

/*!
 * \brief SensorControl::SensorControl
 * \param station
 * \param parent
 */
SensorControl::SensorControl(QPointer<Station> &station, QObject *parent) : QObject(parent), station(station), sensorValid(false){

}

/*!
 * \brief SensorControl::~SensorControl
 */
SensorControl::~SensorControl(){
    this->stopSensorWorker();
}

/*!
 * \brief SensorControl::getSensor
 * Returns a copy of the current sensor
 * \return
 */
Sensor SensorControl::getSensor(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return Sensor();
    }

    //get sensor
    Sensor sensor;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getSensor", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(Sensor, sensor));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return sensor;

}

/*!
 * \brief SensorControl::setSensor
 * Sets the current sensor to the given one
 * \param sensor
 */
void SensorControl::setSensor(const QPointer<Sensor> &sensor){

    //check sensor
    if(sensor.isNull()){
        return;
    }

    //start worker thread if it is not running yet
    this->startSensorWorker();

    //check old sensor and add it to the list of used sensors
    if(sensorValid){
        this->usedSensors.append(this->getSensor());
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "setSensor", Qt::BlockingQueuedConnection,
                                                Q_ARG(QPointer<Sensor>, sensor));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    //set sensor valid
    this->sensorValid = true;

}

/*!
 * \brief SensorControl::takeSensor
 * Returns the current sensor instance. That sensor will no longer be used by the sensor worker
 * \return
 */
QPointer<Sensor> SensorControl::takeSensor(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return QPointer<Sensor>(NULL);
    }

    //check old sensor and add it to the list of used sensors
    if(sensorValid){
        this->usedSensors.append(this->getSensor());
    }

    //call method of sensor worker
    QPointer<Sensor> sensor(NULL);
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "takeSensor", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(QPointer<Sensor>, sensor));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    //stop worker thread
    this->stopSensorWorker();

    //set sensor invalid
    this->sensorValid = false;

    return sensor;

}

/*!
 * \brief SensorControl::resetSensor
 * Disconnects and deletes the current sensor
 */
void SensorControl::resetSensor(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //check old sensor and add it to the list of used sensors
    if(sensorValid){
        this->usedSensors.append(this->getSensor());
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "resetSensor", Qt::BlockingQueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    //stop worker thread
    this->stopSensorWorker();

    //set sensor invalid
    this->sensorValid = false;

}

/*!
 * \brief SensorControl::getUsedSensors
 * \return
 */
const QList<Sensor> &SensorControl::getUsedSensors(){
    return this->usedSensors;
}

/*!
 * \brief SensorControl::setUsedSensors
 * \param sensors
 */
void SensorControl::setUsedSensors(const QList<Sensor> &sensors){
    this->usedSensors = sensors;
}

/*!
 * \brief SensorControl::getStreamFormat
 * \return
 */
ReadingTypes SensorControl::getStreamFormat(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return eUndefinedReading;
    }

    //call method of sensor worker
    ReadingTypes type = eUndefinedReading;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getStreamFormat", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(ReadingTypes, type));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return type;

}

/*!
 * \brief SensorControl::setStreamFormat
 * \param streamFormat
 */
void SensorControl::setStreamFormat(ReadingTypes streamFormat){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "setStreamFormat", Qt::QueuedConnection,
                                                Q_ARG(ReadingTypes, streamFormat));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::getIsSensorSet
 * \return
 */
bool SensorControl::getIsSensorSet(){
    return this->sensorValid;
}

/*!
 * \brief SensorControl::getIsSensorConnected
 * \return
 */
bool SensorControl::getIsSensorConnected(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return false;
    }

    //call method of sensor worker
    bool isConnected = false;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getIsSensorConnected", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(bool, isConnected));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return isConnected;

}

/*!
 * \brief SensorControl::getIsReadyForMeasurement
 * \return
 */
bool SensorControl::getIsReadyForMeasurement(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return false;
    }

    //call method of sensor worker
    bool isReady = false;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getIsReadyForMeasurement", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(bool, isReady));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return isReady;

}

/*!
 * \brief SensorControl::getIsBusy
 * \return
 */
bool SensorControl::getIsBusy(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return false;
    }

    //call method of sensor worker
    bool isBusy = false;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getIsBusy", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(bool, isBusy));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return isBusy;

}

/*!
 * \brief SensorControl::getSensorStatus
 * \return
 */
QMap<QString, QString> SensorControl::getSensorStatus(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return QMap<QString, QString>();
    }

    //call method of sensor worker
    QMap<QString, QString> status;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getSensorStatus", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(StringStringMap, status));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return status;

}

/*!
 * \brief SensorControl::getActiveSensorType
 * \return
 */
SensorTypes SensorControl::getActiveSensorType(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return eUndefinedSensor;
    }

    //call method of sensor worker
    SensorTypes type;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getActiveSensorType", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(SensorTypes, type));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return type;

}

/*!
 * \brief SensorControl::getSupportedReadingTypes
 * \return
 */
QList<ReadingTypes> SensorControl::getSupportedReadingTypes(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return QList<ReadingTypes>();
    }

    //call method of sensor worker
    QList<ReadingTypes> types;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getSupportedReadingTypes", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(QList<ReadingTypes>, types));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return types;

}

/*!
 * \brief SensorControl::getSupportedConnectionTypes
 * \return
 */
QList<ConnectionTypes> SensorControl::getSupportedConnectionTypes(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return QList<ConnectionTypes>();
    }

    //call method of sensor worker
    QList<ConnectionTypes> types;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getSupportedConnectionTypes", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(QList<ConnectionTypes>, types));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return types;

}

/*!
 * \brief SensorControl::getSupportedSensorActions
 * \return
 */
QList<SensorFunctions> SensorControl::getSupportedSensorActions(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return QList<SensorFunctions>();
    }

    //call method of sensor worker
    QList<SensorFunctions> actions;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getSupportedSensorActions", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(QList<SensorFunctions>, actions));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return actions;

}

/*!
 * \brief SensorControl::getSelfDefinedActions
 * \return
 */
QStringList SensorControl::getSelfDefinedActions(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return QStringList();
    }

    //call method of sensor worker
    QStringList actions;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getSelfDefinedActions", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(QStringList, actions));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return actions;

}

/*!
 * \brief SensorControl::getSensorConfiguration
 * \return
 */
SensorConfiguration SensorControl::getSensorConfiguration(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return SensorConfiguration();
    }

    //call method of sensor worker
    SensorConfiguration sConfig;
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "getSensorConfiguration", Qt::BlockingQueuedConnection,
                                                Q_RETURN_ARG(SensorConfiguration, sConfig));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

    return sConfig;

}

/*!
 * \brief SensorControl::setSensorConfiguration
 * \param sConfig
 */
void SensorControl::setSensorConfiguration(const SensorConfiguration &sConfig){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "setSensorConfiguration", Qt::BlockingQueuedConnection,
                                                Q_ARG(SensorConfiguration, sConfig));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::connectSensor
 */
void SensorControl::connectSensor(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "connectSensor", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::disconnectSensor
 */
void SensorControl::disconnectSensor(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "disconnectSensor", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::measure
 * \param geomId
 * \param mConfig
 */
void SensorControl::measure(const int &geomId, const MeasurementConfig &mConfig){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "measure", Qt::QueuedConnection,
                                                Q_ARG(int, geomId), Q_ARG(MeasurementConfig, mConfig));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::move
 * \param azimuth
 * \param zenith
 * \param distance
 * \param isRelative
 * \param measure
 * \param geomId
 * \param mConfig
 */
void SensorControl::move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative, const bool &measure, const int &geomId, const MeasurementConfig &mConfig){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "move", Qt::QueuedConnection,
                                                Q_ARG(double, azimuth), Q_ARG(double, zenith), Q_ARG(double, distance),
                                                Q_ARG(bool, isRelative), Q_ARG(bool, measure), Q_ARG(int, geomId),
                                                Q_ARG(MeasurementConfig, mConfig));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::move
 * \param x
 * \param y
 * \param z
 * \param measure
 * \param geomId
 * \param mConfig
 */
void SensorControl::move(const double &x, const double &y, const double &z, const bool &measure, const int &geomId, const MeasurementConfig &mConfig){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "move", Qt::QueuedConnection,
                                                Q_ARG(double, x), Q_ARG(double, y), Q_ARG(double, z),
                                                Q_ARG(bool, measure), Q_ARG(int, geomId), Q_ARG(MeasurementConfig, mConfig));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::initialize
 */
void SensorControl::initialize(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "initialize", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::motorState
 */
void SensorControl::motorState(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "motorState", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::home
 */
void SensorControl::home(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "home", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::toggleSight
 */
void SensorControl::toggleSight(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "toggleSight", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::compensation
 */
void SensorControl::compensation(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "compensation", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::selfDefinedAction
 * \param action
 */
void SensorControl::selfDefinedAction(const QString &action){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "selfDefinedAction", Qt::QueuedConnection,
                                                Q_ARG(QString, action));
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::startReadingStream
 */
void SensorControl::startReadingStream(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "startReadingStream", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::stopReadingStream
 */
void SensorControl::stopReadingStream(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "stopReadingStream", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::startConnectionMonitoringStream
 */
void SensorControl::startConnectionMonitoringStream(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "startConnectionMonitoringStream", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::stopConnectionMonitoringStream
 */
void SensorControl::stopConnectionMonitoringStream(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "stopConnectionMonitoringStream", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::startStatusMonitoringStream
 */
void SensorControl::startStatusMonitoringStream(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "startStatusMonitoringStream", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::stopStatusMonitoringStream
 */
void SensorControl::stopStatusMonitoringStream(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "stopStatusMonitoringStream", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}

/*!
 * \brief SensorControl::isWorkerValid
 * \return
 */
bool SensorControl::isWorkerValid() const{
    if(this->worker.isNull() || this->workerThread.isNull()){
        return false;
    }
    return true;
}

/*!
 * \brief SensorControl::isWorkerRunning
 * \return
 */
bool SensorControl::isWorkerRunning() const{
    if(!this->isWorkerValid() || !this->workerThread->isRunning()){
        return false;
    }
    return true;
}

/*!
 * \brief SensorControl::startSensorWorker
 */
void SensorControl::startSensorWorker(){

    //check sensor worker
    if(!this->isWorkerValid()){

        //create worker and worker thread
        this->worker = new SensorWorker();
        this->workerThread = new QThread();

        //move worker to worker thread
        this->worker->moveToThread(this->workerThread);

        //connect sensor worker
        this->connectSensorWorker();

        //delete worker thread when it is finished
        QObject::connect(this->workerThread, &QThread::finished, this->workerThread, &QThread::deleteLater, Qt::AutoConnection);

    }

    //check if sensor worker is running
    if(!this->isWorkerRunning()){
        this->workerThread->start();
    }

}

/*!
 * \brief SensorControl::stopSensorWorker
 */
void SensorControl::stopSensorWorker(){

    //check worker and worker thread
    if(!this->isWorkerValid()){
        return;
    }

    //disconnect sensor worker
    this->disconnectSensorWorker();

    //stop worker thread if it is still running
    this->worker->deleteLater();
    this->workerThread->exit();

}

void SensorControl::finishMeasurement(){

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    bool hasInvoked = QMetaObject::invokeMethod(this->worker, "finishMeasurement", Qt::QueuedConnection);
    if(!hasInvoked){
        emit this->sensorMessage("Cannot invoke getSensor method of sensor worker", eErrorMessage, eConsoleMessage);
    }

}
/*!
 * \brief SensorControl::connectSensorWorker
 */
void SensorControl::connectSensorWorker(){

    //connect sensor action results
    QObject::connect(this->worker, &SensorWorker::commandFinished, this, &SensorControl::commandFinished, Qt::QueuedConnection);
    QObject::connect(this->worker, &SensorWorker::measurementFinished, this, &SensorControl::measurementFinished, Qt::QueuedConnection);
    QObject::connect(this->worker, &SensorWorker::measurementDone, this, &SensorControl::measurementDone, Qt::QueuedConnection);

    //connect streaming results
    QObject::connect(this->worker, &SensorWorker::realTimeReading, this, &SensorControl::realTimeReading, Qt::QueuedConnection);
    QObject::connect(this->worker, &SensorWorker::realTimeStatus, this, &SensorControl::realTimeStatus, Qt::QueuedConnection);
    QObject::connect(this->worker, &SensorWorker::connectionLost, this, &SensorControl::connectionLost, Qt::QueuedConnection);
    QObject::connect(this->worker, &SensorWorker::connectionReceived, this, &SensorControl::connectionReceived, Qt::QueuedConnection);
    QObject::connect(this->worker, &SensorWorker::isReadyForMeasurement, this, &SensorControl::isReadyForMeasurement, Qt::QueuedConnection);

    //connect sensor messages
    QObject::connect(this->worker, &SensorWorker::sensorMessage, this, &SensorControl::sensorMessage, Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::disconnectSensorWorker
 */
void SensorControl::disconnectSensorWorker(){

}
