#include "function.h"

using namespace oi;

/*!
 * \brief Function::Function
 * \param parent
 */
Function::Function(QObject *parent) : QObject(parent){

    this->supportsWeights = false;
}

/*!
 * \brief Function::~Function
 */
Function::~Function(){

}

/*!
 * \brief Function::init
 */
void Function::init(){

}

/*!
 * \brief Function::exec
 * \param feature
 * \return
 */
bool Function::exec(const QPointer<FeatureWrapper> &feature){

    if(!feature.isNull()){

        switch(feature->getFeatureTypeEnum()){
        case eStationFeature:
            if(!feature->getStation().isNull()){ return this->exec(*feature->getStation()); }
        case eTrafoParamFeature:
            if(!feature->getTrafoParam().isNull()){ return this->exec(*feature->getTrafoParam()); }
        case eCoordinateSystemFeature:
            if(!feature->getCoordinateSystem().isNull()){ return this->exec(*feature->getCoordinateSystem()); }
        case eCircleFeature:
            if(!feature->getCircle().isNull()){ return this->exec(*feature->getCircle()); }
        case eConeFeature:
            if(!feature->getCone().isNull()){ return this->exec(*feature->getCone()); }
        case eCylinderFeature:
            if(!feature->getCylinder().isNull()){ return this->exec(*feature->getCylinder()); }
        case eEllipseFeature:
            if(!feature->getEllipse().isNull()){ return this->exec(*feature->getEllipse()); }
        case eEllipsoidFeature:
            if(!feature->getEllipsoid().isNull()){ return this->exec(*feature->getEllipsoid()); }
        case eHyperboloidFeature:
            if(!feature->getHyperboloid().isNull()){ return this->exec(*feature->getHyperboloid()); }
        case eLineFeature:
            if(!feature->getLine().isNull()){ return this->exec(*feature->getLine()); }
        case eNurbsFeature:
            if(!feature->getNurbs().isNull()){ return this->exec(*feature->getNurbs()); }
        case eParaboloidFeature:
            if(!feature->getParaboloid().isNull()){ return this->exec(*feature->getParaboloid()); }
        case ePlaneFeature:
            if(!feature->getPlane().isNull()){ return this->exec(*feature->getPlane()); }
        case ePointFeature:
            if(!feature->getPoint().isNull()){ return this->exec(*feature->getPoint()); }
        case ePointCloudFeature:
            if(!feature->getPointCloud().isNull()){ return this->exec(*feature->getPointCloud()); }
        case eScalarEntityAngleFeature:
            if(!feature->getScalarEntityAngle().isNull()){ return this->exec(*feature->getScalarEntityAngle()); }
        case eScalarEntityDistanceFeature:
            if(!feature->getScalarEntityDistance().isNull()){ return this->exec(*feature->getScalarEntityDistance()); }
        case eScalarEntityMeasurementSeriesFeature:
            if(!feature->getScalarEntityMeasurementSeries().isNull()){ return this->exec(*feature->getScalarEntityMeasurementSeries()); }
        case eScalarEntityTemperatureFeature:
            if(!feature->getScalarEntityTemperature().isNull()){ return this->exec(*feature->getScalarEntityTemperature()); }
        case eSlottedHoleFeature:
            if(!feature->getSlottedHole().isNull()){ return this->exec(*feature->getSlottedHole()); }
        case eSphereFeature:
            if(!feature->getSphere().isNull()){ return this->exec(*feature->getSphere()); }
        case eTorusFeature:
            if(!feature->getTorus().isNull()){ return this->exec(*feature->getTorus()); }
        }

    }

    return false;

}

/*!
 * \brief Function::getNeededElements
 * \return
 */
const QList<NeededElement> &Function::getNeededElements() const{
    return this->neededElements;
}

/*!
 * \brief Function::getApplicableFor
 * \return
 */
const QList<FeatureTypes> &Function::getApplicableFor() const{
    return this->applicableFor;
}

/*!
 * \brief Function::getMetaData
 * \return
 */
const PluginMetaData &Function::getMetaData() const{
    return this->metaData;
}

/*!
 * \brief Function::getIntegerParameter
 * \return
 */
const QMap<QString, int> &Function::getIntegerParameter() const{
    return this->integerParameters;
}

/*!
 * \brief Function::getDoubleParameter
 * \return
 */
const QMap<QString, double> &Function::getDoubleParameter() const{
    return this->doubleParameters;
}

/*!
 * \brief Function::getStringParameter
 * \return
 */
const QMultiMap<QString, QString> &Function::getStringParameter() const{
    return this->stringParameters;
}

/*!
 * \brief Function::setScalarInputParams
 * \param params
 */
void Function::setScalarInputParams(const ScalarInputParams &params){
    this->scalarInputParams = params;
    this->scalarInputParams.isValid = true;
    emit this->scalarInputParametersChanged();
}

/*!
 * \brief Function::getScalarInputParams
 * \return
 */
const ScalarInputParams &Function::getScalarInputParams(){

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
 * \brief Function::getFixedParameters
 * \return
 */
const QList<FixedParameter> &Function::getFixedParameters() const{
    return this->fixedParameters;
}

/*!
 * \brief Function::fixParameter
 * \param parameter
 */
void Function::fixParameter(const FixedParameter &parameter){
    if(!this->fixedParameters.contains(parameter)){
        this->fixedParameters.append(parameter);
    }
}

/*!
 * \brief Function::unfixParameter
 * \param parameter
 */
void Function::unfixParameter(const GeometryParameters &parameter){
    this->fixedParameters.removeOne(FixedParameter(parameter));
}

/*!
 * \brief Function::unfixAllParameters
 */
void Function::unfixAllParameters(){
    this->fixedParameters.clear();
}

/*!
 * \brief Function::getResultProtocol
 * \return
 */
const QStringList &Function::getResultProtocol() const{
    return this->resultProtocol;
}

/*!
 * \brief Function::getStatistic
 * \return
 */
const Statistic &Function::getStatistic() const{
    return this->statistic;
}

/*!
 * \brief Function::getId
 * \return
 */
const int &Function::getId() const{
    return this->id;
}

/*!
 * \brief Function::getSupportWeights
 * \return
 */
const bool &Function::getSupportWeights()
{
    return this->supportsWeights;
}

/*!
 * \brief Function::setSupportWeights
 * \return
 */
void Function::setSupportWeights(bool supports)
{
    this->supportsWeights = supports;
}

/*!
 * \brief Function::getInputElements
 * \return
 */
const QMap<int, QList<InputElement> > &Function::getInputElements() const{
    return this->inputElements;
}

/*!
 * \brief Function::getInputElement
 * Returns the InputElement with the specified id or an empty InputElement
 * \param id
 * \return
 */
InputElement Function::getInputElement(const int &id) const{

    QList<int> keys = this->inputElements.keys();
    foreach(const int &key, keys){
        foreach(const InputElement &element, this->inputElements[key]){
            if(element.id == id){
                return element;
            }
        }
    }
    return InputElement();

}

/*!
 * \brief Function::getInputElement
 * Returns the InputElement with the specified id at the specified position
 * \param id
 * \param position
 * \return
 */
InputElement Function::getInputElement(const int &id, const int &position) const{

    if(this->inputElements.contains(position)){
        foreach(const InputElement &element, this->inputElements[position]){
            if(element.id == id){
                return element;
            }
        }
    }
    return InputElement();

}

/*!
 * \brief Function::hasInputElement
 * Check wether the function contains the specified InputElement
 * \param id
 * \return
 */
bool Function::hasInputElement(const int &id) const{

    QList<int> keys = this->inputElements.keys();
    foreach(const int &key, keys){
        foreach(const InputElement &element, this->inputElements[key]){
            if(element.id == id){
                return true;
            }
        }
    }
    return false;

}

/*!
 * \brief Function::addInputElement
 * \param element
 * \param position
 */
void Function::addInputElement(const InputElement &element, const int &position){

    //add the input element
    if(this->inputElements.contains(position)){
        this->inputElements[position].append(element);
    }else{
        QList<InputElement> elements;
        elements.append(element);
        this->inputElements.insert(position, elements);
    }

    emit this->inputElementsChanged();

}

/*!
 * \brief Function::removeInputElement
 * \param id
 * \param position
 */
void Function::removeInputElement(const int &id, const int &position){
    if(this->inputElements.contains(position)){
        this->inputElements[position].removeOne(InputElement(id));
        emit this->inputElementsChanged();
    }
}

/*!
 * \brief Function::removeInputElement
 * \param id
 */
void Function::removeInputElement(const int &id){
    for(int i = 0; i < this->inputElements.size(); i++){
        this->inputElements[i].removeOne(InputElement(id));
        emit this->inputElementsChanged();
    }
}

/*!
 * \brief Function::replaceInputElement
 * \param element
 * \param position
 */
void Function::replaceInputElement(const InputElement &element, const int &position){
    if(this->inputElements.contains(position)){
        int index = this->inputElements[position].indexOf(element);
        if(index > -1){
            this->inputElements[position].replace(index, element);
            emit this->inputElementsChanged();
        }
    }
}

/*!
 * \brief Function::getIsUsed
 * \param position
 * \param id
 * \return
 */
bool Function::getIsUsed(const int &position, const int &id){

    if(this->inputElements.contains(position)){
        int index = this->inputElements[position].indexOf(id);
        if(index > -1){
            return this->inputElements[position][index].isUsed;
        }
    }

    return false;

}

/*!
 * \brief Function::getShouldBeUsed
 * \param position
 * \param id
 * \return
 */
bool Function::getShouldBeUsed(const int &position, const int &id){

    if(this->inputElements.contains(position)){
        int index = this->inputElements[position].indexOf(id);
        if(index > -1){
            return this->inputElements[position][index].shouldBeUsed;
        }
    }

    return false;

}

/*!
 * \brief Function::setShouldBeUsed
 * \param position
 * \param id
 * \param state
 */
void Function::setShouldBeUsed(const int &position, const int &id, const bool &state){

    //get the element at position with id and set shouldBeUsed to state
    if(this->inputElements.contains(position)){
        int index = this->inputElements[position].indexOf(id);
        if(index > -1){
            this->inputElements[position][index].shouldBeUsed = state;
            emit this->inputElementsChanged();
        }
    }

}

/*!
 * \brief Function::clear
 */
void Function::clear(){
    this->inputElements.clear();
    this->fixedParameters.clear();
    this->scalarInputParams.isValid = false;
    this->resultProtocol.clear();
    this->statistic.setIsValid(false);

    emit this->inputElementsChanged();
}

/*!
 * \brief Function::clearResults
 */
void Function::clearResults(){

    //clear statistic and protocol
    this->resultProtocol.clear();
    this->statistic.setIsValid(false);

    //set all elements to not have been used
    QList<int> keys = this->inputElements.keys();
    foreach(const int &key, keys){
        for(int i = 0; i < this->inputElements[key].size(); ++i){
            this->inputElements[key][i].isUsed = false;
        }
    }

}

/*!
 * \brief Function::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Function::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    //add function attributes
    QDomElement function = xmlDoc.createElement("function");
    function.setAttribute("name", this->getMetaData().name);
    function.setAttribute("type", this->getMetaData().iid);
    function.setAttribute("plugin", this->getMetaData().pluginName);

    //add used elements
    QDomElement inputElements = xmlDoc.createElement("inputElements");
    QMapIterator<int, QList<InputElement> > inputIterator(this->getInputElements());
    while (inputIterator.hasNext()) {
        inputIterator.next();
        foreach(const InputElement &input, inputIterator.value()){
            QDomElement inputElement = xmlDoc.createElement("element");
            inputElement.setAttribute("index", inputIterator.key());
            inputElement.setAttribute("type", input.typeOfElement);
            inputElement.setAttribute("ref", input.id);
            inputElement.setAttribute("isUsed", input.isUsed);
            inputElement.setAttribute("shouldBeUsed", input.shouldBeUsed);

            //add ignored destination parameters of the input element
            QDomElement ignoredDestinationParams = xmlDoc.createElement("ignoredDestinationParams");

            for(int i=0; i< input.ignoredDestinationParams.size();i++){
                //add all ignored parameters to the element
                QDomElement ignoredParam = xmlDoc.createElement("ignoredParam");
                ignoredParam.setAttribute("parameter", getGeometryParameterName(input.ignoredDestinationParams.at(i)));
                ignoredDestinationParams.appendChild(ignoredParam);
            }
            inputElement.appendChild(ignoredDestinationParams);

            inputElements.appendChild(inputElement);
        }
    }
    function.appendChild(inputElements);

    ScalarInputParams params;
    if(!this->scalarInputParams.isValid){
        params.doubleParameter = this->doubleParameters;
        params.intParameter = this->integerParameters;
        QStringList keys = this->stringParameters.keys();
        foreach(const QString &key, keys){
            params.stringParameter.insert(key, this->stringParameters.value(key));
        }
    }else{
        params = this->scalarInputParams;
    }

    //add integer parameters
    QDomElement integerParams = xmlDoc.createElement("integerParameters");
    QMapIterator<QString, int> intIterator(params.intParameter);
    while (intIterator.hasNext()) {
        intIterator.next();
        QDomElement intParam = xmlDoc.createElement("parameter");
        intParam.setAttribute("name", intIterator.key());
        intParam.setAttribute("value", intIterator.value());
        integerParams.appendChild(intParam);
    }
    function.appendChild(integerParams);

    //add double parameters
    QDomElement doubleParams = xmlDoc.createElement("doubleParameters");
    QMapIterator<QString, double> doubleIterator(params.doubleParameter);
    while (doubleIterator.hasNext()) {
        doubleIterator.next();
        QDomElement doubleParam = xmlDoc.createElement("parameter");
        doubleParam.setAttribute("name", doubleIterator.key());
        doubleParam.setAttribute("value", doubleIterator.value());
        doubleParams.appendChild(doubleParam);
    }
    function.appendChild(doubleParams);

    //add string parameters
    QDomElement stringParams = xmlDoc.createElement("stringParameters");
    QMapIterator<QString, QString> stringIterator(params.stringParameter);
    while (stringIterator.hasNext()) {
        stringIterator.next();
        QDomElement stringParam = xmlDoc.createElement("parameter");
        stringParam.setAttribute("name", stringIterator.key());
        stringParam.setAttribute("value", stringIterator.value());
        stringParams.appendChild(stringParam);
    }
    function.appendChild(stringParams);

    return function;

}

/*!
 * \brief Function::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Function::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    if(!xmlElem.hasAttribute("name") || !xmlElem.hasAttribute("plugin") || !xmlElem.hasAttribute("executionIndex")
            || !xmlElem.hasAttribute("type")){
        return false;
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
 * \brief Function::exec
 * \param station
 * \return
 */
bool Function::exec(Station &station){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type station", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param coordinateSystem
 * \return
 */
bool Function::exec(CoordinateSystem &coordinateSystem){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type coordinate system", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param trafoParam
 * \return
 */
bool Function::exec(TrafoParam &trafoParam){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type transformation parameter", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param circle
 * \return
 */
bool Function::exec(Circle &circle){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type circle", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param cone
 * \return
 */
bool Function::exec(Cone &cone){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type cone", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param cylinder
 * \return
 */
bool Function::exec(Cylinder &cylinder){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type cylinder", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param ellipse
 * \return
 */
bool Function::exec(Ellipse &ellipse){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type ellipse", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param ellipsoid
 * \return
 */
bool Function::exec(Ellipsoid &ellipsoid){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type ellipsoid", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param hyperboloid
 * \return
 */
bool Function::exec(Hyperboloid &hyperboloid){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type hyperboloid", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param line
 * \return
 */
bool Function::exec(Line &line){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type line", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param nurbs
 * \return
 */
bool Function::exec(Nurbs &nurbs){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type nurbs", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param paraboloid
 * \return
 */
bool Function::exec(Paraboloid &paraboloid){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type paraboloid", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param plane
 * \return
 */
bool Function::exec(Plane &plane){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type plane", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param point
 * \return
 */
bool Function::exec(Point &point){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type point", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param pointCloud
 * \return
 */
bool Function::exec(PointCloud &pointCloud){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type point cloud", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param angle
 * \return
 */
bool Function::exec(ScalarEntityAngle &angle){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type angle", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param distance
 * \return
 */
bool Function::exec(ScalarEntityDistance &distance){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type distance", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param measurementSeries
 * \return
 */
bool Function::exec(ScalarEntityMeasurementSeries &measurementSeries){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type measurement series", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param temperature
 * \return
 */
bool Function::exec(ScalarEntityTemperature &temperature){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type temperature", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param slottedHole
 * \return
 */
bool Function::exec(SlottedHole &slottedHole){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type slotted hole", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param sphere
 * \return
 */
bool Function::exec(Sphere &sphere){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type sphere", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::exec
 * \param torus
 * \return
 */
bool Function::exec(Torus &torus){
    emit this->sendMessage("Function \"" + this->getMetaData().name + "\" not implemented for type torus", eCriticalMessage, eMessageBoxMessage);
    return false;
}

/*!
 * \brief Function::setIsUsed
 * \param position
 * \param id
 * \param state
 */
void Function::setIsUsed(const int &position, const int &id, const bool &state){

    //get the element at position with id and set isUsed to state
    if(this->inputElements.contains(position)){
        int index = this->inputElements[position].indexOf(id);
        if(index > -1){
            this->inputElements[position][index].isUsed = state;
        }
    }

}
