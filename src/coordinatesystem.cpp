#include "coordinatesystem.h"

#include "featurewrapper.h"
#include "observation.h"
#include "geometry.h"
#include "trafoparam.h"
#include "oijob.h"
#include "bundleadjustment.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief CoordinateSystem::CoordinateSystem
 * \param parent
 */
CoordinateSystem::CoordinateSystem(QObject *parent) : Feature(parent), isActiveCoordinateSystem(false), isStationSystem(false), isBundleSystem(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

}

/*!
 * \brief CoordinateSystem::CoordinateSystem
 * \param isBundleSystem
 * \param parent
 */
CoordinateSystem::CoordinateSystem(bool isBundleSystem, QObject *parent) : Feature(parent), isActiveCoordinateSystem(false), isStationSystem(false), isBundleSystem(isBundleSystem){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

}

/*!
 * \brief CoordinateSystem::CoordinateSystem
 * \param station
 * \param parent
 */
CoordinateSystem::CoordinateSystem(const QPointer<Station> &station, QObject *parent) : Feature(parent), isActiveCoordinateSystem(false), isBundleSystem(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

    //set up corresponding station
    if(!station.isNull()){
        this->isStationSystem = true;
        this->station = station;
    }

}

/*!
 * \brief CoordinateSystem::CoordinateSystem
 * \param copy
 * \param parent
 */
CoordinateSystem::CoordinateSystem(const CoordinateSystem &copy, QObject *parent) : Feature(parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

    //copy attributes
    this->name = copy.name;
    this->id = copy.id;
    this->origin = copy.origin;
    this->xAxis = copy.xAxis;
    this->yAxis = copy.yAxis;
    this->zAxis = copy.zAxis;
    this->isStationSystem = copy.isStationSystem;
    this->isBundleSystem = copy.isBundleSystem;
    this->isActiveCoordinateSystem = copy.isActiveCoordinateSystem;
    this->expansionOrigin = copy.expansionOrigin;

}

/*!
 * \brief CoordinateSystem::operator =
 * \param copy
 * \return
 */
CoordinateSystem &CoordinateSystem::operator=(const CoordinateSystem &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCoordinateSystem(this);
    }

    //copy attributes
    this->name = copy.name;
    this->id = copy.id;
    this->origin = copy.origin;
    this->xAxis = copy.xAxis;
    this->yAxis = copy.yAxis;
    this->zAxis = copy.zAxis;
    this->isStationSystem = copy.isStationSystem;
    this->isBundleSystem = copy.isBundleSystem;
    this->isActiveCoordinateSystem = copy.isActiveCoordinateSystem;
    this->expansionOrigin = copy.expansionOrigin;

    return *this;

}

/*!
 * \brief CoordinateSystem::~CoordinateSystem
 */
CoordinateSystem::~CoordinateSystem(){

    //delete all observations made from this station coordinate system (only if this is a station system)
    foreach(const QPointer<Observation> &observation, this->observationsList){
        if(!observation.isNull()){
            delete observation;
        }
    }

    //delete transformation parameter sets from this coordinate system
    foreach(const QPointer<TrafoParam> &trafoParam, this->trafoParams){
        if(!trafoParam.isNull()){
            delete trafoParam;
        }
    }

    //delete nominals of this coordinate system
    foreach(const QPointer<FeatureWrapper> &nominal, this->nominalsList){
        if(!nominal.isNull()){
            if(!nominal->getFeature().isNull()){
                delete nominal->getFeature();
            }
            delete nominal;
        }
    }

    //delete bundle adjustment
    if(!this->bundlePlugin.isNull()){
        delete this->bundlePlugin;
    }

}

/*!
 * \brief CoordinateSystem::getIsActiveCoordinateSystem
 * \return
 */
const bool &CoordinateSystem::getIsActiveCoordinateSystem() const{
    return this->isActiveCoordinateSystem;
}

/*!
 * \brief CoordinateSystem::setActiveCoordinateSystemState
 * \param isActiveCoordinateSystem
 */
void CoordinateSystem::setActiveCoordinateSystemState(const bool &isActiveCoordinateSystem){
    if(this->isActiveCoordinateSystem != isActiveCoordinateSystem){
        this->isActiveCoordinateSystem = isActiveCoordinateSystem;
        emit this->activeCoordinateSystemChanged(this->id);
    }
}

/*!
 * \brief CoordinateSystem::getIsStationSystem
 * \return
 */
const bool &CoordinateSystem::getIsStationSystem() const{
    return this->isStationSystem;
}

/*!
 * \brief CoordinateSystem::getStation
 * \return
 */
const QPointer<Station> &CoordinateSystem::getStation() const{
    return this->station;
}

/*!
 * \brief CoordinateSystem::getIsBundleSystem
 * \return
 */
const bool &CoordinateSystem::getIsBundleSystem() const{
    return this->isBundleSystem;
}

/*!
 * \brief CoordinateSystem::getOrigin
 * \return
 */
const Position &CoordinateSystem::getOrigin() const{
    return this->origin;
}

/*!
 * \brief CoordinateSystem::getXAxis
 * \return
 */
const Direction &CoordinateSystem::getXAxis() const{
    return this->xAxis;
}

/*!
 * \brief CoordinateSystem::getYAxis
 * \return
 */
const Direction &CoordinateSystem::getYAxis() const{
    return this->yAxis;
}

/*!
 * \brief CoordinateSystem::getZAxis
 * \return
 */
const Direction &CoordinateSystem::getZAxis() const{
    return this->zAxis;
}

/*!
 * \brief CoordinateSystem::setCoordinateSystem
 * \param origin
 * \param xAxis
 * \param yAxis
 * \param zAxis
 */
void CoordinateSystem::setCoordinateSystem(const Position &origin, const Direction &xAxis, const Direction &yAxis, const Direction &zAxis){
    this->origin = origin;
    this->xAxis = xAxis;
    this->yAxis = yAxis;
    this->zAxis = zAxis;
}

/*!
 * \brief CoordinateSystem::setOrigin
 * \param origin
 */
void CoordinateSystem::setOrigin(const Position origin)
{
    this->origin.setVector(origin.getVector());

    if(!this->station.isNull()){
        this->station->setPosition(origin);
    }
}

/*!
 * \brief CoordinateSystem::getExpansionOrigin
 * \return
 */
const Position &CoordinateSystem::getExpansionOrigin() const{
    return this->expansionOrigin;
}

/*!
 * \brief CoordinateSystem::setExpansionOrigin
 * \param expansionOrigin
 */
void CoordinateSystem::setExpansionOrigin(const Position &expansionOrigin){
    this->expansionOrigin = expansionOrigin;
}

/*!
 * \brief CoordinateSystem::getObservations
 * \return
 */
const QList<QPointer<Observation> > &CoordinateSystem::getObservations() const{
    return this->observationsList;
}

/*!
 * \brief CoordinateSystem::getObservation
 * \param observationId
 * \return
 */
QPointer<Observation> CoordinateSystem::getObservation(const int &observationId) const{
    return this->observationsMap.value(observationId, QPointer<Observation>());
}

/*!
 * \brief CoordinateSystem::addObservation
 * \param observation
 * \return
 */
bool CoordinateSystem::addObservation(const QPointer<Observation> &observation){

    //check if this is a station system
    if(!this->isStationSystem || this->station.isNull()){
        return false;
    }

    if(!observation.isNull() && !observation->getReading().isNull() && observation->getStation().isNull()){

        //set observation's station
        observation->setStation(this->station);

        //generate unique id for the observation and its corresponding reading
        if(!this->job.isNull()){
            observation->id = this->job->generateUniqueId();
            observation->getReading()->id = this->job->generateUniqueId();
        }

        //add observation to the coordinate system
        this->observationsList.append(observation);
        this->observationsMap.insert(observation->getId(), observation);

        emit this->observationsChanged(this->id, observation->getId());

        return true;

    }

    return false;

}

/*!
 * \brief CoordinateSystem::getTransformationParameters
 * \return
 */
const QList<QPointer<TrafoParam> > &CoordinateSystem::getTransformationParameters() const{
    return this->trafoParams;
}

/*!
 * \brief CoordinateSystem::getTransformationParameters
 * \param to
 * \return
 */
const QList<QPointer<TrafoParam> > CoordinateSystem::getTransformationParameters(const QPointer<CoordinateSystem> &to) const{

    QList< QPointer<TrafoParam> > result;

    //check the destination system
    if(to.isNull()){
        return result;
    }

    //run through all trafo params and select those whose destination system equals "to"
    foreach(const QPointer<TrafoParam> &trafo, this->trafoParams){
        if(!trafo.isNull() && !trafo->getDestinationSystem().isNull() && !trafo->getStartSystem().isNull()){
            if(trafo->getDestinationSystem() == to || trafo->getStartSystem() == to){
               result.append(trafo);
            }
        }
    }

    return result;

}

/*!
 * \brief CoordinateSystem::addTransformationParameter
 * \param trafoParam
 * \return
 */
bool CoordinateSystem::addTransformationParameter(const QPointer<TrafoParam> &trafoParam){

    if(!trafoParam.isNull() && !trafoParam->getStartSystem().isNull() && !trafoParam->getDestinationSystem().isNull()){

        //check if the trafo param is already contained
        if(this->trafoParams.contains(trafoParam)){
            return false;
        }

        //check if the trafoParam is in the same job
        if(!this->job.isNull() && !this->job->getTransformationParametersList().contains(trafoParam)){
            return false;
        }

        //check if this system is either start or destination system
        if(trafoParam->getStartSystem() != this && trafoParam->getDestinationSystem() != this){
            return false;
        }

        this->trafoParams.append(trafoParam);

        emit this->transformationParametersChanged(this->id);

        return true;

    }

    return false;

}

/*!
 * \brief CoordinateSystem::removeTransformationParameter
 * \param trafoParam
 * \return
 */
bool CoordinateSystem::removeTransformationParameter(const QPointer<TrafoParam> &trafoParam){
    if(!trafoParam.isNull()){
        bool result = this->trafoParams.removeOne(trafoParam);
        emit this->transformationParametersChanged(this->id);
        return result;
    }
    return false;
}

/*!
 * \brief CoordinateSystem::getNominals
 * \return
 */
const QList<QPointer<FeatureWrapper> > &CoordinateSystem::getNominals() const{
    return this->nominalsList;
}

/*!
 * \brief CoordinateSystem::addNominal
 * \param nominal
 * \return
 */
bool CoordinateSystem::addNominal(const QPointer<FeatureWrapper> &nominal){

    //check if this is a nominal system
    if(this->isStationSystem){
        return false;
    }

    if(!nominal.isNull() && !nominal->getGeometry().isNull() && nominal->getGeometry()->getIsNominal()){

        //check if the nominal has already been added
        if(this->nominalsMap.contains(nominal->getGeometry()->getId())){
            return false;
        }

        //check if the nominal is in the same job
        if(!this->job.isNull()){
            QPointer<FeatureWrapper> jobFeature = this->job->getFeatureById(nominal->getGeometry()->getId());
            if(jobFeature.isNull() || jobFeature->getGeometry().isNull() || jobFeature->getGeometry() != nominal->getGeometry()){
                return false;
            }
        }

        nominal->getGeometry()->setNominalSystem(this);
        this->nominalsList.append(nominal);
        this->nominalsMap.insert(nominal->getGeometry()->getId(), nominal);

        emit this->nominalsChanged(this->id);

        return true;

    }

    return false;

}

/*!
 * \brief CoordinateSystem::addNominals
 * \param nominals
 * \return
 */
bool CoordinateSystem::addNominals(const QList<QPointer<FeatureWrapper> > &nominals){

    if(nominals.size() > 0){

        int oldSize = this->nominalsList.size();

        //add all nominals without emitting the corresponding signals
        this->blockSignals(true);
        foreach(const QPointer<FeatureWrapper> &geom, nominals){
            this->addNominal(geom);
        }
        this->blockSignals(false);

        if(this->nominalsList.size() > oldSize){
            emit this->nominalsChanged(this->id);
            return true;
        }

    }

    return false;

}

/*!
 * \brief CoordinateSystem::removeNominal
 * \param nominal
 * \return
 */
bool CoordinateSystem::removeNominal(const QPointer<FeatureWrapper> &nominal){

    if(!nominal.isNull() && !nominal->getGeometry().isNull()){

        //get nominal pointer to remove
        QPointer<FeatureWrapper> removeNominal = nominal;
        if(this->nominalsMap.contains(nominal->getGeometry()->getId())){
            removeNominal = this->nominalsMap[nominal->getGeometry()->getId()];
        }

        if(this->nominalsMap.remove(nominal->getGeometry()->getId()) == 1){
            this->nominalsList.removeOne(removeNominal);
            emit this->nominalsChanged(this->id);
            return true;
        }

    }

    return false;

}

/*!
 * \brief CoordinateSystem::removeNominal
 * \param featureId
 * \return
 */
bool CoordinateSystem::removeNominal(const int &featureId){

    QPointer<FeatureWrapper> nominal = this->nominalsMap.value(featureId, QPointer<FeatureWrapper>());

    if(!nominal.isNull()){
        this->nominalsMap.remove(featureId);
        this->nominalsList.removeOne(nominal);
        emit this->nominalsChanged(this->id);
        return true;
    }

    return false;

}

/*!
 * \brief CoordinateSystem::getBundleTemplate
 * \return
 */
const QJsonObject &CoordinateSystem::getBundleTemplate() const{
    return this->bundleTemplate;
}

/*!
 * \brief CoordinateSystem::setBundleTemplate
 * \param bundleTemplate
 */
void CoordinateSystem::setBundleTemplate(const QJsonObject &bundleTemplate){
    this->bundleTemplate = bundleTemplate;
}

/*!
 * \brief CoordinateSystem::getBundlePlugin
 * \return
 */
const QPointer<BundleAdjustment> &CoordinateSystem::getBundlePlugin() const{
    return this->bundlePlugin;
}

/*!
 * \brief CoordinateSystem::setBundlePlugin
 * \param bundle
 */
void CoordinateSystem::setBundlePlugin(const QPointer<BundleAdjustment> &bundle){

    //check if the system is a bundle system
    if(!this->isBundleSystem){
        return;
    }

    //check bundle plugin
    if(bundle.isNull()){
        return;
    }

    //pass coordinate system to bundle plugin
    bundle->setBundleSystem(this);

    //set bundle
    this->bundlePlugin = bundle;
    emit this->bundlePluginChanged(this->id);

}

/*!
 * \brief CoordinateSystem::recalc
 */
void CoordinateSystem::recalc(){
    Feature::recalc();
}

/*!
 * \brief CoordinateSystem::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement CoordinateSystem::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement coordinateSystem = Feature::toOpenIndyXML(xmlDoc);

    if(coordinateSystem.isNull()){
        return coordinateSystem;
    }

    coordinateSystem.setTagName("coordinateSystem");

    //add coordinate system attributes
    coordinateSystem.setAttribute("activeSystem", this->getIsActiveCoordinateSystem());
    coordinateSystem.setAttribute("isBundleSystem", this->getIsBundleSystem());

    //add system definition
    QDomElement position = xmlDoc.createElement("coordinates");
    position.setAttribute("x", this->origin.getVector().getAt(0));
    position.setAttribute("y", this->origin.getVector().getAt(1));
    position.setAttribute("z", this->origin.getVector().getAt(2));
    coordinateSystem.appendChild(position);
    QDomElement xAxis = xmlDoc.createElement("xAxis");
    xAxis.setAttribute("i", this->xAxis.getVector().getAt(0));
    xAxis.setAttribute("j", this->xAxis.getVector().getAt(1));
    xAxis.setAttribute("k", this->xAxis.getVector().getAt(2));
    coordinateSystem.appendChild(xAxis);
    QDomElement yAxis = xmlDoc.createElement("yAxis");
    yAxis.setAttribute("i", this->yAxis.getVector().getAt(0));
    yAxis.setAttribute("j", this->yAxis.getVector().getAt(1));
    yAxis.setAttribute("k", this->yAxis.getVector().getAt(2));
    coordinateSystem.appendChild(yAxis);
    QDomElement zAxis = xmlDoc.createElement("zAxis");
    zAxis.setAttribute("i", this->zAxis.getVector().getAt(0));
    zAxis.setAttribute("j", this->zAxis.getVector().getAt(1));
    zAxis.setAttribute("k", this->zAxis.getVector().getAt(2));
    coordinateSystem.appendChild(zAxis);

    //add trafo params
    if(this->trafoParams.size() > 0){
        QDomElement trafoParams = xmlDoc.createElement("transformationParameters");
        foreach(const QPointer<TrafoParam> &tp, this->trafoParams){
            if(!tp.isNull()){
                QDomElement trafoParam = xmlDoc.createElement("transformationParameter");
                trafoParam.setAttribute("ref", tp->getId());
                trafoParams.appendChild(trafoParam);
            }
        }
        coordinateSystem.appendChild(trafoParams);
    }

    //add observations
    if(this->observationsList.size() > 0){
        QDomElement observations = xmlDoc.createElement("observations");
        foreach(const QPointer<Observation> &obs, this->observationsList){
            if(!obs.isNull()){
                QDomElement observation = xmlDoc.createElement("observation");
                observation.setAttribute("ref", obs->getId());
                observations.appendChild(observation);
            }
        }
        coordinateSystem.appendChild(observations);
    }

    //add nominals
    if(this->nominalsList.size() > 0){
        QDomElement nominals = xmlDoc.createElement("nominalGeometries");
        foreach(const QPointer<FeatureWrapper> &geom, this->nominalsList){
            if(!geom.isNull() && !geom->getGeometry().isNull()){
                QDomElement nominal = xmlDoc.createElement("geometry");
                nominal.setAttribute("ref", geom->getGeometry()->getId());
                nominals.appendChild(nominal);
            }
        }
        coordinateSystem.appendChild(nominals);
    }

    //add expansion origin
    QDomElement expansionOrigin = xmlDoc.createElement("expansionOrigin");
    expansionOrigin.setAttribute("x", this->expansionOrigin.getVector().getAt(0));
    expansionOrigin.setAttribute("y", this->expansionOrigin.getVector().getAt(1));
    expansionOrigin.setAttribute("z", this->expansionOrigin.getVector().getAt(2));
    coordinateSystem.appendChild(expansionOrigin);

    //add bundleTemplate
    //temp variables
    QString tmpName = this->bundleTemplate["name"].toString();
    QString tmpLevelStation = this->bundleTemplate["levelStation"].toString();
    QString tmpMaxIterations = this->bundleTemplate["maxIterations"].toString();

    //add bundleTemplate
    QDomElement bundleTmp = xmlDoc.createElement("bundleTemplate");
    bundleTmp.setAttribute("name", tmpName);
    bundleTmp.setAttribute("levelStation", tmpLevelStation);
    bundleTmp.setAttribute("maxIterations", tmpMaxIterations);

    QDomElement parametersTmp = xmlDoc.createElement("parameters");
    QJsonArray tmpArray = this->bundleTemplate["parameters"].toArray();
    for(int i=0; i<tmpArray.size();i++){
        parametersTmp.setAttribute(tmpArray.at(i).toString(),"");
    }
    bundleTmp.appendChild(parametersTmp);

    QDomElement pluginTmp = xmlDoc.createElement("plugin");
    QJsonObject tmpPlugin = this->bundleTemplate["plugin"].toObject();

    if(tmpPlugin.contains("name") && tmpPlugin.contains("pluginName")){
        pluginTmp.setAttribute("name", tmpPlugin.value("name").toString());
        pluginTmp.setAttribute("pluginName", tmpPlugin.value("pluginName").toString());
    }
    bundleTmp.appendChild(pluginTmp);

    coordinateSystem.appendChild(bundleTmp);

    //add bundle adjustment
    if(!this->bundlePlugin.isNull()){
        QDomElement bundle = this->bundlePlugin->toOpenIndyXML(xmlDoc);
        coordinateSystem.appendChild(bundle);
    }

    return coordinateSystem;
}

/*!
 * \brief CoordinateSystem::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool CoordinateSystem::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        if(!xmlElem.hasAttribute("activeSystem") || !xmlElem.hasAttribute("group") || !xmlElem.hasAttribute("comment")
                || !xmlElem.hasAttribute("solved")){
            return false;
        }

        //set coordinate system attributes
        this->isActiveCoordinateSystem = xmlElem.attribute("activeSystem").toInt();

        //set bundle value for coordinate system and trafo params
        this->isBundleSystem = xmlElem.attribute("isBundleSystem").toInt();

        //set coordinate system definition
        QDomElement position = xmlElem.firstChildElement("coordinates");
        if(position.isNull() || !position.hasAttribute("x") || !position.hasAttribute("y") || !position.hasAttribute("z")){
            return false;
        }
        this->origin.setVector(position.attribute("x").toDouble(),
                               position.attribute("y").toDouble(),
                               position.attribute("z").toDouble());
        QDomElement xAxis = xmlElem.firstChildElement("xAxis");
        if(xAxis.isNull() || !xAxis.hasAttribute("i") || !xAxis.hasAttribute("j") || !xAxis.hasAttribute("k")){
            return false;
        }
        this->xAxis.setVector(xAxis.attribute("i").toDouble(),
                              xAxis.attribute("j").toDouble(),
                              xAxis.attribute("k").toDouble());
        QDomElement yAxis = xmlElem.firstChildElement("yAxis");
        if(yAxis.isNull() || !yAxis.hasAttribute("i") || !yAxis.hasAttribute("j") || !yAxis.hasAttribute("k")){
            return false;
        }
        this->yAxis.setVector(yAxis.attribute("i").toDouble(),
                              yAxis.attribute("j").toDouble(),
                              yAxis.attribute("k").toDouble());
        QDomElement zAxis = xmlElem.firstChildElement("zAxis");
        if(zAxis.isNull() || !zAxis.hasAttribute("i") || !zAxis.hasAttribute("j") || !zAxis.hasAttribute("k")){
            return false;
        }
        this->zAxis.setVector(zAxis.attribute("i").toDouble(),
                              zAxis.attribute("j").toDouble(),
                              zAxis.attribute("k").toDouble());


        //set expansion origin
        QDomElement expansionOrigin = xmlElem.firstChildElement("expansionOrigin");
        if(!expansionOrigin.isNull()){
            if(!expansionOrigin.hasAttribute("x") || !expansionOrigin.hasAttribute("y") || !expansionOrigin.hasAttribute("z")){
                return false;
            }
            this->expansionOrigin.setVector(expansionOrigin.attribute("x").toDouble(),
                                            expansionOrigin.attribute("y").toDouble(),
                                            expansionOrigin.attribute("z").toDouble());
        }

        //set bundleTemplate
        QDomElement eBundleTemplate =xmlElem.firstChildElement("bundleTemplate");
        if(!eBundleTemplate.isNull()){
            QJsonObject objBundleTemp;
            QJsonValue name = eBundleTemplate.attribute("name");
            objBundleTemp.insert("name", name);
            QJsonValue levelStation = eBundleTemplate.attribute("levelStation");
            objBundleTemp.insert("levelStation", levelStation);
            QJsonValue maxIterations = eBundleTemplate.attribute("maxIterations");
            objBundleTemp.insert("maxIterations", maxIterations);

            QJsonValue parameters = eBundleTemplate.attribute("parameters");
            objBundleTemp.insert("parameters", parameters);

            QDomElement eplugin = eBundleTemplate.firstChildElement("plugin");
            QJsonObject objBundleTmpPLugin;
            objBundleTmpPLugin.insert("name", eplugin.attribute("name"));
            objBundleTmpPLugin.insert("pluginName", eplugin.attribute("pluginName"));
            //QJsonValue plugin(obj2);
            objBundleTemp.insert("plugin", objBundleTmpPLugin);

            this->bundleTemplate = objBundleTemp;
        }

        //load bundle adjustment

    }

    return result;

}

/*!
 * \brief CoordinateSystem::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString CoordinateSystem::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{

    if(this->getIsActiveCoordinateSystem()){
        return QString::number(convertFromDefault(this->origin.getVector().getAt(0), type), 'f', digits);
    }
    foreach (QPointer<TrafoParam> trafoP, this->trafoParams) {
        if(trafoP->getIsUsed()){
            return QString::number(convertFromDefault(this->origin.getVector().getAt(0), type), 'f', digits);
        }
    }
    return "";
}

/*!
 * \brief CoordinateSystem::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString CoordinateSystem::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{

    if(this->getIsActiveCoordinateSystem()){
        return QString::number(convertFromDefault(this->origin.getVector().getAt(1), type), 'f', digits);
    }
    foreach (QPointer<TrafoParam> trafoP, this->trafoParams) {
        if(trafoP->getIsUsed()){
            return QString::number(convertFromDefault(this->origin.getVector().getAt(1), type), 'f', digits);
        }
    }
    return "";
}

/*!
 * \brief CoordinateSystem::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString CoordinateSystem::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{

    if(this->getIsActiveCoordinateSystem()){
        return QString::number(convertFromDefault(this->origin.getVector().getAt(2), type), 'f', digits);
    }
    foreach (QPointer<TrafoParam> trafoP, this->trafoParams) {
        if(trafoP->getIsUsed()){
            return QString::number(convertFromDefault(this->origin.getVector().getAt(2), type), 'f', digits);
        }
    }
    return "";
}

/*!
 * \brief CoordinateSystem::getDisplayExpansionOriginX
 * \param type
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayExpansionOriginX(const UnitType &type, const int &digits) const{
    return QString::number(convertFromDefault(this->expansionOrigin.getVector().getAt(0), type), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayExpansionOriginY
 * \param type
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayExpansionOriginY(const UnitType &type, const int &digits) const{
    return QString::number(convertFromDefault(this->expansionOrigin.getVector().getAt(1), type), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayExpansionOriginZ
 * \param type
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayExpansionOriginZ(const UnitType &type, const int &digits) const{
    return QString::number(convertFromDefault(this->expansionOrigin.getVector().getAt(2), type), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayXAxisI
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayXAxisI(const int &digits) const{
    return QString::number(this->xAxis.getVector().getAt(0), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayXAxisJ
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayXAxisJ(const int &digits) const{
    return QString::number(this->xAxis.getVector().getAt(1), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayXAxisK
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayXAxisK(const int &digits) const{
    return QString::number(this->xAxis.getVector().getAt(2), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayYAxisI
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayYAxisI(const int &digits) const{
    return QString::number(this->yAxis.getVector().getAt(0), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayYAxisJ
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayYAxisJ(const int &digits) const{
    return QString::number(this->yAxis.getVector().getAt(1), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayYAxisK
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayYAxisK(const int &digits) const{
    return QString::number(this->yAxis.getVector().getAt(2), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayZAxisI
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayZAxisI(const int &digits) const{
    return QString::number(this->zAxis.getVector().getAt(0), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayZAxisJ
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayZAxisJ(const int &digits) const{
    return QString::number(this->zAxis.getVector().getAt(1), 'f', digits);
}

/*!
 * \brief CoordinateSystem::getDisplayZAxisK
 * \param digits
 * \return
 */
QString CoordinateSystem::getDisplayZAxisK(const int &digits) const{
    return QString::number(this->zAxis.getVector().getAt(2), 'f', digits);
}

/*!
 * \brief CoordinateSystem::setUpFeatureId
 */
void CoordinateSystem::setUpFeatureId(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //generate unique ids for observations
    foreach(const QPointer<Observation> &obs, this->observationsList){
        if(!obs.isNull()){
            obs->id = this->job->generateUniqueId();
        }
    }

    //feature specific
    Feature::setUpFeatureId();

}

/*!
 * \brief CoordinateSystem::removeObservation
 * \param obs
 */
void CoordinateSystem::removeObservation(const QPointer<Observation> &obs){

    //check observation
    if(obs.isNull()){
        return;
    }

    this->observationsList.removeOne(obs);
    this->observationsMap.remove(obs->getId());

}
