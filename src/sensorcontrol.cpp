#include "sensorcontrol.h"

#include "sensorlistener.h"

using namespace oi;

/*!
 * \brief SensorControl::SensorControl
 * \param station
 * \param parent
 */
SensorControl::SensorControl(QPointer<Station> &station, QObject *parent) : QObject(parent), station(station){

    //create sensor listener, connect it and move it to thread
    this->sensorListener = new SensorListener(this->locker);
    QObject::connect(&this->listenerThread, &QThread::started, this->sensorListener.data(), &SensorListener::startStreaming, Qt::AutoConnection);

    //connect(this,SIGNAL(activateStatStream()),this->sensorListener,SLOT(sensorStatStream()));
    //connect(this,SIGNAL(activateReadingStream(int)),this->sensorListener,SLOT(sensorReadingStream(int)));
    //connect(sensorListener,SIGNAL(connectionLost()),this,SLOT(streamLostSignal()));
    //sensorListener->moveToThread(&listenerThread);
    //listenerThread.start();

}

/*!
 * \brief SensorControl::~SensorControl
 */
SensorControl::~SensorControl(){

    //stop listener thread if it is still running
    if(this->listenerThread.isRunning()){
        this->listenerThread.quit();
        this->listenerThread.wait();
    }

    delete this->sensorListener;

}

/*!
 * \brief SensorControl::getSensor
 * \return
 */
const QPointer<Sensor> &SensorControl::getSensor() const{
    return this->sensor;
}

/*!
 * \brief SensorControl::setSensor
 * \param sensor
 */
void SensorControl::setSensor(const QPointer<Sensor> &sensor){

    //check sensor
    if(sensor.isNull()){
        return;
    }

    //check old sensor and add it to list of used sensors
    if(!this->sensor.isNull()){

        //stop listener thread if it is still running
        if(this->listenerThread.isRunning()){
            this->listenerThread.quit();
            this->listenerThread.wait(5000);
        }

        this->usedSensors.append(this->sensor);

    }

    //set new sensor
    this->sensor = sensor;
    this->sensorListener->setSensor(this->sensor);

    //move sensor listener to thread and start listening
    this->sensorListener->moveToThread(&this->listenerThread);
    this->listenerThread.start();

}

/*!
 * \brief SensorControl::getUsedSensors
 * \return
 */
const QList<QPointer<Sensor> > &SensorControl::getUsedSensors() const{
    return this->usedSensors;
}

/*!
 * \brief SensorControl::getSensorListener
 * \return
 */
const QPointer<SensorListener> SensorControl::getSensorListener() const{
    return this->sensorListener;
}

/*!
 * \brief SensorControl::setStreamFormat
 * \param streamFormat
 */
void SensorControl::setStreamFormat(ReadingTypes streamFormat){

    //stop listener thread if it is still running
    if(this->listenerThread.isRunning()){
        this->listenerThread.quit();
        this->listenerThread.wait();
    }

    this->sensorListener->setReadingStreamFormat(streamFormat);

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return;
    }

    //restart sensor listener
    this->sensorListener->moveToThread(&this->listenerThread);
    this->listenerThread.start();

}

/*!
 * \brief SensorControl::getIsSensorConnected
 * \return
 */
bool SensorControl::getIsSensorConnected(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        locker.unlock();
        return false;
    }

    //check connection state
    bool isConnected = false;
    if(this->sensor->getConnectionState()){
        isConnected = true;
    }

    this->locker.unlock();

    return isConnected;

}

/*!
 * \brief SensorControl::getIsReadyForMeasurement
 * \return
 */
bool SensorControl::getIsReadyForMeasurement(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        locker.unlock();
        return false;
    }

    //check measurement state
    bool isReady = false;
    if(this->sensor->getIsReadyForMeasurement()){
        isReady = true;
    }

    this->locker.unlock();

    return isReady;

}

/*!
 * \brief SensorControl::getIsBusy
 * \return
 */
bool SensorControl::getIsBusy(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        locker.unlock();
        return false;
    }

    //check busy state
    bool isBusy = false;
    if(this->sensor->getIsBusy()){
        isBusy = true;
    }

    this->locker.unlock();

    return isBusy;

}

/*!
 * \brief SensorControl::getSensorStatus
 * \return
 */
QMap<QString, QString> SensorControl::getSensorStatus(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        locker.unlock();
        return QMap<QString, QString>();
    }

    //check sensor status
    QMap<QString, QString> status;
    status = this->sensor->getSensorStatus();

    this->locker.unlock();

    return status;

}

/*!
 * \brief SensorControl::connectSensor
 */
void SensorControl::connectSensor(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        locker.unlock();
        return;
    }

    //connect sensor if it is not connected yet
    bool success = false;
    QString msg;
    if(!this->sensor->getConnectionState()){
        success = this->sensor->connectSensor();
        if(success){
            msg = "sensor connected";
        }else{
            msg = "failed to connect sensor";
        }
    }else{
        msg = "sensor already connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::disconnectSensor
 */
void SensorControl::disconnectSensor(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //disconnect sensor if it is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){
        success = this->sensor->disconnectSensor();
        if(success){
            msg = "sensor disconnected";
        }else{
            msg = "failed to disconnect sensor";
        }
    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::measure
 * \param geomId
 * \param mConfig
 */
void SensorControl::measure(const int &geomId, const MeasurementConfig &mConfig){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start measure if the sensor is connected
    bool success = false;
    QList<QPointer<Reading> > readings;
    QString msg;
    if(this->sensor->getConnectionState()){
        readings = this->sensor->measure(mConfig);
        if(readings.size() > 0){
            msg = "measurement finished";
            success = true;
        }else{
            msg = "failed to measure";
        }
    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);
    if(success){
        emit this->measurementFinished(geomId, readings);
    }

    this->locker.unlock();

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

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start moving if the sensor is connected
    bool success = false;
    QString msg;
    QList<QPointer<Reading> > readings;
    if(this->sensor->getConnectionState()){

        //set up sensor attributes
        SensorAttributes attr;
        attr.moveAzimuth = azimuth;
        attr.moveZenith = zenith;
        attr.moveDistance = distance;
        attr.moveIsRelative = isRelative;

        bool success = this->sensor->accept(eMoveAngle, attr);
        if(success){
            msg = "moving sensor finished";
            if(measure){

                //start measure
                readings = this->sensor->measure(mConfig);
                if(readings.size() > 0){
                    msg.append(", measurement finished");
                    success = true;
                }else{
                    msg.append(", failed to measure");
                }

            }else{
                success = true;
            }
        }else{
            msg = "failed to move sensor";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);
    if(measure && success){
        emit this->measurementFinished(geomId, readings);
    }

    this->locker.unlock();

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

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start moving if the sensor is connected
    bool success = false;
    QString msg;
    QList<QPointer<Reading> > readings;
    if(this->sensor->getConnectionState()){

        //set up sensor attributes
        SensorAttributes attr;
        attr.moveX = x;
        attr.moveY = y;
        attr.moveZ = z;
        attr.moveIsRelative = false;

        bool success = this->sensor->accept(eMoveXYZ, attr);
        if(success){
            msg = "moving sensor finished";
            if(measure){

                //start measure
                readings = this->sensor->measure(mConfig);
                if(readings.size() > 0){
                    msg.append(", measurement finished");
                    success = true;
                }else{
                    msg.append(", failed to measure");
                }

            }else{
                success = true;
            }
        }else{
            msg = "failed to move sensor";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);
    if(measure && success){
        emit this->measurementFinished(geomId, readings);
    }

    this->locker.unlock();

}

/*!
 * \brief SensorControl::initialize
 */
void SensorControl::initialize(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start initializing if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eInitialize, SensorAttributes());
        if(success){
            msg = "initializing finished";
            success = true;
        }else{
            msg = "failed to initialize sensor";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::motorState
 */
void SensorControl::motorState(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start changing motor state if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eMotorState, SensorAttributes());
        if(success){
            msg = "changing motor state finished";
            success = true;
        }else{
            msg = "failed to change motor state";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::home
 */
void SensorControl::home(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start home if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eHome, SensorAttributes());
        if(success){
            msg = "setting home finished";
            success = true;
        }else{
            msg = "failed to set sensor to home position";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::toggleSight
 */
void SensorControl::toggleSight(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start toggle sight if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eToggleSight, SensorAttributes());
        if(success){
            msg = "toggle sight orientation finished";
            success = true;
        }else{
            msg = "failed to toggle sight orientation";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::compensation
 */
void SensorControl::compensation(){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start compensation if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->accept(eCompensation, SensorAttributes());
        if(success){
            msg = "starting compensation finished";
            success = true;
        }else{
            msg = "failed to start compensation";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}

/*!
 * \brief SensorControl::selfDefinedAction
 * \param action
 */
void SensorControl::selfDefinedAction(const QString &action){

    this->locker.lock();

    //check sensor
    if(this->sensor.isNull()){
        this->locker.unlock();
        return;
    }

    //start compensation if the sensor is connected
    bool success = false;
    QString msg;
    if(this->sensor->getConnectionState()){

        bool success = this->sensor->doSelfDefinedAction(action);
        if(success){
            msg = "self defined action finished";
            success = true;
        }else{
            msg = "failed to do self defined action";
        }

    }else{
        msg = "sensor not connected";
    }

    emit this->commandFinished(success, msg);

    this->locker.unlock();

}
