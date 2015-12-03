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
 * \return
 */
const Sensor &SensorControl::getSensor() const{
    return this->sensor;
}

/*!
 * \brief SensorControl::setSensor
 * \param sensor
 */
void SensorControl::setSensor(const QPointer<Sensor> &sensor){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(sensor.isNull()){
        return;
    }

    //check old sensor and add it to the list of used sensors
    if(sensorValid){
        this->usedSensors.append(this->sensor);
    }

    //update current sensor
    this->sensor = Sensor(*sensor.data());
    this->sensorValid = true;

    //start worker thread if it is not running yet
    this->startSensorWorker();

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "setSensor", Qt::QueuedConnection,
                              Q_ARG(QPointer<Sensor>, sensor));

}

/*!
 * \brief SensorControl::resetSensor
 */
void SensorControl::resetSensor(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //stop worker thread
    this->stopSensorWorker();

    //check old sensor and add it to the list of used sensors
    if(sensorValid){
        this->usedSensors.append(this->sensor);
    }

    //update current sensor
    this->sensor = Sensor();
    this->sensorValid = false;

}

/*!
 * \brief SensorControl::getUsedSensors
 * \return
 */
const QList<Sensor> &SensorControl::getUsedSensors() const{
    return this->usedSensors;
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
    QMetaObject::invokeMethod(this->worker, "getStreamFormat", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(ReadingTypes, type));

    return type;

}

/*!
 * \brief SensorControl::setStreamFormat
 * \param streamFormat
 */
void SensorControl::setStreamFormat(ReadingTypes streamFormat){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "setStreamFormat", Qt::QueuedConnection,
                              Q_ARG(ReadingTypes, streamFormat));

}

/*!
 * \brief SensorControl::getIsSensorSet
 * \return
 */
bool SensorControl::getIsSensorSet(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    return this->sensorValid;
}

/*!
 * \brief SensorControl::getIsSensorConnected
 * \return
 */
bool SensorControl::getIsSensorConnected(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return false;
    }

    //call method of sensor worker
    bool isConnected = false;
    QMetaObject::invokeMethod(this->worker, "getIsSensorConnected", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, isConnected));

    return isConnected;

}

/*!
 * \brief SensorControl::getIsReadyForMeasurement
 * \return
 */
bool SensorControl::getIsReadyForMeasurement(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return false;
    }

    //call method of sensor worker
    bool isReady = false;
    QMetaObject::invokeMethod(this->worker, "getIsReadyForMeasurement", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, isReady));

    return isReady;

}

/*!
 * \brief SensorControl::getIsBusy
 * \return
 */
bool SensorControl::getIsBusy(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return false;
    }

    //call method of sensor worker
    bool isBusy = false;
    QMetaObject::invokeMethod(this->worker, "getIsBusy", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(bool, isBusy));

    return isBusy;

}

/*!
 * \brief SensorControl::getSensorStatus
 * \return
 */
QMap<QString, QString> SensorControl::getSensorStatus(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return QMap<QString, QString>();
    }

    //call method of sensor worker
    QMap<QString, QString> status;
    QMetaObject::invokeMethod(this->worker, "getSensorStatus", Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(StringStringMap, status));

    return status;

}

/*!
 * \brief SensorControl::getActiveSensorType
 * \return
 */
SensorTypes SensorControl::getActiveSensorType() const{

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check current sensor
    if(!this->sensorValid){
        return eUndefinedSensor;
    }

    return this->sensor.getSensorConfiguration().getTypeOfSensor();

}

/*!
 * \brief SensorControl::getSupportedReadingTypes
 * \return
 */
QList<ReadingTypes> SensorControl::getSupportedReadingTypes() const{

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check current sensor
    if(!this->sensorValid){
        return QList<ReadingTypes>();
    }

    return this->sensor.getSupportedReadingTypes();

}

/*!
 * \brief SensorControl::getSupportedConnectionTypes
 * \return
 */
QList<ConnectionTypes> SensorControl::getSupportedConnectionTypes() const{

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check current sensor
    if(!this->sensorValid){
        return QList<ConnectionTypes>();
    }

    return this->sensor.getSupportedConnectionTypes();

}

/*!
 * \brief SensorControl::getSupportedSensorActions
 * \return
 */
QList<SensorFunctions> SensorControl::getSupportedSensorActions() const{

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check current sensor
    if(!this->sensorValid){
        return QList<SensorFunctions>();
    }

    return this->sensor.getSupportedSensorActions();

}

/*!
 * \brief SensorControl::getSelfDefinedActions
 * \return
 */
QStringList SensorControl::getSelfDefinedActions() const{

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check current sensor
    if(!this->sensorValid){
        return QStringList();
    }

    return this->sensor.getSelfDefinedActions();

}

/*!
 * \brief SensorControl::getSensorConfiguration
 * \return
 */
SensorConfiguration SensorControl::getSensorConfiguration(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check current sensor
    if(!this->sensorValid){
        return SensorConfiguration();
    }

    return this->sensor.getSensorConfiguration();

}

/*!
 * \brief SensorControl::setSensorConfiguration
 * \param sConfig
 */
void SensorControl::setSensorConfiguration(const SensorConfiguration &sConfig){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check current sensor
    if(!this->sensorValid){
        return;
    }

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "setSensorConfiguration", Qt::QueuedConnection,
                              Q_ARG(SensorConfiguration, sConfig));

    //update sensor helper copy
    this->sensor.setSensorConfiguration(sConfig);

}

/*!
 * \brief SensorControl::connectSensor
 */
void SensorControl::connectSensor(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "connectSensor", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::disconnectSensor
 */
void SensorControl::disconnectSensor(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "disconnectSensor", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::measure
 * \param geomId
 * \param mConfig
 */
void SensorControl::measure(const int &geomId, const MeasurementConfig &mConfig){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "measure", Qt::QueuedConnection,
                              Q_ARG(int, geomId), Q_ARG(MeasurementConfig, mConfig));

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

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "move", Qt::QueuedConnection,
                              Q_ARG(double, azimuth), Q_ARG(double, zenith), Q_ARG(double, distance),
                              Q_ARG(bool, isRelative), Q_ARG(bool, measure), Q_ARG(int, geomId),
                              Q_ARG(MeasurementConfig, mConfig));

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

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "move", Qt::QueuedConnection,
                              Q_ARG(double, x), Q_ARG(double, y), Q_ARG(double, z),
                              Q_ARG(bool, measure), Q_ARG(int, geomId), Q_ARG(MeasurementConfig, mConfig));

}

/*!
 * \brief SensorControl::initialize
 */
void SensorControl::initialize(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "initialize", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::motorState
 */
void SensorControl::motorState(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "motorState", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::home
 */
void SensorControl::home(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "home", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::toggleSight
 */
void SensorControl::toggleSight(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "toggleSight", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::compensation
 */
void SensorControl::compensation(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "compensation", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::selfDefinedAction
 * \param action
 */
void SensorControl::selfDefinedAction(const QString &action){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "selfDefinedAction", Qt::QueuedConnection,
                              Q_ARG(QString, action));

}

/*!
 * \brief SensorControl::startReadingStream
 */
void SensorControl::startReadingStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "startReadingStream", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::stopReadingStream
 */
void SensorControl::stopReadingStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "stopReadingStream", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::startConnectionMonitoringStream
 */
void SensorControl::startConnectionMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "startConnectionMonitoringStream", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::stopConnectionMonitoringStream
 */
void SensorControl::stopConnectionMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "stopConnectionMonitoringStream", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::startStatusMonitoringStream
 */
void SensorControl::startStatusMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "startStatusMonitoringStream", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::stopStatusMonitoringStream
 */
void SensorControl::stopStatusMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor worker
    if(!this->isWorkerRunning()){
        return;
    }

    //call method of sensor worker
    QMetaObject::invokeMethod(this->worker, "stopStatusMonitoringStream", Qt::QueuedConnection);

}

/*!
 * \brief SensorControl::isWorkerValid
 * \return
 */
bool SensorControl::isWorkerValid(){
    if(this->worker.isNull() || this->workerThread.isNull()){
        return false;
    }
    return true;
}

/*!
 * \brief SensorControl::isWorkerRunning
 * \return
 */
bool SensorControl::isWorkerRunning(){
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

/*!
 * \brief SensorControl::connectSensorWorker
 */
void SensorControl::connectSensorWorker(){

    //connect sensor action results
    QObject::connect(this->worker, &SensorWorker::commandFinished, this, &SensorControl::commandFinished, Qt::QueuedConnection);
    QObject::connect(this->worker, &SensorWorker::measurementFinished, this, &SensorControl::measurementFinished, Qt::QueuedConnection);

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
