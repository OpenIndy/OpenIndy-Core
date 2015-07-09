#include "circle.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Circle::Circle
 * \param isNominal
 * \param parent
 */
Circle::Circle(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

}

/*!
 * \brief Circle::Circle
 * \param isNominal
 * \param center
 * \param normal
 * \param radius
 * \param parent
 */
Circle::Circle(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radius, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

    this->setCircle(center, normal, radius);

}

/*!
 * \brief Circle::Circle
 * \param copy
 * \param parent
 */
Circle::Circle(const Circle &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

    this->center = copy.center;
    this->radius = copy.radius;
    this->normal = copy.normal;

}

/*!
 * \brief Circle::operator =
 * \param copy
 * \return
 */
Circle &Circle::operator=(const Circle &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setCircle(this);
    }

    this->center = copy.center;
    this->radius = copy.radius;
    this->normal = copy.normal;

    return *this;

}

Circle::~Circle()
{

}

/*!
 * \brief Circle::hasDirection
 * \return
 */
bool Circle::hasDirection() const{
    return true;
}

/*!
 * \brief Circle::hasPosition
 * \return
 */
bool Circle::hasPosition() const{
    return true;
}

/*!
 * \brief Circle::hasRadius
 * \return
 */
bool Circle::hasRadius() const{
    return true;
}

/*!
 * \brief Circle::getRadius
 * Returns the radius of the circle
 * \return
 */
const Radius &Circle::getRadius() const{
    return this->radius;
}

/*!
 * \brief Circle::getDirection
 * Returns the normal vector of the circle
 * \return
 */
const Direction &Circle::getDirection() const{
    return this->normal;
}

/*!
 * \brief Circle::getPosition
 * Returns the center of the circle
 * \return
 */
const Position &Circle::getPosition() const{
    return this->center;
}

/*!
 * \brief Circle::setCircle
 * \param center
 * \param normal
 * \param radius
 */
void Circle::setCircle(const Position &center, const Direction &normal, const Radius &radius){

    //set the given parameters
    this->center = center;
    this->normal = normal;
    this->radius = radius;

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Circle::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Circle::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<GeometryParameters, QString> parameters;

    parameters.insert(eUnknownX, this->getDisplayX(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownY, this->getDisplayY(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownZ, this->getDisplayZ(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownPrimaryI, this->getDisplayPrimaryI(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryJ, this->getDisplayPrimaryJ(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryK, this->getDisplayPrimaryK(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownRadiusA, this->getDisplayRadiusA(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));

    return parameters;

}

/*!
 * \brief Circle::setUnknownParameters
 * \param parameters
 */
void Circle::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    OiVec position = this->center.getVector();
    OiVec direction = this->normal.getVector();
    double radius = this->radius.getRadius();

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
        case eUnknownRadiusA:
            radius = parameters.value(eUnknownRadiusA);
            break;
        }
    }

    //update circle definition
    direction.normalize();
    Position circlePosition(position);
    Direction circleDirection(direction);
    Radius circleRadius(radius);
    this->setCircle(circlePosition, circleDirection, circleRadius);

}

/*!
 * \brief Circle::recalc
 */
void Circle::recalc(){

    Geometry::recalc();

    //reset circle definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->center.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->radius.setRadius(0.0);
    }

}

/*!
 * \brief Circle::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Circle::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement circle = Geometry::toOpenIndyXML(xmlDoc);

    if(circle.isNull()){
        return circle;
    }

    circle.setAttribute("type", getGeometryTypeName(eCircleGeometry));

    return circle;

}

/*!
 * \brief Circle::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Circle::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement normalVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || normalVector.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !normalVector.hasAttribute("i") || !normalVector.hasAttribute("j") || !normalVector.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius.setRadius(radius.attribute("value").toDouble());
        this->normal.setVector(normalVector.attribute("i").toDouble(),
                               normalVector.attribute("j").toDouble(),
                               normalVector.attribute("k").toDouble());
        this->center.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Circle::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->center.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Circle::getDisplayRadiusA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Circle::getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->radius.getRadius(), type), 'f', digits);
    }
    return QString("");
}
