#include "sensorfacade.h"

using namespace oi;

SensorFacade::SensorFacade(const QPointer<Sensor> sensor, QObject *parent) : Sensor(parent)
{
    this->inner = sensor;
}
/*
SensorFacade::SensorFacade(const SensorFacade &copy, QObject *parent){
    this->inner = copy.inner;
}*/

SensorFacade &SensorFacade::operator=(const SensorFacade &copy){
    this->inner = copy.inner;

    return *this;
}

SensorFacade::~SensorFacade() {

}

void SensorFacade::init() {
    this->inner->init();

    connect(this, &Sensor::sensorStatus, inner, &Sensor::sensorStatus);
    connect(this, &Sensor::sensorMessage, inner, &Sensor::sensorMessage);
    connect(this, &Sensor::asyncSensorResponse, inner, &Sensor::asyncSensorResponse);
    connect(this, &Sensor::asyncMeasurementResult, inner, &Sensor::asyncMeasurementResult);
    connect(this, &Sensor::asyncStreamResult, inner, &Sensor::asyncStreamResult);
    connect(this, &Sensor::asyncSensorNotification, inner, &Sensor::asyncSensorNotification);

}

//####################################
//get or set general sensor attributes
//####################################

const SensorConfiguration &SensorFacade::getSensorConfiguration() const{
    return this->inner->getSensorConfiguration();
}
void SensorFacade::setSensorConfiguration(const SensorConfiguration &sConfig){
    this->inner->setSensorConfiguration(sConfig);
}

void SensorFacade::setMeasurementConfig(const MeasurementConfig &mConfig){
    this->inner->setMeasurementConfig(mConfig);
}

const QPair<ReadingTypes, QPointer<Reading> > &SensorFacade::getLastReading() const{
    return this->inner->getLastReading();
}

bool SensorFacade::isSensorAsync() const{
    return this->inner->isSensorAsync();
}

//#########################################################
//methods to get or set further information to use a sensor
//#########################################################

//reading types, sensor actions and connection types
const QList<ReadingTypes> &SensorFacade::getSupportedReadingTypes() const{
    return this->inner->getSupportedReadingTypes();
}
const QList<SensorFunctions> &SensorFacade::getSupportedSensorActions() const{
    return this->inner->getSupportedSensorActions();
}
const QList<ConnectionTypes> &SensorFacade::getSupportedConnectionTypes() const{
    return this->inner->getSupportedConnectionTypes();
}

//meta data
const PluginMetaData &SensorFacade::getMetaData() const{
    return this->inner->getMetaData();
}

//integer, double and string parameter
const QMap<QString, int> &SensorFacade::getIntegerParameter() const{
    return this->inner->getIntegerParameter();
}
const QMap<QString, double> &SensorFacade::getDoubleParameter() const{
    return this->inner->getDoubleParameter();
}
const QMultiMap<QString, QString> &SensorFacade::getStringParameter() const{
    return this->inner->getStringParameter();
}

//self deined actions
const QStringList &SensorFacade::getSelfDefinedActions() const{
    return this->inner->getSelfDefinedActions();
}

//accuracy
const Accuracy &SensorFacade::getDefaultAccuracy() const{
    return this->inner->getDefaultAccuracy();
}

//########################
//sensor state and actions
//########################

//sensor actions
bool SensorFacade::accept(const SensorFunctions &method, const SensorAttributes &sAttr){
    return this->inner->accept(method, sAttr);
}

QJsonObject SensorFacade::performAsyncSensorCommand(const QJsonObject &request){
    return this->inner->performAsyncSensorCommand(request);
}

//abort actions
bool SensorFacade::abortAction(){
    return this->inner->abortAction();
}

//connect or disconnect
bool SensorFacade::connectSensor(){
    return this->inner->connectSensor();
}
bool SensorFacade::disconnectSensor(){
    return this->inner->disconnectSensor();
}

//measurements
QList<QPointer<Reading> > SensorFacade::measure(const MeasurementConfig &mConfig){
    return this->inner->measure(mConfig);
}
QVariantMap SensorFacade::readingStream(const ReadingTypes &streamFormat){
    return this->inner->readingStream(streamFormat);
}

//status information
bool SensorFacade::getConnectionState(){
    return this->inner->getConnectionState();
}
bool SensorFacade::getIsReadyForMeasurement(){
    return this->inner->getIsReadyForMeasurement();
}
bool SensorFacade::getIsBusy(){
    return this->inner->getIsBusy();
}
QMap<QString, QString> SensorFacade::getSensorStatus(){
    return this->inner->getSensorStatus();
}

//self defined actions
bool SensorFacade::doSelfDefinedAction(const QString &action){
    return this->inner->doSelfDefinedAction(action);
}

bool SensorFacade::search(){
    return this->inner->search();
}

//#################
//save and load XML
//#################

QDomElement SensorFacade::toOpenIndyXML(QDomDocument &xmlDoc) const{
    return this->inner->toOpenIndyXML(xmlDoc);
}
bool SensorFacade::fromOpenIndyXML(QDomElement &xmlElem){
    return this->inner->fromOpenIndyXML(xmlElem);
}



