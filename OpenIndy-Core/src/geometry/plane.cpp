#include "plane.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Plane::Plane
 * \param isNominal
 * \param parent
 */
Plane::Plane(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

}

/*!
 * \brief Plane::Plane
 * \param isNominal
 * \param xyz
 * \param normal
 * \param parent
 */
Plane::Plane(const bool &isNominal, const Position &xyz, const Direction &normal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->setPlane(xyz, normal);

}

/*!
 * \brief Plane::Plane
 * \param isNominal
 * \param normal
 * \param d
 * \param parent
 */
Plane::Plane(const bool &isNominal, const Direction &normal, const double &d, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->setPlane(normal, d);

}

/*!
 * \brief Plane::Plane
 * \param copy
 * \param parent
 */
Plane::Plane(const Plane &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->xyz = copy.xyz;
    this->normal = copy.normal;
    this->d = copy.d;

}

/*!
 * \brief Plane::operator =
 * \param copy
 * \return
 */
Plane &Plane::operator=(const Plane &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPlane(this);
    }

    this->xyz = copy.xyz;
    this->normal = copy.normal;
    this->d = copy.d;

    return *this;

}

/*!
 * \brief Plane::~Plane
 */
Plane::~Plane(){

}

/*!
 * \brief Plane::hasDirection
 * \return
 */
bool Plane::hasDirection() const{
    return true;
}

/*!
 * \brief Plane::hasPosition
 * \return
 */
bool Plane::hasPosition() const{
    return true;
}

/*!
 * \brief Plane::getDirection
 * Returns the normal vector of the plane
 * \return
 */
const Direction &Plane::getDirection() const{
    return this->normal;
}

/*!
 * \brief Plane::getPosition
 * Returns a point on the plane
 * \return
 */
const Position &Plane::getPosition() const{
    return this->xyz;
}

/*!
 * \brief Plane::getDistanceFromOrigin
 * Returns the smallest vertical distance of the plane from the origin
 * \return
 */
const double &Plane::getDistanceFromOrigin() const{
    return this->d;
}

/*!
 * \brief Plane::setPlane
 * \param xyz
 * \param normal
 */
void Plane::setPlane(const Position &xyz, const Direction &normal){

    //set the given parameters
    this->xyz.setVector(xyz.getVector());
    this->normal = normal;

    //calculate the other parameters
    OiVec n0 = normal.getVector();
    n0.normalize();
    OiVec::dot(this->d, this->xyz.getVector(), n0);

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Plane::setPlane
 * \param normal
 * \param d
 */
void Plane::setPlane(const Direction &normal, const double &d){

    //set the given parameters
    this->normal = normal;
    this->d = d;

    //calculate the other parameters
    this->xyz.setVector(this->d * this->normal.getVector());

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Plane::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Plane::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

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
 * \brief Plane::setUnknownParameters
 * \param parameters
 */
void Plane::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    OiVec position = this->xyz.getVector();
    OiVec direction = this->normal.getVector();

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

    //update plane definition
    direction.normalize();
    Position planePosition(position);
    Direction planeDirection(direction);
    this->setPlane(planePosition, planeDirection);

}

/*!
 * \brief Plane::recalc
 */
void Plane::recalc(){

    Geometry::recalc();

    //reset plane definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->d = 0.0;
    }

}

/*!
 * \brief Plane::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Plane::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement plane = Geometry::toOpenIndyXML(xmlDoc);

    if(plane.isNull()){
        return plane;
    }

    plane.setAttribute("type", getGeometryTypeName(ePlaneGeometry));

    return plane;

}

/*!
 * \brief Plane::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Plane::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set plane attributes
        QDomElement normalVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement pointOnPlane = xmlElem.firstChildElement("coordinates");

        if(normalVector.isNull() || pointOnPlane.isNull()
                || !normalVector.hasAttribute("i") || !normalVector.hasAttribute("j") || !normalVector.hasAttribute("k")
                || !pointOnPlane.hasAttribute("x") || !pointOnPlane.hasAttribute("y") || !pointOnPlane.hasAttribute("z")){
            return false;
        }

        this->normal.setVector(normalVector.attribute("i").toDouble(),
                               normalVector.attribute("j").toDouble(),
                               normalVector.attribute("k").toDouble());
        this->xyz.setVector(pointOnPlane.attribute("x").toDouble(),
                            pointOnPlane.attribute("y").toDouble(),
                            pointOnPlane.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Plane::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Plane::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
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
QString Plane::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
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
QString Plane::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
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
QString Plane::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Plane::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Plane::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Plane::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Plane::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}
