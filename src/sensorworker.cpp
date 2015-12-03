#include "sensorworker.h"

using namespace oi;

/*!
 * \brief SensorWorker::SensorWorker
 * \param locker
 * \param parent
 */
SensorWorker::SensorWorker(QObject *parent) : QObject(parent), isSensorConnected(false),
    isReadingStreamStarted(false), isConnectionStreamStarted(false), isStatusStreamStarted(false){

}

/*!
 * \brief SensorWorker::~SensorWorker
 */
SensorWorker::~SensorWorker(){

    //check and disconnect old sensor
    if(!this->sensor.isNull()){
        this->sensor->disconnectSensor();
        delete this->sensor;
    }

}

/*!
 * \brief SensorWorker::setInstrument
 * \param sensor
 */
void SensorWorker::setSensor(QPointer<Sensor> sensor){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check and disconnect old sensor
    if(!this->sensor.isNull()){
        this->sensor->disconnectSensor();
        delete this->sensor;
    }

    if(!sensor.isNull()){
        this->sensor = sensor;
    }

}

/*!
 * \brief SensorWorker::setSensorConfiguration
 * \param sConfig
 */
void SensorWorker::setSensorConfiguration(SensorConfiguration sConfig){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        return;
    }

    this->sensor->setSensorConfiguration(sConfig);

}

/*!
 * \brief SensorWorker::getIsSensorConnected
 * \return
 */
bool SensorWorker::getIsSensorConnected(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        return false;
    }

    this->sensor->getConnectionState();

}

/*!
 * \brief SensorWorker::getIsReadyForMeasurement
 * \return
 */
bool SensorWorker::getIsReadyForMeasurement(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return false;
    }

    this->sensor->getIsReadyForMeasurement();

}

/*!
 * \brief SensorWorker::getIsBusy
 * \return
 */
bool SensorWorker::getIsBusy(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return false;
    }

    this->sensor->getIsBusy();

}

/*!
 * \brief SensorWorker::getSensorStatus
 * \return
 */
QMap<QString, QString> SensorWorker::getSensorStatus(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return QMap<QString, QString>();
    }

    this->sensor->getSensorStatus();

}

/*!
 * \brief SensorWorker::connectSensor
 */
void SensorWorker::connectSensor(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "sensor connected";
    bool success = false;
    if(this->sensor->getConnectionState()){
        msg = "sensor is already connected";
    }else{

        //connect sensor
        success = this->sensor->connectSensor();
        if(!success){
            msg = "failed to connect sensor";
        }else{
            this->isSensorConnected = true;
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::disconnectSensor
 */
void SensorWorker::disconnectSensor(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "sensor disconnected";
    bool success = false;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //disconnect sensor
        success = this->sensor->disconnectSensor();
        if(!success){
            msg = "failed to disconnect sensor";
        }else{
            this->isSensorConnected = false;
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::measure
 * \param geomId
 * \param mConfig
 */
void SensorWorker::measure(int geomId, MeasurementConfig mConfig){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to measure";
    bool success = false;
    QList<QPointer<Reading> > readings;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //measure
        readings = this->sensor->measure(mConfig);
        if(readings.size() > 0){
            msg = "measurement finished";
            success = true;
        }

    }

    emit this->commandFinished(success, msg);
    if(success){
        emit this->measurementFinished(geomId, readings);
    }

}

/*!
 * \brief SensorWorker::move
 * \param azimuth
 * \param zenith
 * \param distance
 * \param isRelative
 * \param measure
 * \param geomId
 * \param mConfig
 */
void SensorWorker::move(double azimuth, double zenith, double distance, bool isRelative, bool measure, int geomId, MeasurementConfig mConfig){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to move sensor";
    bool success = false;
    QList<QPointer<Reading> > readings;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //set up sensor attributes
        SensorAttributes attr;
        attr.moveAzimuth = azimuth;
        attr.moveZenith = zenith;
        attr.moveDistance = distance;
        attr.moveIsRelative = isRelative;

        //move
        success = this->sensor->accept(eMoveAngle, attr);
        if(success){

            msg = "moving sensor finished";

            //optionally perform measurement after move
            if(measure){

                //start measure
                readings = this->sensor->measure(mConfig);
                if(readings.size() > 0){
                    msg.append(", measurement finished");
                }else{
                    msg.append(", failed to measure");
                    success = false;
                }

            }

        }

    }

    emit this->commandFinished(success, msg);
    if(success && measure){
        emit this->measurementFinished(geomId, readings);
    }

}

/*!
 * \brief SensorWorker::move
 * \param x
 * \param y
 * \param z
 * \param measure
 * \param geomId
 * \param mConfig
 */
void SensorWorker::move(double x, double y, double z, bool measure, int geomId, MeasurementConfig mConfig){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to move sensor";
    bool success = false;
    QList<QPointer<Reading> > readings;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //set up sensor attributes
        SensorAttributes attr;
        attr.moveX = x;
        attr.moveY = y;
        attr.moveZ = z;
        attr.moveIsRelative = false;

        //move
        success = this->sensor->accept(eMoveAngle, attr);
        if(success){

            msg = "moving sensor finished";

            //optionally perform measurement after move
            if(measure){

                //start measure
                readings = this->sensor->measure(mConfig);
                if(readings.size() > 0){
                    msg.append(", measurement finished");
                }else{
                    msg.append(", failed to measure");
                    success = false;
                }

            }

        }

    }

    emit this->commandFinished(success, msg);
    if(success && measure){
        emit this->measurementFinished(geomId, readings);
    }

}

/*!
 * \brief SensorWorker::initialize
 */
void SensorWorker::initialize(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to initialize sensor";
    bool success = false;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //initialize sensor
        success = this->sensor->accept(eInitialize, SensorAttributes());
        if(success){
            msg = "initializing finished";
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::motorState
 */
void SensorWorker::motorState(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to change motor state";
    bool success = false;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //changing motor state
        success = this->sensor->accept(eMotorState, SensorAttributes());
        if(success){
            msg = "changing motor state finished";
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::home
 */
void SensorWorker::home(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to set sensor to home position";
    bool success = false;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //setting sensor to home position
        success = this->sensor->accept(eHome, SensorAttributes());
        if(success){
            msg = "setting home finished";
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::toggleSight
 */
void SensorWorker::toggleSight(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to toggle sight orientation";
    bool success = false;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //toggle sight
        success = this->sensor->accept(eToggleSight, SensorAttributes());
        if(success){
            msg = "toggle sight orientation finished";
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::compensation
 */
void SensorWorker::compensation(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to start compensation";
    bool success = false;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //starting compensation
        success = this->sensor->accept(eCompensation, SensorAttributes());
        if(success){
            msg = "starting compensation finished";
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::selfDefinedAction
 * \param action
 */
void SensorWorker::selfDefinedAction(QString action){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to do self defined action";
    bool success = false;
    if(!this->sensor->getConnectionState()){
        msg = "sensor is not connected";
    }else{

        //starting compensation
        success = this->sensor->doSelfDefinedAction(action);
        if(success){
            msg = "self defined action finished";
        }

    }

    emit this->commandFinished(success, msg);

}

/*!
 * \brief SensorWorker::getStreamFormat
 * \return
 */
ReadingTypes SensorWorker::getStreamFormat(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    return this->streamFormat;
}

/*!
 * \brief SensorWorker::setStreamFormat
 * \param streamFormat
 */
void SensorWorker::setStreamFormat(ReadingTypes streamFormat){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    this->streamFormat = streamFormat;
}

/*!
 * \brief SensorWorker::startReadingStream
 */
void SensorWorker::startReadingStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        this->isReadingStreamStarted = false;
        emit this->sensorMessage("no sensor instance", eErrorMessage, eConsoleMessage);
        return;
    }

    //check if the reading stream has already been started
    if(this->isReadingStreamStarted){
        return;
    }

    //call streaming method
    this->isReadingStreamStarted = true;
    QMetaObject::invokeMethod(this, "streamReading", Qt::QueuedConnection);


}

/*!
 * \brief SensorWorker::stopReadingStream
 */
void SensorWorker::stopReadingStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    this->isReadingStreamStarted = false;
}

/*!
 * \brief SensorWorker::startConnectionMonitoringStream
 */
void SensorWorker::startConnectionMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        this->isConnectionStreamStarted = false;
        emit this->sensorMessage("no sensor instance", eErrorMessage, eConsoleMessage);
        return;
    }

    //check if the connection stream has already been started
    if(this->isConnectionStreamStarted){
        return;
    }

    //call streaming method
    this->isConnectionStreamStarted = true;
    QMetaObject::invokeMethod(this, "monitorConnectionStatus", Qt::QueuedConnection);

}

/*!
 * \brief SensorWorker::stopConnectionMonitoringStream
 */
void SensorWorker::stopConnectionMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    this->isConnectionStreamStarted = false;
}

/*!
 * \brief SensorWorker::startStatusMonitoringStream
 */
void SensorWorker::startStatusMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check sensor
    if(this->sensor.isNull()){
        this->isStatusStreamStarted = false;
        emit this->sensorMessage("no sensor instance", eErrorMessage, eConsoleMessage);
        return;
    }

    //check if the status stream has already been started
    if(this->isStatusStreamStarted){
        return;
    }

    //call streaming method
    this->isStatusStreamStarted = true;
    QMetaObject::invokeMethod(this, "streamStatus", Qt::QueuedConnection);

}

/*!
 * \brief SensorWorker::stopStatusMonitoringStream
 */
void SensorWorker::stopStatusMonitoringStream(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    this->isStatusStreamStarted = false;
}

/*!
 * \brief SensorWorker::streamReading
 */
void SensorWorker::streamReading(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check streaming status
    if(!this->isReadingStreamStarted){
        return;
    }

    //check sensor
    if(this->sensor.isNull()){
        this->isReadingStreamStarted = false;
        return;
    }

    //get real time reading
    QVariantMap reading = this->sensor->readingStream(this->streamFormat);
    emit this->realTimeReading(reading);

    //put reading stream into event queue again
    QMetaObject::invokeMethod(this, "streamReading", Qt::QueuedConnection);

}

/*!
 * \brief SensorWorker::monitorConnectionStatus
 */
void SensorWorker::monitorConnectionStatus(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check streaming status
    if(!this->isConnectionStreamStarted){
        return;
    }

    //check sensor
    if(this->sensor.isNull()){
        this->isConnectionStreamStarted = false;
        return;
    }

    //get and compare connection status
    bool isConnected = this->sensor->getConnectionState();
    if(isConnected == false && this->isSensorConnected == true){
        this->isSensorConnected = false;
        emit this->connectionLost();
    }else if(isConnected == true && this->isSensorConnected == false){
        this->isSensorConnected = true;
        emit this->connectionReceived();
    }

    //put connection stream into event queue again
    QMetaObject::invokeMethod(this, "monitorConnectionStatus", Qt::QueuedConnection);

}

/*!
 * \brief SensorWorker::streamStatus
 */
void SensorWorker::streamStatus(){

    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    //check streaming status
    if(!this->isStatusStreamStarted){
        return;
    }

    //check sensor
    if(this->sensor.isNull()){
        this->isStatusStreamStarted = false;
        return;
    }

    //get sensor status
    QMap<QString, QString> status = this->sensor->getSensorStatus();
    emit this->realTimeStatus(status);

    //put connection stream into event queue again
    QMetaObject::invokeMethod(this, "streamStatus", Qt::QueuedConnection);

}
