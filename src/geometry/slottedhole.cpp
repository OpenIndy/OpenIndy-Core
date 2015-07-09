#include "slottedhole.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief SlottedHole::SlottedHole
 * \param isNominal
 * \param parent
 */
SlottedHole::SlottedHole(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSlottedHole(this);
    }

}

/*!
 * \brief SlottedHole::SlottedHole
 * \param isNominal
 * \param center
 * \param normal
 * \param radius
 * \param length
 * \param holeAxis
 * \param parent
 */
SlottedHole::SlottedHole(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radius, const double &length, const Direction &holeAxis, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSlottedHole(this);
    }

    this->setSlottedHole(center, normal, radius, length, holeAxis);

}

/*!
 * \brief SlottedHole::SlottedHole
 * \param isNominal
 * \param circleCenterA
 * \param circleCenterB
 * \param normal
 * \param radius
 * \param parent
 */
SlottedHole::SlottedHole(const bool &isNominal, const Position &circleCenterA, const Position &circleCenterB, const Direction &normal, const Radius &radius, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSlottedHole(this);
    }

    this->setSlottedHole(circleCenterA, circleCenterB, normal, radius);

}

/*!
 * \brief SlottedHole::SlottedHole
 * \param copy
 * \param parent
 */
SlottedHole::SlottedHole(const SlottedHole &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSlottedHole(this);
    }

    this->center = copy.center;
    this->normal = copy.normal;
    this->radius = copy.radius;
    this->length = copy.length;
    this->holeAxis = copy.holeAxis;
    this->circleCenterA = copy.circleCenterA;
    this->circleCenterB = copy.circleCenterB;

}

/*!
 * \brief SlottedHole::operator =
 * \param copy
 * \return
 */
SlottedHole &SlottedHole::operator=(const SlottedHole &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSlottedHole(this);
    }

    this->center = copy.center;
    this->normal = copy.normal;
    this->radius = copy.radius;
    this->length = copy.length;
    this->holeAxis = copy.holeAxis;
    this->circleCenterA = copy.circleCenterA;
    this->circleCenterB = copy.circleCenterB;

    return *this;

}

/*!
 * \brief SlottedHole::~SlottedHole
 */
SlottedHole::~SlottedHole(){

}

/*!
 * \brief SlottedHole::hasDirection
 * \return
 */
bool SlottedHole::hasDirection() const{
    return true;
}

/*!
 * \brief SlottedHole::hasPosition
 * \return
 */
bool SlottedHole::hasPosition() const{
    return true;
}

/*!
 * \brief SlottedHole::hasRadius
 * \return
 */
bool SlottedHole::hasRadius() const{
    return true;
}

/*!
 * \brief SlottedHole::getRadius
 * Returns the radius of the outer circles
 * \return
 */
const Radius &SlottedHole::getRadius() const{
    return this->radius;
}

/*!
 * \brief SlottedHole::getDirection
 * Returns the normal vector of the slotted hole
 * \return
 */
const Direction &SlottedHole::getDirection() const{
    return this->normal;
}

/*!
 * \brief SlottedHole::getPosition
 * Returns the center of the slotted hole
 * \return
 */
const Position &SlottedHole::getPosition() const{
    return this->center;
}

/*!
 * \brief SlottedHole::getCircleCenterA
 * Returns the center of one circle
 * \return
 */
const Position &SlottedHole::getCircleCenterA() const{
    return this->circleCenterA;
}

/*!
 * \brief SlottedHole::getCircleCenterB
 * Returns the center of the other circle
 * \return
 */
const Position &SlottedHole::getCircleCenterB() const{
    return this->circleCenterB;
}

/*!
 * \brief SlottedHole::getHoleAxis
 * Returns a vector pointing from circle center A to B
 * \return
 */
const Direction &SlottedHole::getHoleAxis() const{
    return this->holeAxis;
}

/*!
 * \brief SlottedHole::getLength
 * \return
 */
const double &SlottedHole::getLength() const{
    return this->length;
}

/*!
 * \brief SlottedHole::setSlottedHole
 * \param center
 * \param normal
 * \param radius
 * \param length
 * \param holeAxis
 */
void SlottedHole::setSlottedHole(const Position &center, const Direction &normal, const Radius &radius, const double &length, const Direction &holeAxis){

    //set the given parameters
    this->center = center;
    this->normal = normal;
    this->radius = radius;
    this->length = length;
    this->holeAxis = holeAxis;

    //calculate the other parameters
    OiVec axis = this->holeAxis.getVector();
    axis.normalize();
    this->circleCenterA.setVector(this->center.getVector() + (this->length / 2 - this->radius.getRadius()) * axis);
    this->circleCenterB.setVector(this->center.getVector() - (this->length / 2 - this->radius.getRadius()) * axis);

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief SlottedHole::setSlottedHole
 * \param circleCenterA
 * \param circleCenterB
 * \param normal
 * \param radius
 */
void SlottedHole::setSlottedHole(const Position &circleCenterA, const Position &circleCenterB, const Direction &normal, const Radius &radius){

    //set the given parameters
    this->circleCenterA = circleCenterA;
    this->circleCenterB = circleCenterB;
    this->normal = normal;
    this->radius = radius;

    //calculate the other parameters
    this->center.setVector(this->circleCenterA.getVector() + 0.5*(this->circleCenterB.getVector() - this->circleCenterA.getVector()));
    OiVec axis = this->circleCenterB.getVector() - this->circleCenterA.getVector();
    OiVec::dot(this->length, axis, axis);
    axis.normalize();
    this->holeAxis.setVector(axis);

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief SlottedHole::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<UnknownParameters, QString> SlottedHole::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<UnknownParameters, QString> parameters;

    parameters.insert(eUnknownX, this->getDisplayX(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownY, this->getDisplayY(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownZ, this->getDisplayZ(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownPrimaryI, this->getDisplayPrimaryI(displayDigits.value(eDimensionless)));
    parameters.insert(eUnknownPrimaryJ, this->getDisplayPrimaryJ(displayDigits.value(eDimensionless)));
    parameters.insert(eUnknownPrimaryK, this->getDisplayPrimaryK(displayDigits.value(eDimensionless)));
    parameters.insert(eUnknownRadiusA, this->getDisplayRadiusA(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownLength, this->getDisplayLength(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownSecondaryI, this->getDisplaySecondaryI(displayDigits.value(eDimensionless)));
    parameters.insert(eUnknownSecondaryJ, this->getDisplaySecondaryJ(displayDigits.value(eDimensionless)));
    parameters.insert(eUnknownSecondaryK, this->getDisplaySecondaryK(displayDigits.value(eDimensionless)));

    return parameters;

}

/*!
 * \brief SlottedHole::setUnknownParameters
 * \param parameters
 */
void SlottedHole::setUnknownParameters(const QMap<UnknownParameters, double> &parameters){

    //get current parameters
    OiVec position = this->center.getVector();
    OiVec directionA = this->normal.getVector();
    OiVec directionB = this->holeAxis.getVector();
    double radius = this->radius.getRadius();
    double length = this->length;

    //update parameters
    QList<UnknownParameters> keys = parameters.keys();
    foreach(const UnknownParameters &key, keys){
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
            directionA.setAt(0, parameters.value(eUnknownPrimaryI));
            break;
        case eUnknownPrimaryJ:
            directionA.setAt(1, parameters.value(eUnknownPrimaryJ));
            break;
        case eUnknownPrimaryK:
            directionA.setAt(2, parameters.value(eUnknownPrimaryK));
            break;
        case eUnknownRadiusA:
            radius = parameters.value(eUnknownRadiusA);
            break;
        case eUnknownLength:
            length = parameters.value(eUnknownLength);
            break;
        case eUnknownSecondaryI:
            directionB.setAt(0, parameters.value(eUnknownSecondaryI));
            break;
        case eUnknownSecondaryJ:
            directionB.setAt(1, parameters.value(eUnknownSecondaryJ));
            break;
        case eUnknownSecondaryK:
            directionB.setAt(2, parameters.value(eUnknownSecondaryK));
            break;
        }
    }

    //update slotted hole definition
    directionA.normalize();
    directionB.normalize();
    Position holePosition(position);
    Direction holeDirectionA(directionA);
    Direction holeDirectionB(directionB);
    Radius holeRadius(radius);
    this->setSlottedHole(holePosition, holeDirectionA, holeRadius, length, holeDirectionB);

}

/*!
 * \brief SlottedHole::recalc
 */
void SlottedHole::recalc(){

    Geometry::recalc();

    //reset slotted hole definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->center.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->radius.setRadius(0.0);
        this->length = 0.0;
        this->holeAxis.setVector(0.0, 0.0, 0.0);
        this->circleCenterA.setVector(0.0, 0.0, 0.0);
        this->circleCenterB.setVector(0.0, 0.0, 0.0);
    }

}

/*!
 * \brief SlottedHole::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement SlottedHole::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement slottedHole = Geometry::toOpenIndyXML(xmlDoc);

    if(slottedHole.isNull()){
        return slottedHole;
    }

    slottedHole.setAttribute("type", getGeometryTypeName(eNurbsGeometry));

    //set length
    QDomElement length = xmlDoc.createElement("length");
    if(this->isSolved || this->isNominal){
        length.setAttribute("value", this->length);
    }else{
        length.setAttribute("value", 0.0);
    }
    slottedHole.appendChild(length);

    //set hole axis
    QDomElement holeAxis = xmlDoc.createElement("holeAxis");
    if(this->isSolved || this->isNominal){
        holeAxis.setAttribute("i", this->holeAxis.getVector().getAt(0));
        holeAxis.setAttribute("j", this->holeAxis.getVector().getAt(1));
        holeAxis.setAttribute("k", this->holeAxis.getVector().getAt(2));
    }else{
        holeAxis.setAttribute("i", 0.0);
        holeAxis.setAttribute("j", 0.0);
        holeAxis.setAttribute("k", 0.0);
    }
    slottedHole.appendChild(holeAxis);

    return slottedHole;

}

/*!
 * \brief SlottedHole::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool SlottedHole::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set slotted hole attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement length = xmlElem.firstChildElement("length");
        QDomElement holeAxis = xmlElem.firstChildElement("holeAxis");
        QDomElement normal = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || length.isNull() || holeAxis.isNull() || normal.isNull() || center.isNull()
                || !radius.hasAttribute("value") || !length.hasAttribute("value") || holeAxis.hasAttribute("i")
                || !holeAxis.hasAttribute("j") || !holeAxis.hasAttribute("k")
                || !normal.hasAttribute("i") || normal.hasAttribute("j") || normal.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius.setRadius(radius.attribute("value").toDouble());
        this->length = length.attribute("value").toDouble();
        this->holeAxis.setVector(holeAxis.attribute("i").toDouble(),
                                 holeAxis.attribute("j").toDouble(),
                                 holeAxis.attribute("k").toDouble());
        this->normal.setVector(normal.attribute("i").toDouble(),
                               normal.attribute("j").toDouble(),
                               normal.attribute("k").toDouble());
        this->center.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief SlottedHole::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplayRadiusA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->radius.getRadius(), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplaySecondaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplaySecondaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->holeAxis.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplaySecondaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplaySecondaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->holeAxis.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplaySecondaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplaySecondaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->holeAxis.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief SlottedHole::getDisplayLength
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString SlottedHole::getDisplayLength(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->length, type), 'f', digits);
    }
    return QString("");
}
