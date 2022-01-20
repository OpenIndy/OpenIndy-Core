#include "measurementconfig.h"

using namespace oi;

/*!
 * \brief MeasurementConfig::MeasurementConfig
 */
MeasurementConfig::MeasurementConfig() : isSaved(false){

    //set defaults
    this->count = 500;
    this->iterations = 1;
    this->measureTwoSides = false;
    this->timeDependent = false;
    this->distanceDependent = false;
    this->timeInterval = 0;
    this->distanceInterval = 0.0;
    this->typeOfReading = ePolarReading;
    this->measureWithAim = false;

    this->isStablePoint = false;
    this->stablePointMinDistance = 10.0;
    this->stablePointThresholdRange = 0.1;
    this->stablePointThresholdTime = 2.0;

}

/*!
 * \brief MeasurementConfig::MeasurementConfig
 * \param copy
 */
MeasurementConfig::MeasurementConfig(const MeasurementConfig &copy){

    //copy measurement config attributes
    this->name = copy.name;
    this->isSaved = copy.isSaved;
    this->count = copy.count;
    this->iterations = copy.iterations;
    this->measureTwoSides = copy.measureTwoSides;
    this->timeDependent = copy.timeDependent;
    this->distanceDependent = copy.distanceDependent;
    this->timeInterval = copy.timeInterval;
    this->distanceInterval = copy.distanceInterval;
    this->typeOfReading = copy.typeOfReading;
    this->measureWithAim = copy.measureWithAim;

    this->isStablePoint = copy.isStablePoint;
    this->stablePointMinDistance = copy.stablePointMinDistance;
    this->stablePointThresholdRange = copy.stablePointThresholdRange;
    this->stablePointThresholdTime = copy.stablePointThresholdTime;

    this->transientData = copy.transientData;
    this->transientData.detach();
}

/*!
 * \brief MeasurementConfig::operator =
 * \param copy
 * \return
 */
MeasurementConfig &MeasurementConfig::operator=(const MeasurementConfig &copy){

    //copy measurement config attributes
    this->name = copy.name;
    this->isSaved = copy.isSaved;
    this->count = copy.count;
    this->iterations = copy.iterations;
    this->measureTwoSides = copy.measureTwoSides;
    this->timeDependent = copy.timeDependent;
    this->distanceDependent = copy.distanceDependent;
    this->timeInterval = copy.timeInterval;
    this->distanceInterval = copy.distanceInterval;
    this->typeOfReading = copy.typeOfReading;
    this->measureWithAim = copy.measureWithAim;
    this->isStablePoint = copy.isStablePoint;
    this->stablePointMinDistance = copy.stablePointMinDistance;
    this->stablePointThresholdRange = copy.stablePointThresholdRange;
    this->stablePointThresholdTime = copy.stablePointThresholdTime;

    this->transientData = copy.transientData;
    this->transientData.detach();

    return *this;

}

/*!
 * \brief MeasurementConfig::getName
 * \return
 */
const QString &MeasurementConfig::getName() const{
    return this->name;
}

/*!
 * \brief MeasurementConfig::setName
 * \param name
 */
void MeasurementConfig::setName(const QString &name){
    this->name = name;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getIsSaved
 * \return
 */
const bool &MeasurementConfig::getIsSaved() const{
    return this->isSaved;
}

/*!
 * \brief MeasurementConfig::setIsSaved
 * \param isSaved
 */
void MeasurementConfig::setIsSaved(const bool &isSaved){
    this->isSaved = isSaved;
}

/*!
 * \brief MeasurementConfig::getIsValid
 * \return
 */
bool MeasurementConfig::getIsValid() const{
    if(this->name.compare("") != 0){
        return true;
    }
    return false;
}

/*!
 * \brief MeasurementConfig::getCount
 * \return
 */
const int &MeasurementConfig::getCount() const{
    return this->count;
}

/*!
 * \brief MeasurementConfig::setCount
 * \param count
 */
void MeasurementConfig::setCount(const int &count){
    this->count = count;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getIterations
 * \return
 */
const int &MeasurementConfig::getIterations() const{
    return this->iterations;
}

/*!
 * \brief MeasurementConfig::setIterations
 * \param iterations
 */
void MeasurementConfig::setIterations(const int &iterations){
    this->iterations = iterations;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getMeasureTwoSides
 * \return
 */
const bool &MeasurementConfig::getMeasureTwoSides() const{
    return this->measureTwoSides;
}

/*!
 * \brief MeasurementConfig::setMeasureTwoSides
 * \param twoSides
 */
void MeasurementConfig::setMeasureTwoSides(const bool &measureTwoSides){
    this->measureTwoSides = measureTwoSides;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getMeasureWithAim
 * \return
 */
const bool &MeasurementConfig::getMeasureWithAim() const{
    return this->measureWithAim;
}

/*!
 * \brief MeasurementConfig::setMeasureWithAim
 * \param twoSides
 */
void MeasurementConfig::setMeasureWithAim(const bool &measureWithAim){
    this->measureWithAim = measureWithAim;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getTimeDependent
 * \return
 */
const bool &MeasurementConfig::getTimeDependent() const{
    return this->timeDependent;
}

/*!
 * \brief MeasurementConfig::setTimeDependent
 * \param timeDependent
 */
void MeasurementConfig::setTimeDependent(const bool &timeDependent){
    this->timeDependent = timeDependent;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getDistanceDependent
 * \return
 */
const bool &MeasurementConfig::getDistanceDependent() const{
    return this->distanceDependent;
}

/*!
 * \brief MeasurementConfig::setDistanceDependent
 * \param distanceDependent
 */
void MeasurementConfig::setDistanceDependent(const bool &distanceDependent){
    this->distanceDependent = distanceDependent;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getTimeInterval
 * \return
 */
const long &MeasurementConfig::getTimeInterval() const{
    return this->timeInterval;
}

/*!
 * \brief MeasurementConfig::setTimeInterval
 * \param interval
 */
void MeasurementConfig::setTimeInterval(const long &interval){
    this->timeInterval = timeInterval;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getDistanceInterval
 * \return interval [mm]
 */
const double &MeasurementConfig::getDistanceInterval() const{
    return this->distanceInterval;
}

/*!
 * \brief MeasurementConfig::setDistanceInterval
 * \param interval [mm]
 */
void MeasurementConfig::setDistanceInterval(const double &interval){
    this->distanceInterval = interval;
    this->isSaved = false;
}

/*!
 * \brief MeasurementConfig::getTypeOfReading
 * \return
 */
const ReadingTypes &MeasurementConfig::getTypeOfReading() const{
    return this->typeOfReading;
}

/*!
 * \brief MeasurementConfig::setTypeOfReading
 * \param type
 */
void MeasurementConfig::setTypeOfReading(const ReadingTypes &type){
    this->typeOfReading = type;
    this->isSaved = false;
}


void MeasurementConfig::setIsStablePoint(const bool isStablePoint) {
    this->isStablePoint = isStablePoint;
    this->isSaved = false;
}

const bool &MeasurementConfig::getIsStablePoint() const {
    return this->isStablePoint;
}

void MeasurementConfig::setStablePointMinDistance(const double &minDistance) {
    this->stablePointMinDistance = minDistance;
    this->isSaved = false;
}

const double &MeasurementConfig::getStablePointMinDistance() const {
    return this->stablePointMinDistance;
}

void MeasurementConfig::setStablePointThresholdRange(const double &threshold) {
    this->stablePointThresholdRange = threshold;
    this->isSaved = false;
}

const double &MeasurementConfig::getStablePointThresholdRange() const {
    return this->stablePointThresholdRange;
}

void MeasurementConfig::setStablePointThresholdTime(const double &threshold) {
    this->stablePointThresholdTime = threshold;
    this->isSaved = false;
}

const double &MeasurementConfig::getStablePointThresholdTime() const {
    return this->stablePointThresholdTime;
}

/*!
 * \brief MeasurementConfig::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement MeasurementConfig::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement mConfig = xmlDoc.createElement("measurementConfig");

    //set measurement config attributes
    mConfig.setAttribute("name", this->name);
    mConfig.setAttribute("isSaved", this->isSaved);
    mConfig.setAttribute("count", this->count);
    mConfig.setAttribute("iterations", this->iterations);
    mConfig.setAttribute("measureTwoSides", this->measureTwoSides);
    mConfig.setAttribute("timeDependent", this->timeDependent);
    mConfig.setAttribute("distanceDependent", this->distanceDependent);
    mConfig.setAttribute("timeInterval", QString::number(this->timeInterval));
    mConfig.setAttribute("distanceInterval", this->distanceInterval);
    mConfig.setAttribute("typeOfReading", this->typeOfReading);
    mConfig.setAttribute("measureWithAim", this->measureWithAim);

    mConfig.setAttribute("isStablePoint", this->isStablePoint);
    mConfig.setAttribute("stablePointMinDistance", this->stablePointMinDistance);
    mConfig.setAttribute("stablePointThresholdRange", this->stablePointThresholdRange);
    mConfig.setAttribute("stablePointThresholdTime", this->stablePointThresholdTime);

    return mConfig;

}

/*!
 * \brief MeasurementConfig::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool MeasurementConfig::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("count") || !xmlElem.hasAttribute("iterations")
            || !xmlElem.hasAttribute("measureTwoSides") || !xmlElem.hasAttribute("timeDependent")
            || !xmlElem.hasAttribute("distanceDependent") || !xmlElem.hasAttribute("timeInterval")
            || !xmlElem.hasAttribute("distanceInterval") || !xmlElem.hasAttribute("typeOfReading")){
        return false;
    }

    this->name = xmlElem.attribute("name");
    this->count = xmlElem.attribute("count").toInt();
    this->iterations = xmlElem.attribute("iterations").toInt();
    this->measureTwoSides = xmlElem.attribute("measureTwoSides").toInt();
    this->timeDependent = xmlElem.attribute("timeDependent").toInt();
    this->distanceDependent = xmlElem.attribute("distanceDependent").toInt();
    this->timeInterval = xmlElem.attribute("timeInterval").toLong();
    this->distanceInterval = xmlElem.attribute("distanceInterval").toDouble();
    this->typeOfReading = (ReadingTypes)xmlElem.attribute("typeOfReading").toInt();
    this->measureWithAim = xmlElem.attribute("measureWithAim").toInt();

    this->isStablePoint = xmlElem.attribute("isStablePoint").toInt();
    this->stablePointMinDistance = xmlElem.attribute("stablePointMinDistance").toDouble();
    this->stablePointThresholdRange = xmlElem.attribute("stablePointThresholdRange").toDouble();
    this->stablePointThresholdTime = xmlElem.attribute("stablePointThresholdTime").toDouble();

    if(xmlElem.hasAttribute("isSaved")){
        this->isSaved = xmlElem.attribute("isSaved").toInt();
    }

    return true;

}

const QVariant MeasurementConfig::getTransientData(const QString key) const {
    return this->transientData.value(key);
}
void MeasurementConfig::setTransientData(const QString key, const QVariant value) {
    this->transientData.insert(key, value);
}
