#include "sensorworker.h"

using namespace oi;

/*!
 * \brief SensorWorker::SensorWorker
 * \param locker
 * \param parent
 */
SensorWorker::SensorWorker(QObject *parent) : QObject(parent), isSensorConnected(false),
    isReadingStreamStarted(false), isConnectionStreamStarted(false), isStatusStreamStarted(false),
    streamFormat(eUndefinedReading){

}

/*!
 * \brief SensorWorker::~SensorWorker
 */
SensorWorker::~SensorWorker(){

}

/*!
 * \brief SensorWorker::getSensor
 * \return
 */
Sensor SensorWorker::getSensor(){

    //check sensor
    if(this->sensor.isNull()){
        return Sensor();
    }

    return Sensor(*this->sensor.data());

}

/*!
 * \brief SensorWorker::setInstrument
 * \param sensor
 */
void SensorWorker::setSensor(QPointer<Sensor> sensor){

    if(!sensor.isNull()){

        //set sensor
        this->sensor = sensor;

        //connect sensor
        QObject::connect(sensor, &Sensor::sensorMessage, this, &SensorWorker::sensorMessage, Qt::AutoConnection);
        QObject::connect(sensor, &Sensor::asyncSensorResponse, this, &SensorWorker::asyncSensorResponseReceived, Qt::AutoConnection);
        QObject::connect(sensor, &Sensor::asyncMeasurementResult, this, &SensorWorker::asyncSensorMeasurementReceived, Qt::AutoConnection);
        QObject::connect(sensor, &Sensor::asyncStreamResult, this, &SensorWorker::asyncSensorStreamDataReceived, Qt::AutoConnection);
    }

}

/*!
 * \brief SensorWorker::takeSensor
 * \return
 */
QPointer<Sensor> SensorWorker::takeSensor(){

    //save sensor pointer
    QPointer<Sensor> sensor = this->sensor;

    //disconnect sensor
    if(!sensor.isNull()){
        QObject::disconnect(sensor, &Sensor::sensorMessage, this, &SensorWorker::sensorMessage);
        QObject::disconnect(sensor, &Sensor::asyncSensorResponse, this, &SensorWorker::asyncSensorResponseReceived);
        QObject::disconnect(sensor, &Sensor::asyncMeasurementResult, this, &SensorWorker::asyncSensorMeasurementReceived);
        QObject::disconnect(sensor, &Sensor::asyncStreamResult, this, &SensorWorker::asyncSensorStreamDataReceived);
    }

    //set sensor pointer to NULL pointer
    this->sensor = QPointer<Sensor>(NULL);

    return sensor;

}

/*!
 * \brief SensorWorker::resetSensor
 */
void SensorWorker::resetSensor(){

    if(!this->sensor.isNull()){

        //disconnect sensor
        if(this->sensor->getConnectionState()){
            this->sensor->disconnectSensor();
        }
        QObject::disconnect(sensor, &Sensor::sensorMessage, this, &SensorWorker::sensorMessage);
        QObject::disconnect(sensor, &Sensor::asyncSensorResponse, this, &SensorWorker::asyncSensorResponseReceived);
        QObject::disconnect(sensor, &Sensor::asyncMeasurementResult, this, &SensorWorker::asyncSensorMeasurementReceived);
        QObject::disconnect(sensor, &Sensor::asyncStreamResult, this, &SensorWorker::asyncSensorStreamDataReceived);

        //delete sensor
        delete this->sensor.data();

    }

}

/*!
 * \brief SensorWorker::getIsSensorConnected
 * \return
 */
bool SensorWorker::getIsSensorConnected(){
    qDebug() << "SensorWorker::getIsSensorConnected()";
    //check sensor
    if(this->sensor.isNull()){
        return false;
    }

    return this->sensor->getConnectionState();

}

/*!
 * \brief SensorWorker::getIsReadyForMeasurement
 * \return
 */
bool SensorWorker::getIsReadyForMeasurement(){

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return false;
    }

    return this->sensor->getIsReadyForMeasurement();

}

/*!
 * \brief SensorWorker::getIsBusy
 * \return
 */
bool SensorWorker::getIsBusy(){

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return false;
    }

    return this->sensor->getIsBusy();

}

/*!
 * \brief SensorWorker::getSensorStatus
 * \return
 */
QMap<QString, QString> SensorWorker::getSensorStatus(){

    //check sensor
    if(this->sensor.isNull() || !this->sensor->getConnectionState()){
        return QMap<QString, QString>();
    }

    return this->sensor->getSensorStatus();

}

/*!
 * \brief SensorWorker::getActiveSensorType
 * \return
 */
SensorTypes SensorWorker::getActiveSensorType(){

    //check sensor
    if(this->sensor.isNull()){
        return eUndefinedSensor;
    }

    SensorTypes type = this->sensor->getSensorConfiguration().getTypeOfSensor();
    return type;

}

/*!
 * \brief SensorWorker::getSupportedReadingTypes
 * \return
 */
QList<ReadingTypes> SensorWorker::getSupportedReadingTypes(){

    //check sensor
    if(this->sensor.isNull()){
        return QList<ReadingTypes>();
    }

    QList<ReadingTypes> types = this->sensor->getSupportedReadingTypes();
    return types;

}

/*!
 * \brief SensorWorker::getSupportedConnectionTypes
 * \return
 */
QList<ConnectionTypes> SensorWorker::getSupportedConnectionTypes(){

    //check sensor
    if(this->sensor.isNull()){
        return QList<ConnectionTypes>();
    }

    QList<ConnectionTypes> types = this->sensor->getSupportedConnectionTypes();
    return types;

}

/*!
 * \brief SensorWorker::getSupportedSensorActions
 * \return
 */
QList<SensorFunctions> SensorWorker::getSupportedSensorActions(){

    //check sensor
    if(this->sensor.isNull()){
        return QList<SensorFunctions>();
    }

    QList<SensorFunctions> functions = this->sensor->getSupportedSensorActions();
    return functions;

}

/*!
 * \brief SensorWorker::getSelfDefinedActions
 * \return
 */
QStringList SensorWorker::getSelfDefinedActions(){

    //check sensor
    if(this->sensor.isNull()){
        return QStringList();
    }

    QStringList actions = this->sensor->getSelfDefinedActions();
    return actions;

}

/*!
 * \brief SensorWorker::getSensorConfiguration
 * \return
 */
SensorConfiguration SensorWorker::getSensorConfiguration(){

    //check sensor
    if(this->sensor.isNull()){
        return SensorConfiguration();
    }

    SensorConfiguration sConfig = this->sensor->getSensorConfiguration();
    return sConfig;

}

/*!
 * \brief SensorWorker::setSensorConfiguration
 * \param sConfig
 */
void SensorWorker::setSensorConfiguration(SensorConfiguration sConfig){

    //check sensor
    if(this->sensor.isNull()){
        return;
    }

    this->sensor->setSensorConfiguration(sConfig);

}

/*!
 * \brief SensorWorker::connectSensor
 */
void SensorWorker::connectSensor(){

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "sensor connected";
    bool success = false;

    if(!this->sensor->isSensorAsync()){
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
    }else{
        QJsonObject request;
        request.insert("method", "connect");
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
        }
    }

}

/*!
 * \brief SensorWorker::disconnectSensor
 */
void SensorWorker::disconnectSensor(){

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "sensor disconnected";
    bool success = false;

    if(!this->sensor->isSensorAsync()){
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
    }else{
        QJsonObject request;
        request.insert("method", "disconnect");
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
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

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to measure";
    bool success = false;
    QList<QPointer<Reading> > readings;

    if(!this->sensor->isSensorAsync()){
        if(!this->sensor->getConnectionState()){
            msg = "sensor is not connected";
        }else{

            //measure
            readings = this->sensor->measure(mConfig);
            if(readings.size() > 0){
                foreach(QPointer<Reading> r, readings) {
                    QVariant p =  mConfig.getTransientData("isDummyPoint");
                    if(p.isValid()) {
                        r->setProperty("isDummyPoint", p);
                    }
                }
                msg = "measurement finished";
                success = true;
            }

        }

        emit this->measurementDone(success);

        emit this->commandFinished(success, msg);
        if(success){
            emit this->measurementFinished(geomId, readings);
        }
    }else{
        QJsonObject request;
        request.insert("method", "measure");
        request.insert("geomId", geomId);
        this->sensor->setMeasurementConfig(mConfig);
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
        }
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

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    if(!this->sensor->isSensorAsync()){
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
            success = this->sensor->accept(eMoveXYZ, attr);
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
    }else{
        QJsonObject request;
        request.insert("method", "move (cartesian)");
        request.insert("x", x);
        request.insert("y", y);
        request.insert("z", z);
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
        }
    }


}

/*!
 * \brief SensorWorker::initialize
 */
void SensorWorker::initialize(){

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    if(!this->sensor->isSensorAsync()){
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
    }else{
        QJsonObject request;
        request.insert("method", "initialize");
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
        }
    }

}

/*!
 * \brief SensorWorker::motorState
 */
void SensorWorker::motorState(){

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

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to toggle sight orientation";
    bool success = false;

    if(!this->sensor->isSensorAsync()){
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
    }else{
        QJsonObject request;
        request.insert("method", "toggle sight orientation");
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
        }
    }


}

/*!
 * \brief SensorWorker::compensation
 */
void SensorWorker::compensation(){

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
    return this->streamFormat;
}

/*!
 * \brief SensorWorker::setStreamFormat
 * \param streamFormat
 */
void SensorWorker::setStreamFormat(ReadingTypes streamFormat){
    this->streamFormat = streamFormat;
}

/*!
 * \brief SensorWorker::startReadingStream
 */
void SensorWorker::startReadingStream(){

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
    this->isReadingStreamStarted = false;
}

/*!
 * \brief SensorWorker::startConnectionMonitoringStream
 */
void SensorWorker::startConnectionMonitoringStream(){

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
    this->isConnectionStreamStarted = false;
}

/*!
 * \brief SensorWorker::startStatusMonitoringStream
 */
void SensorWorker::startStatusMonitoringStream(){

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
    this->isStatusStreamStarted = false;
}

/*!
 * \brief SensorWorker::streamReading
 */
void SensorWorker::streamReading(){

    //check streaming status
    if(!this->isReadingStreamStarted){
        return;
    }

    //check sensor
    if(this->sensor.isNull()){
        this->isReadingStreamStarted = false;
        return;
    }

    if(!this->sensor->isSensorAsync()){
        //get real time reading
        QVariantMap reading = this->sensor->readingStream(this->streamFormat);
        emit this->realTimeReading(reading);

        //put reading stream into event queue again
        QMetaObject::invokeMethod(this, "streamReading", Qt::QueuedConnection);
    }else{
        QJsonObject request;
        request.insert("method", "stream");
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
        }
    }


}

/*!
 * \brief SensorWorker::monitorConnectionStatus
 */
void SensorWorker::monitorConnectionStatus(){

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

void SensorWorker::asyncSensorResponseReceived(const QJsonObject &response)
{
    bool success = false;
    QString msg = "";
    if(response.value("error") != QJsonValue::Undefined){
        msg = response.value("error").toObject().value("message").toString();
    } else {
        msg = response.value("result").toString();
        success = true;
    }
    emit this->commandFinished(success, msg);
}

void SensorWorker::asyncSensorMeasurementReceived(const int &geomId, const QList<QPointer<Reading> > &measurements)
{
    emit this->measurementDone(true);

    if(measurements.size() > 0) {
        emit this->commandFinished(true, "measurement data received");

    }else {
        emit this->commandFinished(true, "measurement did not deliver any results");
    }

    emit this->measurementFinished(geomId, measurements);

}

void SensorWorker::asyncSensorStreamDataReceived(const QVariantMap &reading)
{
    emit this->realTimeReading(reading);

    //put reading stream into event queue again
    QMetaObject::invokeMethod(this, "streamReading", Qt::QueuedConnection);
}


void SensorWorker::finishMeasurement(){
    qDebug() << "SensorWorker::finishMeasurement()";

    //check sensor
    if(this->sensor.isNull()){
        emit this->commandFinished(false, "no sensor instance");
        return;
    }

    //check wether the sensor is already connected
    QString msg = "failed to finish";
    bool success = false;

    if(!this->sensor->isSensorAsync()){
        if(!this->sensor->getConnectionState()){
            msg = "sensor is not connected";

        }else if(this->sensor->doSelfDefinedAction("stopMeasure")) {
                msg = "finish measurement called";
                success = true;
        }

        emit this->commandFinished(success, msg);
    }else{
        QJsonObject request;
        request.insert("method", "stopMeasure");
        QJsonObject status = this->sensor->performAsyncSensorCommand(request);
        if(status.value("status").toString().compare("blocked") == 0) {
            emit this->commandFinished(false, "connection was blocked - please try again");
        }
    }

}
