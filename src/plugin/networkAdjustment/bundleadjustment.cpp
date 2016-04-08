#include "bundleadjustment.h"

using namespace oi;

/*!
 * \brief BundleAdjustment::BundleAdjustment
 * \param parent
 */
BundleAdjustment::BundleAdjustment(QObject *parent) : QObject(parent){

}

/*!
 * \brief BundleAdjustment::~BundleAdjustment
 */
BundleAdjustment::~BundleAdjustment(){

}

/*!
 * \brief BundleAdjustment::init
 */
void BundleAdjustment::init(){

}

/*!
 * \brief BundleAdjustment::runBundle
 * \return
 */
bool BundleAdjustment::runBundle(){
    emit this->sendMessage("Bundle \"" + this->getMetaData().name + "\" not implemented", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief BundleAdjustment::getMetaData
 * \return
 */
const PluginMetaData &BundleAdjustment::getMetaData() const{
    return this->metaData;
}

/*!
 * \brief BundleAdjustment::getIntegerParameter
 * \return
 */
const QMap<QString, int> &BundleAdjustment::getIntegerParameter() const{
    return this->integerParameters;
}

/*!
 * \brief BundleAdjustment::getDoubleParameter
 * \return
 */
const QMap<QString, double> &BundleAdjustment::getDoubleParameter() const{
    return this->doubleParameters;
}

/*!
 * \brief BundleAdjustment::getStringParameter
 * \return
 */
const QMultiMap<QString, QString> &BundleAdjustment::getStringParameter() const{
    return this->stringParameters;
}

/*!
 * \brief BundleAdjustment::getScalarInputParams
 * \return
 */
const ScalarInputParams &BundleAdjustment::getScalarInputParams(){

    //use default values if no scalar input params have been specified
    if(!this->scalarInputParams.isValid){
        this->scalarInputParams.doubleParameter = this->doubleParameters;
        this->scalarInputParams.intParameter = this->integerParameters;
        QStringList keys = this->stringParameters.keys();
        foreach(const QString &key, keys){
            this->scalarInputParams.stringParameter.insert(key, this->stringParameters.value(key));
        }
    }

    return this->scalarInputParams;

}

/*!
 * \brief BundleAdjustment::setScalarInputParams
 * \param params
 */
void BundleAdjustment::setScalarInputParams(const ScalarInputParams &params){
    this->scalarInputParams = params;
    this->scalarInputParams.isValid = true;
    emit this->scalarInputParametersChanged();
}

/*!
 * \brief BundleAdjustment::getInputStations
 * \return
 */
const QList<BundleStation> &BundleAdjustment::getInputStations() const{
    return this->stations;
}

/*!
 * \brief BundleAdjustment::setInputStations
 * \param stations
 */
void BundleAdjustment::setInputStations(const QList<BundleStation> &stations){
    this->stations = stations;
    emit this->inputStationsChanged();
}

/*!
 * \brief BundleAdjustment::getBaseStation
 * \return
 */
const BundleStation &BundleAdjustment::getBaseStation() const{
    return this->baseSystem;
}

/*!
 * \brief BundleAdjustment::setBaseStation
 * \param station
 */
void BundleAdjustment::setBaseStation(const BundleStation &station){
    this->baseSystem = station;
    emit this->baseStationChanged();
}

/*!
 * \brief BundleAdjustment::getBundleSystem
 * \return
 */
const QPointer<CoordinateSystem> &BundleAdjustment::getBundleSystem() const{
    return this->bundleSystem;
}

/*!
 * \brief BundleAdjustment::setBundleSystem
 * \param system
 */
void BundleAdjustment::setBundleSystem(const QPointer<CoordinateSystem> &system){
    this->bundleSystem = system;
    emit this->bundleSystemChanged();
}

/*!
 * \brief BundleAdjustment::getCurrentJob
 * \return
 */
const QPointer<OiJob> &BundleAdjustment::getCurrentJob() const{
    return this->currentJob;
}

/*!
 * \brief BundleAdjustment::setCurrentJob
 * \param job
 */
void BundleAdjustment::setCurrentJob(const QPointer<OiJob> &job){

    //disconnect old job
    this->disconnectJob();

    //set and connect new job
    this->currentJob = job;
    this->connectJob();
    emit this->jobChanged();

}

/*!
 * \brief BundleAdjustment::getId
 * \return
 */
const int &BundleAdjustment::getId() const{
    return this->id;
}

/*!
 * \brief BundleAdjustment::clear
 */
void BundleAdjustment::clear(){
    this->scalarInputParams.isValid = false;
    this->stations.clear();
    this->baseSystem = BundleStation();
}

/*!
 * \brief BundleAdjustment::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement BundleAdjustment::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    //add function attributes
    QDomElement bundle = xmlDoc.createElement("bundle");
    bundle.setAttribute("name", this->getMetaData().name);
    bundle.setAttribute("type", this->getMetaData().iid);
    bundle.setAttribute("plugin", this->getMetaData().pluginName);

    //add bundle stations
    QDomElement bundleStations = xmlDoc.createElement("bundleStations");
    foreach(const BundleStation &station, this->stations){
        QDomElement bundleStation = xmlDoc.createElement("bundleStation");
        bundleStation.setAttribute("id", station.id);
        bundleStation.setAttribute("tx", station.tx);
        bundleStation.setAttribute("ty", station.ty);
        bundleStation.setAttribute("tz", station.tz);
        bundleStation.setAttribute("rx", station.rx);
        bundleStation.setAttribute("ry", station.ry);
        bundleStation.setAttribute("rz", station.rz);
        bundleStation.setAttribute("m", station.m);
        bundleStations.appendChild(bundleStation);
    }
    bundle.appendChild(bundleStations);

    //add base bundle system
    QDomElement baseSystem = xmlDoc.createElement("baseSystem");
    baseSystem.setAttribute("ref", this->baseSystem.id);
    bundle.appendChild(baseSystem);

    //add integer parameters
    QDomElement integerParams = xmlDoc.createElement("integerParameters");
    QMapIterator<QString, int> intIterator(this->scalarInputParams.intParameter);
    while (intIterator.hasNext()) {
        intIterator.next();
        QDomElement intParam = xmlDoc.createElement("parameter");
        intParam.setAttribute("name", intIterator.key());
        intParam.setAttribute("value", intIterator.value());
        integerParams.appendChild(intParam);
    }
    bundle.appendChild(integerParams);

    //add double parameters
    QDomElement doubleParams = xmlDoc.createElement("doubleParameters");
    QMapIterator<QString, double> doubleIterator(this->scalarInputParams.doubleParameter);
    while (doubleIterator.hasNext()) {
        doubleIterator.next();
        QDomElement doubleParam = xmlDoc.createElement("parameter");
        doubleParam.setAttribute("name", doubleIterator.key());
        doubleParam.setAttribute("value", doubleIterator.value());
        doubleParams.appendChild(doubleParam);
    }
    bundle.appendChild(doubleParams);

    //add string parameters
    QDomElement stringParams = xmlDoc.createElement("stringParameters");
    QMapIterator<QString, QString> stringIterator(this->scalarInputParams.stringParameter);
    while (stringIterator.hasNext()) {
        stringIterator.next();
        QDomElement stringParam = xmlDoc.createElement("parameter");
        stringParam.setAttribute("name", stringIterator.key());
        stringParam.setAttribute("value", stringIterator.value());
        stringParams.appendChild(stringParam);
    }
    bundle.appendChild(stringParams);

    return bundle;

}

/*!
 * \brief BundleAdjustment::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool BundleAdjustment::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("plugin") || !xmlElem.hasAttribute("executionIndex")
            || !xmlElem.hasAttribute("type")){
        return false;
    }

    //set bundle systems
    QDomElement bundleStations = xmlElem.firstChildElement("bundleStations");
    if(!bundleStations.isNull()){
        QDomNodeList paramList = bundleStations.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement station = paramList.at(i).toElement();
            if(!station.isNull() && station.hasAttribute("id") && station.hasAttribute("tx")
                    && station.hasAttribute("ty") && station.hasAttribute("tz")
                    && station.hasAttribute("rx") && station.hasAttribute("ry")
                    && station.hasAttribute("rz") && station.hasAttribute("m")){
                BundleStation bundleStation;
                bundleStation.id = station.attribute("id").toInt();
                bundleStation.tx = station.attribute("tx").toInt();
                bundleStation.ty = station.attribute("ty").toInt();
                bundleStation.tz = station.attribute("tz").toInt();
                bundleStation.rx = station.attribute("rx").toInt();
                bundleStation.ry = station.attribute("ry").toInt();
                bundleStation.rz = station.attribute("rz").toInt();
                bundleStation.m = station.attribute("m").toInt();
                this->stations.append(bundleStation);
            }
        }
    }

    //set base bundle system
    QDomElement baseStation = xmlElem.firstChildElement("baseSystem");
    if(!baseStation.isNull() && baseStation.hasAttribute("ref")){
        int ref = baseStation.attribute("ref").toInt();
        BundleStation station;
        station.id = ref;
        if(this->stations.contains(station)){
            int index = this->stations.indexOf(station);
            station = this->stations.at(index);
            this->baseSystem = station;
        }
    }

    //set integer parameter
    QDomElement integerParameters = xmlElem.firstChildElement("integerParameters");
    if(!integerParameters.isNull()){
        QDomNodeList paramList = integerParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement integerParameter = paramList.at(i).toElement();
            if(integerParameter.tagName().compare("parameter") == 0 && integerParameter.hasAttribute("value")
                    && integerParameter.hasAttribute("name")){
                this->scalarInputParams.intParameter.insert(integerParameter.attribute("name"), integerParameter.attribute("value").toInt());
            }
        }
    }

    //set double parameter
    QDomElement doubleParameters = xmlElem.firstChildElement("doubleParameters");
    if(!doubleParameters.isNull()){
        QDomNodeList paramList = doubleParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement doubleParameter = paramList.at(i).toElement();
            if(doubleParameter.tagName().compare("parameter") == 0 && doubleParameter.hasAttribute("value")
                    && doubleParameter.hasAttribute("name")){
                this->scalarInputParams.doubleParameter.insert(doubleParameter.attribute("name"), doubleParameter.attribute("value").toDouble());
            }
        }
    }

    //set string parameter
    QDomElement stringParameters = xmlElem.firstChildElement("stringParameters");
    if(!stringParameters.isNull()){
        QDomNodeList paramList = stringParameters.childNodes();
        for(int i = 0; i < paramList.size(); i++){
            QDomElement stringParameter = paramList.at(i).toElement();
            if(stringParameter.tagName().compare("parameter") == 0 && stringParameter.hasAttribute("value")
                    && stringParameter.hasAttribute("name")){
                this->scalarInputParams.stringParameter.insert(stringParameter.attribute("name"), stringParameter.attribute("value"));
            }
        }
    }

    this->scalarInputParams.isValid = true;

    return true;

}

/*!
 * \brief BundleAdjustment::connectJob
 */
void BundleAdjustment::connectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::connect(this, &BundleAdjustment::sendMessage,
                     this->currentJob, &OiJob::sendMessage, Qt::AutoConnection);

}

/*!
 * \brief BundleAdjustment::disconnectJob
 */
void BundleAdjustment::disconnectJob(){

    //check job
    if(this->currentJob.isNull()){
        return;
    }

    QObject::disconnect(this, &BundleAdjustment::sendMessage,
                        this->currentJob, &OiJob::sendMessage);

}
