#include "measurementconfig.h"

using namespace oi;

/*!
 * \brief MeasurementConfig::MeasurementConfig
 */
MeasurementConfig::MeasurementConfig() : configType(eUndefinded), editable(true) {

    //set defaults
    this->measurementType = eSinglePoint_MeasurementType;
    this->measurementMode = eFast_MeasurementMode;
    this->measureTwoSides = false;
    this->maxObservations = 500;
    this->timeInterval = 0.0;
    this->distanceInterval = 0.0;
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

    this->configType = copy.configType;
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

    this->configType = copy.configType;
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

/* key for maps */
const Key MeasurementConfig::getKey() const{
    return Key(this->name, this->configType);
}

const bool MeasurementConfig::isUserConfig() const{
    return eUserConfig == this->configType;
}
const bool MeasurementConfig::isProjectConfig() const{
    return eProjectConfig == this->configType || eStandardConfig == this->configType;
}
const bool MeasurementConfig::isStandardConfig() const {
    return eStandardConfig == this->configType;
}
const bool MeasurementConfig::isEditable() const {
    return this->editable;
}
const bool MeasurementConfig::isValid() const{
    return !this->name.isEmpty() && this->configType != eUndefinded;
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
const double &MeasurementConfig::getTimeInterval() const{
    return this->timeInterval;
}

/*!
 * \brief MeasurementConfig::setTimeInterval
 * \param interval
 */
void MeasurementConfig::setTimeInterval(const double &interval){
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
    mConfig.setAttribute("timeInterval", this->timeInterval);
    mConfig.setAttribute("distanceInterval", this->distanceInterval);

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
    this->timeInterval = xmlElem.attribute("timeInterval").toDouble();
    this->distanceInterval = xmlElem.attribute("distanceInterval").toDouble();

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

void MeasurementConfig::makeUserConfig() {
    this->editable = true;
    this->configType = eUserConfig;
}
void MeasurementConfig::makeProjectConfig() {
    this->editable = true;
    this->configType = eProjectConfig;
}
void MeasurementConfig::makeStandardConfig() {
    this->editable = false;
    this->configType = eStandardConfig;
}
