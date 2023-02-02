#include "measurementconfig.h"

using namespace oi;

/*!
 * \brief MeasurementConfig::MeasurementConfig
 */
MeasurementConfig::MeasurementConfig() : userConfig(false), editable(false){

    //set defaults
    this->measurementType = eSinglePoint_MeasurementType;
    this->measurementMode = eFast_MeasurementMode;
    this->measureTwoSides = false;
    this->maxObservations = 500;
    this->timeInterval = 0;
    this->distanceInterval = 0.0;

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
    this->measurementType = copy.measurementType;
    this->measurementMode = copy.measurementMode;
    this->measureTwoSides = copy.measureTwoSides;
    this->maxObservations = copy.maxObservations;
    this->timeInterval = copy.timeInterval;
    this->distanceInterval = copy.distanceInterval;

    this->isStablePoint = copy.isStablePoint;
    this->stablePointMinDistance = copy.stablePointMinDistance;
    this->stablePointThresholdRange = copy.stablePointThresholdRange;
    this->stablePointThresholdTime = copy.stablePointThresholdTime;

    this->userConfig = copy.userConfig;
    this->editable = copy.editable;

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
    this->measurementType = copy.measurementType;
    this->measurementMode = copy.measurementMode;
    this->measureTwoSides = copy.measureTwoSides;
    this->maxObservations = copy.maxObservations;
    this->timeInterval = copy.timeInterval;
    this->distanceInterval = copy.distanceInterval;

    this->isStablePoint = copy.isStablePoint;
    this->stablePointMinDistance = copy.stablePointMinDistance;
    this->stablePointThresholdRange = copy.stablePointThresholdRange;
    this->stablePointThresholdTime = copy.stablePointThresholdTime;

    this->userConfig = copy.userConfig;
    this->editable = copy.editable;

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
}

const bool &MeasurementConfig::isUserConfig() const{
    return this->userConfig;
}
const bool &MeasurementConfig::isProjectConfig() const{
    return !this->userConfig;
}
void MeasurementConfig::isUserConfig(const bool &isUserConfig){
    this->userConfig = isUserConfig;
}

const bool &MeasurementConfig::isEditable() const {
    return this->editable;
}

void MeasurementConfig::isEditable(const bool &isEditable) {
    this->editable = isEditable;
}

/*!
 * \brief MeasurementConfig::getIsValid
 * \return
 */
bool MeasurementConfig::getIsValid() const{
    return !this->name.isEmpty();
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
    this->timeInterval = interval;
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
}

void MeasurementConfig::setIsStablePoint(const bool isStablePoint) {
    this->isStablePoint = isStablePoint;
}

const bool &MeasurementConfig::getIsStablePoint() const {
    return this->isStablePoint;
}

void MeasurementConfig::setStablePointMinDistance(const double &minDistance) {
    this->stablePointMinDistance = minDistance;
}

const double &MeasurementConfig::getStablePointMinDistance() const {
    return this->stablePointMinDistance;
}

void MeasurementConfig::setStablePointThresholdRange(const double &threshold) {
    this->stablePointThresholdRange = threshold;
}

const double &MeasurementConfig::getStablePointThresholdRange() const {
    return this->stablePointThresholdRange;
}

void MeasurementConfig::setStablePointThresholdTime(const double &threshold) {
    this->stablePointThresholdTime = threshold;
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

    mConfig.setAttribute("measurementMode", this->measurementMode);
    mConfig.setAttribute("measurementType", this->measurementType);
    mConfig.setAttribute("measureTwoSides", this->measureTwoSides);
    mConfig.setAttribute("maxObservations", this->maxObservations);
    mConfig.setAttribute("timeInterval", QString::number(this->timeInterval));
    mConfig.setAttribute("distanceInterval", this->distanceInterval);

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

    if(!xmlElem.hasAttribute("name")
            || !xmlElem.hasAttribute("measurementMode")
            || !xmlElem.hasAttribute("measurementType")
            || !xmlElem.hasAttribute("measureTwoSides")
            || !xmlElem.hasAttribute("maxObservations")
            || !xmlElem.hasAttribute("timeInterval")
            || !xmlElem.hasAttribute("distanceInterval")
            ){
        return false;
    }

    this->name = xmlElem.attribute("name");
    this->measurementMode = (MeasurementModes)xmlElem.attribute("measurementMode").toInt();
    this->measurementType = (MeasurementTypes)xmlElem.attribute("measurementType").toInt();
    this->measureTwoSides = xmlElem.attribute("measureTwoSides").toInt();
    this->maxObservations = xmlElem.attribute("maxObservations").toInt();
    this->timeInterval = xmlElem.attribute("timeInterval").toLong();
    this->distanceInterval = xmlElem.attribute("distanceInterval").toDouble();

    this->isStablePoint = xmlElem.attribute("isStablePoint").toInt();
    this->stablePointMinDistance = xmlElem.attribute("stablePointMinDistance").toDouble();
    this->stablePointThresholdRange = xmlElem.attribute("stablePointThresholdRange").toDouble();
    this->stablePointThresholdTime = xmlElem.attribute("stablePointThresholdTime").toDouble();

    return true;

}

const QVariant MeasurementConfig::getTransientData(const QString key) const {
    return this->transientData.value(key);
}
void MeasurementConfig::setTransientData(const QString key, const QVariant value) {
    this->transientData.insert(key, value);
}

/**
 * @brief MeasurementConfig::applicableFor
 * @param elementType
 * @param typeOfFeature
 * @return true if elementType and typeOfFeature can handled by this MeasurementConfig
 */
bool MeasurementConfig::applicableFor(const ElementTypes elementType, QList<FeatureTypes> functionIsApplicableFor) {
    if(ElementTypes::eUndefinedElement == elementType) { // used to filter all configs: CreateFeatureDialog::initMeasurementConfigUI
        return false;
    }

    // handle level ->
    if(functionIsApplicableFor.contains(FeatureTypes::eLevelFeature)) {
        return this->measurementType == MeasurementTypes::eLevel_MeasurementType;
    }
    if(this->measurementType == MeasurementTypes::eLevel_MeasurementType) {
        return false;
    }
    // <- handle level

    return true;
}

void MeasurementConfig::setMeasurementMode(const MeasurementModes mode) {
    this->measurementMode = mode;

}

const MeasurementModes MeasurementConfig::getMeasurementMode() const {
    return this->measurementMode;
}

void MeasurementConfig::setMeasurementType(const MeasurementTypes type) {
    this->measurementType = type;

}

const MeasurementTypes MeasurementConfig::getMeasurementType() const {
    return this->measurementType;
}

const int &MeasurementConfig::getMaxObservations() const {
    return this->maxObservations;
}
void MeasurementConfig::setMaxObservations(const int &maxObservations) {
    this->maxObservations = maxObservations;

}

