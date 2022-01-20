#include "line.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Line::Line
 * \param isNominal
 * \param parent
 */
Line::Line(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

}

/*!
 * \brief Line::Line
 * \param isNominal
 * \param xyz
 * \param axis
 * \param parent
 */
Line::Line(const bool &isNominal, const Position &xyz, const Direction &axis, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

    this->setLine(xyz, axis);

}

/*!
 * \brief Line::Line
 * \param copy
 * \param parent
 */
Line::Line(const Line &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;

}

/*!
 * \brief Line::operator =
 * \param copy
 * \return
 */
Line &Line::operator=(const Line &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setLine(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;

    return *this;

}

/*!
 * \brief Line::~Line
 */
Line::~Line(){

}

/*!
 * \brief Line::hasDirection
 * \return
 */
bool Line::hasDirection() const{
    return true;
}

/*!
 * \brief Line::hasPosition
 * \return
 */
bool Line::hasPosition() const{
    return true;
}

/*!
 * \brief Line::setLine
 * \param xyz
 * \param axis
 */
void Line::setLine(const Position &xyz, const Direction &axis){

    //set the given parameters
    this->xyz = xyz;
    this->ijk = axis;

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Line::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Line::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<GeometryParameters, QString> parameters;

    parameters.insert(eUnknownX, this->getDisplayX(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownY, this->getDisplayY(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownZ, this->getDisplayZ(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownPrimaryI, this->getDisplayPrimaryI(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryJ, this->getDisplayPrimaryJ(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryK, this->getDisplayPrimaryK(displayDigits.value(eDimensionless, 0)));

    return parameters;

}

/*!
 * \brief Line::setUnknownParameters
 * \param parameters
 */
void Line::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    OiVec position = this->xyz.getVector();
    OiVec direction = this->ijk.getVector();

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
        }
    }

    //update line definition
    direction.normalize();
    Position linePosition(position);
    Direction lineDirection(direction);
    this->setLine(linePosition, lineDirection);

}

/*!
 * \brief Line::recalc
 */
void Line::recalc(){

    Geometry::recalc();

    //reset line definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->ijk.setVector(0.0, 0.0, 0.0);
    }

}

/*!
 * \brief Line::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Line::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement line = Geometry::toOpenIndyXML(xmlDoc);

    if(line.isNull()){
        return line;
    }

    line.setAttribute("type", getGeometryTypeName(eLineGeometry));

    return line;

}

/*!
 * \brief Line::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Line::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set line attributes
        QDomElement directionVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement axisPoint = xmlElem.firstChildElement("coordinates");

        if(directionVector.isNull() || axisPoint.isNull()
                || !directionVector.hasAttribute("i") || !directionVector.hasAttribute("j") || !directionVector.hasAttribute("k")
                || !axisPoint.hasAttribute("x") || !axisPoint.hasAttribute("y") || !axisPoint.hasAttribute("z")){
            return false;
        }

        this->ijk.setVector(directionVector.attribute("i").toDouble(),
                               directionVector.attribute("j").toDouble(),
                               directionVector.attribute("k").toDouble());
        this->xyz.setVector(axisPoint.attribute("x").toDouble(),
                               axisPoint.attribute("y").toDouble(),
                               axisPoint.attribute("z").toDouble());

    }

    return result;

}
