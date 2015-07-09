#include "scalarentitydistance.h"

#include "featurewrapper.h"

using namespace oi;

/*!
 * \brief ScalarEntityDistance::ScalarEntityDistance
 * \param isNominal
 * \param parent
 */
ScalarEntityDistance::ScalarEntityDistance(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityDistance(this);
    }

}

/*!
 * \brief ScalarEntityDistance::ScalarEntityDistance
 * \param isNominal
 * \param distance
 * \param parent
 */
ScalarEntityDistance::ScalarEntityDistance(const bool &isNominal, const double &distance, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityDistance(this);
    }

    this->setDistance(distance);

}

/*!
 * \brief ScalarEntityDistance::ScalarEntityDistance
 * \param copy
 * \param parent
 */
ScalarEntityDistance::ScalarEntityDistance(const ScalarEntityDistance &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityDistance(this);
    }

    this->distance = copy.distance;

}

/*!
 * \brief ScalarEntityDistance::operator =
 * \param copy
 * \return
 */
ScalarEntityDistance &ScalarEntityDistance::operator=(const ScalarEntityDistance &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityDistance(this);
    }

    this->distance = copy.distance;

    return *this;

}

/*!
 * \brief ScalarEntityDistance::~ScalarEntityDistance
 */
ScalarEntityDistance::~ScalarEntityDistance(){

}

/*!
 * \brief ScalarEntityDistance::getDistance
 * \return
 */
const double &ScalarEntityDistance::getDistance() const{
    return this->distance;
}

/*!
 * \brief ScalarEntityDistance::setDistance
 * \param distance
 */
void ScalarEntityDistance::setDistance(const double &distance){

    this->distance = distance;

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief ScalarEntityDistance::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> ScalarEntityDistance::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<GeometryParameters, QString> parameters;

    parameters.insert(eUnknownDistance, this->getDisplayDistance(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));

    return parameters;

}

/*!
 * \brief ScalarEntityDistance::setUnknownParameters
 * \param parameters
 */
void ScalarEntityDistance::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    double distance = this->distance;

    //update parameters
    QList<GeometryParameters> keys = parameters.keys();
    foreach(const GeometryParameters &key, keys){
        switch(key){
        case eUnknownDistance:
            distance = parameters.value(eUnknownDistance);
            break;
        }
    }

    //update distance definition
    this->setDistance(distance);

}

/*!
 * \brief ScalarEntityDistance::recalc
 */
void ScalarEntityDistance::recalc(){

    Geometry::recalc();

    //reset distance definition if not solved
    if(!this->isSolved){
        this->distance = 0.0;
    }

}

/*!
 * \brief ScalarEntityDistance::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement ScalarEntityDistance::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityDistance = Geometry::toOpenIndyXML(xmlDoc);

    if(entityDistance.isNull()){
        return entityDistance;
    }

    entityDistance.setAttribute("type", getGeometryTypeName(eScalarEntityDistanceGeometry));

    //add distance
    QDomElement distance = xmlDoc.createElement("distance");
    if(this->isSolved || this->isNominal){
        distance.setAttribute("value", this->distance);
    }else{
        distance.setAttribute("value", 0.0);
    }
    entityDistance.appendChild(distance);

    return entityDistance;

}

/*!
 * \brief ScalarEntityDistance::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool ScalarEntityDistance::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set distance attributes
        QDomElement distance = xmlElem.firstChildElement("distance");

        if(distance.isNull() || !distance.hasAttribute("value")){
            return false;
        }

        this->distance = distance.attribute("value").toDouble();

    }

    return result;

}

/*!
 * \brief ScalarEntityDistance::getDisplayDistance
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString ScalarEntityDistance::getDisplayDistance(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->distance, type), 'f', digits);
    }
    return QString("");
}
