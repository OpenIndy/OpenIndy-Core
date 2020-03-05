#include "cone.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Cone::Cone
 * \param isNominal
 * \param parent
 */
Cone::Cone(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

}

/*!
 * \brief Cone::Cone
 * \param isNominal
 * \param apex
 * \param axis
 * \param aperture
 * \param parent
 */
Cone::Cone(const bool &isNominal, const Position &apex, const Direction &axis, const double &aperture, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

    this->setCone(apex, axis, aperture);

}

/*!
 * \brief Cone::Cone
 * \param copy
 * \param parent
 */
Cone::Cone(const Cone &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->aperture = copy.aperture;

}

/*!
 * \brief Cone::operator =
 * \param copy
 * \return
 */
Cone &Cone::operator=(const Cone &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCone(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->aperture = copy.aperture;

    return *this;

}

/*!
 * \brief Cone::~Cone
 */
Cone::~Cone(){

}

/*!
 * \brief Cone::hasDirection
 * \return
 */
bool Cone::hasDirection() const{
    return true;
}

/*!
 * \brief Cone::hasPosition
 * \return
 */
bool Cone::hasPosition() const{
    return true;
}

/*!
 * \brief Cone::getAperture
 * Returns the aperture of the cone
 * \return
 */
const double &Cone::getAperture() const{
    return this->aperture;
}

/*!
 * \brief Cone::setCone
 * \param apex
 * \param axis
 * \param aperture
 */
void Cone::setCone(const Position &apex, const Direction &axis, const double &aperture){

    //set the given parameters
    this->xyz = apex;
    this->ijk = axis;
    this->aperture = aperture;

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Cone::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Cone::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<GeometryParameters, QString> parameters;

    parameters.insert(eUnknownX, this->getDisplayX(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownY, this->getDisplayY(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownZ, this->getDisplayZ(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownPrimaryI, this->getDisplayPrimaryI(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryJ, this->getDisplayPrimaryJ(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryK, this->getDisplayPrimaryK(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownAperture, this->getDisplayAperture(displayUnits.value(eAngular, eUnitRadiant), displayDigits.value(eAngular, 0)));

    return parameters;

}

/*!
 * \brief Cone::setUnknownParameters
 * \param parameters
 */
void Cone::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    OiVec position = this->xyz.getVector();
    OiVec direction = this->ijk.getVector();
    double aperture = this->getAperture();

    //update parameters
    QList<GeometryParameters> keys = parameters.keys();
    foreach(const GeometryParameters &key, keys){
        switch(key){
        case eUnknownX:
            position.setAt(0, parameters.value(eUnknownX));
            break;
        case eUnknownY:
            position.setAt(1, parameters.value(eUnknownY));
            break;
        case eUnknownZ:
            position.setAt(2, parameters.value(eUnknownZ));
            break;
        case eUnknownPrimaryI:
            direction.setAt(0, parameters.value(eUnknownPrimaryI));
            break;
        case eUnknownPrimaryJ:
            direction.setAt(1, parameters.value(eUnknownPrimaryJ));
            break;
        case eUnknownPrimaryK:
            direction.setAt(2, parameters.value(eUnknownPrimaryK));
            break;
        case eUnknownAperture:
            aperture = parameters.value(eUnknownAperture);
            break;
        }
    }

    //update cone definition
    direction.normalize();
    Position conePosition(position);
    Direction coneDirection(direction);
    this->setCone(conePosition, coneDirection, aperture);

}

/*!
 * \brief Cone::recalc
 */
void Cone::recalc(){

    Geometry::recalc();

    //reset cone definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->ijk.setVector(0.0, 0.0, 0.0);
        this->aperture = 0.0;
    }

}

/*!
 * \brief Cone::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Cone::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement cone = Geometry::toOpenIndyXML(xmlDoc);

    if(cone.isNull()){
        return cone;
    }

    cone.setAttribute("type", getGeometryTypeName(eConeGeometry));

    //set aperture
    QDomElement aperture = xmlDoc.createElement("aperture");
    if(this->isSolved || this->isNominal){
        aperture.setAttribute("value", this->aperture);
    }else{
        aperture.setAttribute("value", 0.0);
    }
    cone.appendChild(aperture);

    return cone;

}

/*!
 * \brief Cone::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Cone::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set cone attributes
        QDomElement aperture = xmlElem.firstChildElement("aperture");
        QDomElement axis = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(aperture.isNull() || axis.isNull() || center.isNull() || !aperture.hasAttribute("value")
                || !axis.hasAttribute("i") || !axis.hasAttribute("j") || !axis.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->aperture = aperture.attribute("value").toDouble();
        this->ijk.setVector(axis.attribute("i").toDouble(),
                             axis.attribute("j").toDouble(),
                             axis.attribute("k").toDouble());
        this->xyz.setVector(center.attribute("x").toDouble(),
                             center.attribute("y").toDouble(),
                             center.attribute("z").toDouble());

    }

    return result;

}
