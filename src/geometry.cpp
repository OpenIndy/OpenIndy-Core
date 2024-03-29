#include "geometry.h"

#include "oijob.h"
#include "coordinatesystem.h"
#include "observation.h"
#include "station.h"
#include "function.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Geometry::Geometry
 * \param isNominal
 * \param parent
 */
Geometry::Geometry(const bool &isNominal, QObject *parent) : Feature(parent), isNominal(isNominal), isCommon(false){

}

/*!
 * \brief Geometry::Geometry
 * \param copy
 * \param parent
 */
Geometry::Geometry(const Geometry &copy, QObject *parent) : Feature(copy, parent){

    //copy attributes
    this->isNominal = copy.isNominal;
    this->isCommon = copy.isCommon;
    this->statistic = copy.statistic;
    this->simulationData = copy.simulationData;
    this->activeMeasurementConfig = copy.activeMeasurementConfig;

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->radius = copy.radius;

    //copy nominals, actual, observations
    //this->nominals = copy.nominals;
    //this->actual = copy.actual;
    //this->observations = copy.observations;
    //this->nominalSystem = copy.nominalSystem;

}

/*!
 * \brief Geometry::operator =
 * \param copy
 * \return
 */
Geometry &Geometry::operator=(const Geometry &copy){

    //copy attributes
    this->isNominal = copy.isNominal;
    this->isCommon = copy.isCommon;
    this->statistic = copy.statistic;
    this->simulationData = copy.simulationData;
    this->activeMeasurementConfig = copy.activeMeasurementConfig;

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->radius = copy.radius;

    //copy nominals, actual, observations
    //this->nominals = copy.nominals;
    //this->actual = copy.actual;
    //this->observations = copy.observations;
    //this->nominalSystem = copy.nominalSystem;

    return *this;

}

/*!
 * \brief Geometry::~Geometry
 */
Geometry::~Geometry(){

    if(this->isNominal){

        //delete this geometry from the nominal list of its actual
        if(!this->actual.isNull()){
            this->actual->removeNominal(this);
        }

        //delete this geometry from list of nominals in nominal system
        if(!this->nominalSystem.isNull()){
            this->nominalSystem->removeNominal(this->selfFeature);
        }

    }else{

        //delete this geometry from target geometries list of the observations
        foreach(const QPointer<Observation> &observation, this->observations){
            if(!observation.isNull()){
                observation->removeTargetGeometry(this->id);
            }
        }

    }

}

/*!
 * \brief Geometry::getIsCommon
 * \return
 */
const bool &Geometry::getIsCommon() const{
    return this->isCommon;
}

/*!
 * \brief Geometry::setCommonState
 * \param isCommon
 */
void Geometry::setCommonState(const bool &isCommon){
    if(isCommon != this->isCommon){
        this->isCommon = isCommon;
        emit this->geomIsCommonChanged(this->id);
    }
}

/*!
 * \brief Geometry::getIsNominal
 * \return
 */
const bool &Geometry::getIsNominal() const{
    return this->isNominal;
}

/*!
 * \brief Geometry::getNominals
 * \return
 */
const QList<QPointer<Geometry> > &Geometry::getNominals() const{
    return this->nominals;
}

/*!
 * \brief Geometry::addNominal
 * \param nominal
 * \return
 */
bool Geometry::addNominal(const QPointer<Geometry> &nominal){

    if(!nominal.isNull() && nominal->getIsNominal() && !this->getIsNominal()){

        //compare feature types
        if(this->selfFeature.isNull() || nominal->selfFeature.isNull() || this->selfFeature->getFeatureTypeEnum() != nominal->selfFeature->getFeatureTypeEnum()){
            return false;
        }

        //check if the nominal is in the same job
        if(!this->job.isNull()){
            QPointer<FeatureWrapper> jobNominal = this->job->getFeatureById(nominal->getId());
            if(jobNominal.isNull() || jobNominal->getGeometry().isNull() || jobNominal->getGeometry() != nominal){
                return false;
            }
        }

        //check if this geometry already contains the nominal
        if(!this->nominals.contains(nominal)){
            this->nominals.append(nominal);
            nominal->setActual(this);
            emit this->geomNominalsChanged(this->id);
            return true;
        }

    }

    return false;

}

/*!
 * \brief Geometry::removeNominal
 * \param nominal
 * \return
 */
bool Geometry::removeNominal(const QPointer<Geometry> &nominal){

    //check if the nominal is valid
    if(nominal.isNull()){
        return false;
    }

    //remove nominal
    return this->nominals.removeOne(nominal);

}

/*!
 * \brief Geometry::getActual
 * \return
 */
const QPointer<Geometry> &Geometry::getActual() const{
    return this->actual;
}

/*!
 * \brief Geometry::setActual
 * \param actual
 * \return
 */
bool Geometry::setActual(const QPointer<Geometry> &actual){

    //check if this is a nominal
    if(!this->isNominal){
        return false;
    }

    //check actual
    if(actual.isNull() || (!this->actual.isNull() && this->actual->getId() == actual->getId())
            || actual->getIsNominal()){
        return false;
    }

    //compare feature types
    if(this->selfFeature.isNull() || actual->selfFeature.isNull() || this->selfFeature->getFeatureTypeEnum() != actual->selfFeature->getFeatureTypeEnum()){
        return false;
    }

    //check if actual is in the same job
    if(!this->job.isNull()){
        QPointer<FeatureWrapper> jobActual = this->job->getFeatureById(actual->getId());
        if(jobActual.isNull() || jobActual->getGeometry().isNull() || jobActual->getGeometry() != actual){
            return false;
        }
    }

    this->actual = actual;
    actual->addNominal(this);
    emit this->geomActualChanged(this->id);

    return true;

}

/*!
 * \brief Geometry::getObservations
 * \return
 */
const QList<QPointer<Observation> > &Geometry::getObservations() const{
    return this->observations;
}

/*!
 * \brief Geometry::addObservation
 * \param obs
 */
void Geometry::addObservation(const QPointer<Observation> &obs){

    if(!obs.isNull() && !this->isNominal){

        //check if obs is already included
        if(this->observations.contains(obs)){
            return;
        }

        this->observations.append(obs);

        //update target geometries
        obs->addTargetGeometry(this);

        //update used reading types
        if(!obs->getReading().isNull() && !this->usedReadingTypes.contains(obs->getReading()->getTypeOfReading())){
            this->usedReadingTypes.append(obs->getReading()->getTypeOfReading());
        }

        this->isUpdated = false;

        emit this->geomObservationsChanged(this->id);

    }

}

/*!
 * \brief Geometry::removeObservation
 * \param obs
 */
void Geometry::removeObservation(const QPointer<Observation> &obs){

    //check if obs is valid
    if(obs.isNull()){
        return;
    }

    //remove observation
    if(this->observations.removeOne(obs)){

        //update target geometries
        obs->removeTargetGeometry(this);

        //update used reading types
        if(!obs->getReading().isNull() && this->usedReadingTypes.contains(obs->getReading()->getTypeOfReading())){
            this->usedReadingTypes.removeOne(obs->getReading()->getTypeOfReading());
        }

        this->isUpdated = false;

        emit this->geomObservationsChanged(this->id);

    }

}

/*!
 * \brief Geometry::getNominalSystem
 * \return
 */
const QPointer<CoordinateSystem> &Geometry::getNominalSystem() const{
    return this->nominalSystem;
}

/*!
 * \brief Geometry::setNominalSystem
 * \param nomSys
 * \return
 */
bool Geometry::setNominalSystem(const QPointer<CoordinateSystem> &nomSys){

    if(!nomSys.isNull() && this->isNominal && this->nominalSystem != nomSys){

        //check if nomSys is in the same job
        if(!this->job.isNull()){
            QPointer<FeatureWrapper> jobSystem = this->job->getFeatureById(nomSys->getId());
            if(jobSystem.isNull() || jobSystem->getCoordinateSystem().isNull() || jobSystem->getCoordinateSystem() != nomSys){
                return false;
            }
        }

        this->nominalSystem = nomSys;

        nomSys->addNominal(this->selfFeature);

        emit this->geomNominalSystemChanged(this->id);

        return true;

    }

    return false;

}

/*!
 * \brief Geometry::getMeasurementConfig
 * \return
 */
const MeasurementConfigKey &Geometry::getMeasurementConfig() const{
    return this->activeMeasurementConfig;
}

/*!
 * \brief Geometry::setMeasurementConfig
 * \param myConfig
 */
void Geometry::setMeasurementConfig(const MeasurementConfigKey &myConfig){
    QString oldName = this->activeMeasurementConfig.getName();
    MeasurementConfigKey oldKey = this->activeMeasurementConfig;
    this->activeMeasurementConfig = myConfig;
    emit this->geomMeasurementConfigChanged(this->id, oldName, oldKey);
}

/*!
 * \brief Geometry::getStatistic
 * \return
 */
const Statistic &Geometry::getStatistic() const{
    return this->statistic;
}

/*!
 * \brief Geometry::setStatistic
 * \param myStatistic
 */
void Geometry::setStatistic(const Statistic &myStatistic){
    this->statistic = myStatistic;
}

/*!
 * \brief Geometry::getSimulationData
 * \return
 */
const SimulationData &Geometry::getSimulationData(){
    return this->simulationData;
}

/*!
 * \brief Geometry::setSimulationData
 * \param s
 */
void Geometry::setSimulationData(const SimulationData &s){
    this->simulationData = s;
}

/*!
 * \brief Geometry::getUsedReadingTypes
 * \return
 */
const QList<ReadingTypes> &Geometry::getUsedReadingTypes() const{
    return this->usedReadingTypes;
}

/*!
 * \brief Geometry::hasDirection
 * \return
 */
bool Geometry::hasDirection() const{
    return false;
}

/*!
 * \brief Geometry::hasPosition
 * \return
 */
bool Geometry::hasPosition() const{
    return false;
}

/*!
 * \brief Geometry::hasRadius
 * \return
 */
bool Geometry::hasRadius() const{
    return false;
}

/*!
 * \brief Geometry::getRadius
 * \return
 */
const Radius &Geometry::getRadius() const{
    return this->radius;
}

/*!
 * \brief Geometry::getDirection
 * \return
 */
const Direction &Geometry::getDirection() const{
    return this->ijk;
}

void Geometry::setDirection(Direction &ijk) {
    this->ijk = ijk;
    emit this->geomParametersChanged(this->id);
}


/*!
 * \brief Geometry::getPosition
 * \return
 */
const Position &Geometry::getPosition() const{
    return this->xyz;
}

void Geometry::setPosition(Position &xyz) {
    this->xyz = xyz;
    emit this->geomParametersChanged(this->id);
}
/*!
 * \brief Geometry::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Geometry::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{
    return QMap<GeometryParameters, QString>();
}

/*!
 * \brief Geometry::setUnknownParameters
 * \param parameters
 */
void Geometry::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){
    return;
}

/*!
 * \brief Geometry::recalc
 */
void Geometry::recalc(){

    Feature::recalc();

    //if the geometry is a nominal without functions and its system is the active system set isSolved to true
    if(this->functionList.size() == 0 && this->isNominal && !this->nominalSystem.isNull() && this->nominalSystem->getIsActiveCoordinateSystem()){
        this->isSolved = true;
    }

    //reset statistic if not solved
    if(!this->isSolved){
        this->statistic.reset();
    }

}

/*!
 * \brief Geometry::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Geometry::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement geometry = Feature::toOpenIndyXML(xmlDoc);

    if(geometry.isNull()){
        return geometry;
    }

    geometry.setTagName("geometry");

    //set geometry attributes
    geometry.setAttribute("nominal", this->getIsNominal());
    geometry.setAttribute("common", this->getIsCommon());

    //add position
    if(this->hasPosition()){
        QDomElement coordinates = xmlDoc.createElement("coordinates");
        if(this->isSolved || this->isNominal){
            coordinates.setAttribute("x", this->getPosition().getVector().getAt(0));
            coordinates.setAttribute("y", this->getPosition().getVector().getAt(1));
            coordinates.setAttribute("z", this->getPosition().getVector().getAt(2));
        }else{
            coordinates.setAttribute("x", 0.0);
            coordinates.setAttribute("y", 0.0);
            coordinates.setAttribute("z", 0.0);
        }
        geometry.appendChild(coordinates);
    }

    //add direction
    if(this->hasDirection()){
        QDomElement coordinates = xmlDoc.createElement("spatialDirection");
        if(this->isSolved || this->isNominal){
            coordinates.setAttribute("i", this->getDirection().getVector().getAt(0));
            coordinates.setAttribute("j", this->getDirection().getVector().getAt(1));
            coordinates.setAttribute("k", this->getDirection().getVector().getAt(2));
        }else{
            coordinates.setAttribute("i", 0.0);
            coordinates.setAttribute("j", 0.0);
            coordinates.setAttribute("k", 0.0);
        }
        geometry.appendChild(coordinates);
    }

    //add radius
    if(this->hasRadius()){
        QDomElement coordinates = xmlDoc.createElement("radius");
        if(this->isSolved || this->isNominal){
            coordinates.setAttribute("value", this->getRadius().getRadius());
        }else{
            coordinates.setAttribute("value", 0.0);
        }
        geometry.appendChild(coordinates);
    }

    //add standard deviation
    QDomElement stdv = xmlDoc.createElement("standardDeviation");
    if(this->getIsSolved()){
        stdv.setAttribute("value", this->statistic.getStdev());
    }else{
        stdv.setAttribute("value", 0.0);
    }
    geometry.appendChild(stdv);

    //add observations
    if(!this->getIsNominal() && this->observations.size() > 0){
        QDomElement observations = xmlDoc.createElement("observations");
        foreach(const QPointer<Observation> &obs, this->observations){
            QDomElement observation = xmlDoc.createElement("observation");
            observation.setAttribute("ref", obs->getId());
            observations.appendChild(observation);
        }
        geometry.appendChild(observations);
    }

    //add nominal system
    if(this->isNominal && !this->nominalSystem.isNull()){
        QDomElement nominalSystem = xmlDoc.createElement("nominalCoordinateSystem");
        nominalSystem.setAttribute("ref", this->nominalSystem->getId());
        geometry.appendChild(nominalSystem);
    }

    //add corresponding actual geometry
    if(this->isNominal && !this->actual.isNull()){
        QDomElement actual = xmlDoc.createElement("actual");
        actual.setAttribute("ref", this->actual->getId());
        geometry.appendChild(actual);
    }

    //add nominals
    if(!this->isNominal && this->nominals.size() >= 0){
        QDomElement nominals = xmlDoc.createElement("nominalGeometries");
        foreach(const QPointer<Geometry> &geom, this->nominals){
            if(!geom.isNull()){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getId());
                nominals.appendChild(nominal);
            }
        }
        geometry.appendChild(nominals);
    }

    //add measurement config
    if(!this->isNominal){
        QDomElement mConfig = xmlDoc.createElement("measurementConfig");
        mConfig.setAttribute("name", this->activeMeasurementConfig.getName());
        geometry.appendChild(mConfig);
    }

    return geometry;

}

/*!
 * \brief Geometry::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Geometry::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        //set geometry attributes
        if(!xmlElem.hasAttribute("nominal") || !xmlElem.hasAttribute("common")){
            return false;
        }
        this->isNominal = xmlElem.attribute("nominal").toInt();
        this->isCommon = xmlElem.attribute("common").toInt();

        //set standard deviation
        QDomElement stdv = xmlElem.firstChildElement("standardDeviation");
        if(stdv.isNull() || !stdv.hasAttribute("value")){
            return false;
        }
        this->statistic.setStdev(stdv.attribute("value").toDouble());

    }

    return result;

}

/*!
 * \brief Geometry::getDisplayStDev
 * \param type
 * \param digits
 * \return
 */
QString Geometry::getDisplayStDev(const UnitType &type, const int &digits) const{
    if(this->statistic.getIsValid()){
        return QString::number(convertFromDefault(this->statistic.getStdev(), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Geometry::getDisplayFormError
 * \param type
 * \param digits
 * \return
 */
QString Geometry::getDisplayFormError(const UnitType &type, const int &digits) const{
    if(this->statistic.getIsValid()){
        return QString::number(convertFromDefault(this->statistic.getFormError(), type), 'f', digits);
    }
    return QString("");
}
/*!
 * \brief Geometry::getDisplayMeasurementConfig
 * \return
 */
QString Geometry::getDisplayMeasurementConfig() const{
    if(this->activeMeasurementConfig.isValid()){
        if(this->activeMeasurementConfig.isProjectConfig()){
            return this->activeMeasurementConfig.getName();
        }else{
            return QString("%1 [user]").arg(this->activeMeasurementConfig.getName());
        }
    }
    return QString("");
}

/*!
 * \brief Geometry::getDisplayObservations
 * \return
 */
QString Geometry::getDisplayObservations() const{

    if(this->isNominal){
        return "";
    }

    int validObs = 0;
    int totalObs = this->observations.size();

    //get the number of used observations in the first function of this geometry
    if(totalObs > 0 && this->functionList.size() > 0){

        //get and check function pointer
        QPointer<Function> firstFunction = this->functionList.at(0);
        if(firstFunction.isNull()){
            return QString("0/%1").arg(totalObs);
        }

        //get the number of used observations
        QMap<int, QList<InputElement> > inputElements = firstFunction->getInputElements();
        QList<int> keys = inputElements.keys();
        foreach(const int &key, keys){

            //check if input alements at key are observations
            if(inputElements[key].size() > 0 && inputElements[key][0].typeOfElement == eObservationElement){

                //run through all observations and check their used state
                foreach(const InputElement &element, inputElements[key]){
                    if(element.isUsed){
                        validObs++;
                    }
                }
            }
        }

        return QString("%1/%2").arg(validObs).arg(totalObs);
    }

    return QString("0/%1").arg(totalObs);
}

/*!
 * \brief Geometry::getDisplayIsCommon
 * \return
 */
QString Geometry::getDisplayIsCommon() const{
    return this->isCommon?"true":"false";
}

/*!
 * \brief Geometry::getDisplayIsActual
 * \return
 */
QString Geometry::getDisplayIsActual() const
{
    if(this->getIsNominal()){
        QString output = "";
        output.append("nominal ");
        output.append(this->getNominalSystem()->getFeatureName());
        return output;
    }else{
        return "actual";
    }
}


/*!
 * \brief Plane::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Geometry::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Plane::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Geometry::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Plane::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Geometry::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->xyz.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Plane::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Geometry::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(!this->hasDirection()) {
        return Feature::getDisplayPrimaryI(digits, showDiff);
    }
    if(this->isSolved){
        return QString::number(this->ijk.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Plane::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Geometry::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(!this->hasDirection()) {
        return Feature::getDisplayPrimaryJ(digits, showDiff);
    }
    if(this->isSolved){
        return QString::number(this->ijk.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Plane::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Geometry::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(!this->hasDirection()) {
        return Feature::getDisplayPrimaryK(digits, showDiff);
    }
    if(this->isSolved){
        return QString::number(this->ijk.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Cylinder::getDisplayRadiusA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Geometry::getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->radius.getRadius(), type), 'f', digits);
    }
    return QString("");
}
