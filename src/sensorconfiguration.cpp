#include "sensorconfiguration.h"

#include "sensor.h"

using namespace oi;

/*!
 * \brief SensorConfiguration::SensorConfiguration
 */
SensorConfiguration::SensorConfiguration() : isSaved(false){

}

/*!
 * \brief SensorConfiguration::SensorConfiguration
 * \param copy
 */
SensorConfiguration::SensorConfiguration(const SensorConfiguration &copy){

    //copy sensor config attributes
    this->name = copy.name;
    this->isSaved = copy.isSaved;
    this->typeOfSensor = copy.typeOfSensor;
    this->pluginName = copy.pluginName;
    this->sensorName = copy.sensorName;
    this->accuracy = copy.accuracy;
    this->cConfig = copy.cConfig;
    this->integerParameter = copy.integerParameter;
    this->doubleParameter = copy.doubleParameter;
    this->stringParameter = copy.stringParameter;
    this->availableStringParameter = copy.availableStringParameter;

}

/*!
 * \brief SensorConfiguration::operator =
 * \param copy
 * \return
 */
SensorConfiguration &SensorConfiguration::operator=(const SensorConfiguration &copy){

    //copy sensor config attributes
    this->name = copy.name;
    this->isSaved = copy.isSaved;
    this->typeOfSensor = copy.typeOfSensor;
    this->pluginName = copy.pluginName;
    this->sensorName = copy.sensorName;
    this->accuracy = copy.accuracy;
    this->cConfig = copy.cConfig;
    this->integerParameter = copy.integerParameter;
    this->doubleParameter = copy.doubleParameter;
    this->stringParameter = copy.stringParameter;
    this->availableStringParameter = copy.availableStringParameter;

    return *this;

}

/*!
 * \brief SensorConfiguration::getName
 * \return
 */
const QString &SensorConfiguration::getName() const{
    return this->name;
}

/*!
 * \brief SensorConfiguration::setName
 * \param name
 */
void SensorConfiguration::setName(const QString &name){
    if(this->name.compare(name) != 0){
        this->name = name;
    }
}

/*!
 * \brief SensorConfiguration::getIsSaved
 * \return
 */
const bool &SensorConfiguration::getIsSaved() const{
    return this->isSaved;
}

/*!
 * \brief SensorConfiguration::setIsSaved
 * \param isSaved
 */
void SensorConfiguration::setIsSaved(const bool &isSaved){
    this->isSaved = isSaved;
}

/*!
 * \brief SensorConfiguration::getIsValid
 * \return
 */
bool SensorConfiguration::getIsValid() const{
    if(this->name.compare("") != 0){
        return true;
    }
    return false;
}

/*!
 * \brief SensorConfiguration::getTypeOfSensor
 * \return
 */
const SensorTypes &SensorConfiguration::getTypeOfSensor() const{
    return this->typeOfSensor;
}

/*!
 * \brief SensorConfiguration::setTypeOfSensor
 * \param type
 */
void SensorConfiguration::setTypeOfSensor(const SensorTypes &type){
    this->typeOfSensor = type;
}

/*!
 * \brief SensorConfiguration::getPluginName
 * \return
 */
const QString &SensorConfiguration::getPluginName() const{
    return this->pluginName;
}

/*!
 * \brief SensorConfiguration::setPluginName
 * \param name
 */
void SensorConfiguration::setPluginName(const QString &name){
    this->pluginName = name;
}

/*!
 * \brief SensorConfiguration::getSensorName
 * \return
 */
const QString &SensorConfiguration::getSensorName() const{
    return this->sensorName;
}

/*!
 * \brief SensorConfiguration::setSensorName
 * \param name
 */
void SensorConfiguration::setSensorName(const QString &name){
    this->sensorName = name;
}

/*!
 * \brief SensorConfiguration::getAccuracy
 * \return
 */
const Accuracy &SensorConfiguration::getAccuracy() const{
    return this->accuracy;
}

/*!
 * \brief SensorConfiguration::setAccuracy
 * \param accuracy
 */
void SensorConfiguration::setAccuracy(const Accuracy &accuracy){
    this->accuracy = accuracy;
}

/*!
 * \brief SensorConfiguration::getConnectionConfig
 * \return
 */
const ConnectionConfig &SensorConfiguration::getConnectionConfig() const{
    return this->cConfig;
}

/*!
 * \brief SensorConfiguration::setConnectionConfig
 * \param cConfig
 */
void SensorConfiguration::setConnectionConfig(const ConnectionConfig &cConfig){
    this->cConfig = cConfig;
}

/*!
 * \brief SensorConfiguration::getIntegerParameter
 * \return
 */
const QMap<QString, int> &SensorConfiguration::getIntegerParameter() const{
    return this->integerParameter;
}

/*!
 * \brief SensorConfiguration::setIntegerParameter
 * \param intParams
 */
void SensorConfiguration::setIntegerParameter(const QMap<QString, int> &intParams){
    this->integerParameter = intParams;
}

/*!
 * \brief SensorConfiguration::getDoubleParameter
 * \return
 */
const QMap<QString, double> &SensorConfiguration::getDoubleParameter() const{
    return this->doubleParameter;
}

/*!
 * \brief SensorConfiguration::setDoubleParameter
 * \param doubleParams
 */
void SensorConfiguration::setDoubleParameter(const QMap<QString, double> &doubleParams){
    this->doubleParameter = doubleParams;
}

/*!
 * \brief SensorConfiguration::getStringParameter
 * \return
 */
const QMap<QString, QString> &SensorConfiguration::getStringParameter() const{
    return this->stringParameter;
}

/*!
 * \brief SensorConfiguration::setStringParameter
 * \param stringParams
 */
void SensorConfiguration::setStringParameter(const QMap<QString, QString> &stringParams){
    this->stringParameter = stringParams;
}

/*!
 * \brief SensorConfiguration::getAvailableStringParameter
 * \return
 */
const QMultiMap<QString, QString> &SensorConfiguration::getAvailableStringParameter() const{
    return this->availableStringParameter;
}

/*!
 * \brief SensorConfiguration::setAvailableStringParameter
 * \param stringParams
 */
void SensorConfiguration::setAvailableStringParameter(const QMultiMap<QString, QString> &stringParams){
    this->availableStringParameter = stringParams;
}

/*!
 * \brief SensorConfiguration::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement SensorConfiguration::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement sConfig = xmlDoc.createElement("sensorConfig");
    sConfig.setAttribute("name", this->name);
    sConfig.setAttribute("isSaved", this->isSaved);

    //save plugin information
    QDomElement pluginInfo = xmlDoc.createElement("plugin");
    pluginInfo.setAttribute("name", this->pluginName);
    pluginInfo.setAttribute("sensor", this->sensorName);
    pluginInfo.setAttribute("sensorType", getSensorTypeName(this->typeOfSensor));
    sConfig.appendChild(pluginInfo);

    //save connection parameters
    QDomElement connParams = xmlDoc.createElement("connection");
    connParams.setAttribute("typeOfConnection", this->cConfig.typeOfConnection);
    connParams.setAttribute("ip", this->cConfig.ip);
    connParams.setAttribute("port", this->cConfig.port);
    connParams.setAttribute("comPort", this->cConfig.comPort);
    connParams.setAttribute("baudRate", this->cConfig.baudRate);
    connParams.setAttribute("dataBits", this->cConfig.dataBits);
    connParams.setAttribute("parity", this->cConfig.parity);
    connParams.setAttribute("stopBits", this->cConfig.stopBits);
    connParams.setAttribute("flowControl", this->cConfig.flowControl);
    sConfig.appendChild(connParams);

    //save accuracy values
    QDomElement accuracy = xmlDoc.createElement("accuracy");
    accuracy.setAttribute("sigmaX", this->accuracy.sigmaXyz.getAt(0));
    accuracy.setAttribute("sigmaY", this->accuracy.sigmaXyz.getAt(1));
    accuracy.setAttribute("sigmaZ", this->accuracy.sigmaXyz.getAt(2));
    accuracy.setAttribute("sigmaAzimuth", this->accuracy.sigmaAzimuth);
    accuracy.setAttribute("sigmaZenith", this->accuracy.sigmaZenith);
    accuracy.setAttribute("sigmaDistance", this->accuracy.sigmaDistance);
    accuracy.setAttribute("sigmaTemp", this->accuracy.sigmaTemp);
    accuracy.setAttribute("sigmaI", this->accuracy.sigmaI);
    accuracy.setAttribute("sigmaJ", this->accuracy.sigmaJ);
    accuracy.setAttribute("sigmaK", this->accuracy.sigmaK);
    QDomElement sigmaUndefined = xmlDoc.createElement("sigmaUndefined");
    QStringList params = this->accuracy.sigmaUndefined.keys();
    for(int i = 0; i < params.size(); i++){
        sigmaUndefined.setAttribute(params.at(i), this->accuracy.sigmaUndefined.value(params.at(i)));
    }
    accuracy.appendChild(sigmaUndefined);
    sConfig.appendChild(accuracy);

    //save integer parameters
    if(!this->integerParameter.isEmpty()){
        QDomElement intParams = xmlDoc.createElement("integerParameters");
        QStringList params = this->integerParameter.keys();
        for(int i = 0; i < params.size(); i++){
            QDomElement param = xmlDoc.createElement("parameter");
            param.setAttribute("name", params.at(i));
            param.setAttribute("value", this->integerParameter.value(params.at(i)));
            intParams.appendChild(param);
        }
        sConfig.appendChild(intParams);
    }

    //save double parameters
    if(!this->doubleParameter.isEmpty()){
        QDomElement doubleParams = xmlDoc.createElement("doubleParameters");
        QStringList params = this->doubleParameter.keys();
        for(int i = 0; i < params.size(); i++){
            QDomElement param = xmlDoc.createElement("parameter");
            param.setAttribute("name", params.at(i));
            param.setAttribute("value", this->doubleParameter.value(params.at(i)));
            doubleParams.appendChild(param);
        }
        sConfig.appendChild(doubleParams);
    }

    //save string parameters
    if(!this->stringParameter.isEmpty()){
        QDomElement stringParams = xmlDoc.createElement("stringParameters");
        QStringList params = this->stringParameter.keys();
        for(int i = 0; i < params.size(); i++){
            QDomElement param = xmlDoc.createElement("parameter");
            param.setAttribute("name", params.at(i));
            param.setAttribute("value", this->stringParameter.value(params.at(i)));
            stringParams.appendChild(param);
        }
        sConfig.appendChild(stringParams);
    }

    //save available string parameters
    if(!this->availableStringParameter.isEmpty()){
        QDomElement stringParams = xmlDoc.createElement("availableStringParameters");
        QStringList params = this->availableStringParameter.uniqueKeys();
        QStringList values;
        for(int i = 0; i < params.size(); i++){
            QDomElement param = xmlDoc.createElement("parameter");
            param.setAttribute("name", params.at(i));
            values = this->availableStringParameter.values(params.at(i));
            for(int j = 0; j < values.size(); j++){
                QDomElement paramOption = xmlDoc.createElement("option");
                paramOption.setAttribute("value", values.at(j));
                param.appendChild(paramOption);
            }
            stringParams.appendChild(param);
        }
        sConfig.appendChild(stringParams);
    }

    return sConfig;

}

/*!
 * \brief SensorConfiguration::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool SensorConfiguration::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull() || !xmlElem.hasAttribute("name")){
        return false;
    }

    //set name of sensor config
    this->name = xmlElem.attribute("name");

    //set saved state
    if(xmlElem.hasAttribute("isSaved")){
        this->isSaved = xmlElem.attribute("isSaved").toInt();
    }

    //get plugin information
    QDomElement pluginInfo = xmlElem.firstChildElement("plugin");
    if(pluginInfo.isNull() || !pluginInfo.hasAttribute("name") || !pluginInfo.hasAttribute("sensor") || !pluginInfo.hasAttribute("sensorType")){
        return false;
    }
    this->pluginName = pluginInfo.attribute("name");
    this->sensorName = pluginInfo.attribute("sensor");
    this->typeOfSensor = getSensorTypeEnum(pluginInfo.attribute("sensorType"));

    //get connection parameters
    QDomElement connectionParams = xmlElem.firstChildElement("connection");
    if(!connectionParams.isNull()){

        //set type of connection
        if(!connectionParams.hasAttribute("typeOfConnection")){
            return false;
        }
        this->cConfig.typeOfConnection = (ConnectionTypes)connectionParams.attribute("typeOfConnection").toInt();

        //set connection attributes
        if(connectionParams.hasAttribute("ip")){ this->cConfig.ip = connectionParams.attribute("ip"); }
        if(connectionParams.hasAttribute("port")){ this->cConfig.port = connectionParams.attribute("port"); }
        if(connectionParams.hasAttribute("comPort")){ this->cConfig.comPort = connectionParams.attribute("comPort"); }
        if(connectionParams.hasAttribute("baudRate")){ this->cConfig.baudRate = (QSerialPort::BaudRate)connectionParams.attribute("baudRate").toInt(); }
        if(connectionParams.hasAttribute("dataBits")){ this->cConfig.dataBits = (QSerialPort::DataBits)connectionParams.attribute("dataBits").toInt(); }
        if(connectionParams.hasAttribute("parity")){ this->cConfig.parity = (QSerialPort::Parity)connectionParams.attribute("parity").toInt(); }
        if(connectionParams.hasAttribute("stopBits")){ this->cConfig.stopBits = (QSerialPort::StopBits)connectionParams.attribute("stopBits").toInt(); }
        if(connectionParams.hasAttribute("flowControl")){ this->cConfig.flowControl = (QSerialPort::FlowControl)connectionParams.attribute("flowControl").toInt(); }

    }

    //get accuracy values
    QDomElement accuracy = xmlElem.firstChildElement("accuracy");
    if(!accuracy.isNull()){

        //set standard accuracy values if available
        if(accuracy.hasAttribute("sigmaAzimuth")){ this->accuracy.sigmaAzimuth = accuracy.attribute("sigmaAzimuth").toDouble(); }
        if(accuracy.hasAttribute("sigmaZenith")){ this->accuracy.sigmaZenith = accuracy.attribute("sigmaZenith").toDouble(); }
        if(accuracy.hasAttribute("sigmaDistance")){ this->accuracy.sigmaDistance = accuracy.attribute("sigmaDistance").toDouble(); }
        if(accuracy.hasAttribute("sigmaX")){ this->accuracy.sigmaXyz.setAt(0, accuracy.attribute("sigmaX").toDouble()); }
        if(accuracy.hasAttribute("sigmaY")){ this->accuracy.sigmaXyz.setAt(1, accuracy.attribute("sigmaY").toDouble()); }
        if(accuracy.hasAttribute("sigmaZ")){ this->accuracy.sigmaXyz.setAt(2, accuracy.attribute("sigmaZ").toDouble()); }
        if(accuracy.hasAttribute("sigmaTemp")){ this->accuracy.sigmaTemp = accuracy.attribute("sigmaTemp").toDouble(); }
        if(accuracy.hasAttribute("sigmaI")){ this->accuracy.sigmaI = accuracy.attribute("sigmaI").toDouble(); }
        if(accuracy.hasAttribute("sigmaJ")){ this->accuracy.sigmaJ = accuracy.attribute("sigmaJ").toDouble(); }
        if(accuracy.hasAttribute("sigmaK")){ this->accuracy.sigmaK = accuracy.attribute("sigmaK").toDouble(); }

        //set undefined accuracy values if available
        QDomElement sigmaUndefined = accuracy.firstChildElement("sigmaUndefined");
        if(!sigmaUndefined.isNull()){
            QDomNodeList sigmas = sigmaUndefined.childNodes();
            for(int i = 0; i < sigmas.size(); ++i){
                QDomElement sigmaElement = sigmas.at(i).toElement();
                if(sigmaElement.hasAttribute("name") && sigmaElement.hasAttribute("value")){
                    this->accuracy.sigmaUndefined.insert(sigmaElement.attribute("name"), sigmaElement.attribute("value").toDouble());
                }
            }
        }

    }

    //get integer parameters
    QDomElement intParams = xmlElem.firstChildElement("integerParameters");
    if(!intParams.isNull()){
        QDomNodeList params = intParams.childNodes();
        for(int i = 0; i < params.size(); ++i){
            QDomElement param = params.at(i).toElement();
            if(param.hasAttribute("name") && param.hasAttribute("value")){
                this->integerParameter.insert(param.attribute("name"), param.attribute("value").toInt());
            }
        }
    }

    //get double parameters
    QDomElement doubleParams = xmlElem.firstChildElement("doubleParameters");
    if(!doubleParams.isNull()){
        QDomNodeList params = doubleParams.childNodes();
        for(int i = 0; i < params.size(); ++i){
            QDomElement param = params.at(i).toElement();
            if(param.hasAttribute("name") && param.hasAttribute("value")){
                this->doubleParameter.insert(param.attribute("name"), param.attribute("value").toDouble());
            }
        }
    }

    //get string parameters
    QDomElement stringParams = xmlElem.firstChildElement("stringParameters");
    if(!stringParams.isNull()){
        QDomNodeList params = stringParams.childNodes();
        for(int i = 0; i < params.size(); ++i){
            QDomElement param = params.at(i).toElement();
            if(param.hasAttribute("name") && param.hasAttribute("value")){
                this->stringParameter.insert(param.attribute("name"), param.attribute("value"));
            }
        }
    }

    //get available string parameters
    QDomElement availableStringParams = xmlElem.firstChildElement("availableStringParameters");
    if(!availableStringParams.isNull()){
        QDomNodeList params = availableStringParams.childNodes();
        for(int i = 0; i < params.size(); ++i){
            QDomElement param = params.at(i).toElement();
            if(param.hasAttribute("name")){
                QDomNodeList values = param.childNodes();
                for(int j = 0; j < values.size(); j++){
                    QDomElement value = values.at(j).toElement();
                    if(value.hasAttribute("value")){
                        this->availableStringParameter.insertMulti(param.attribute("name"), value.attribute("value"));
                    }
                }
            }
        }
    }

    return true;

}
