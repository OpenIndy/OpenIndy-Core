#include "sphere.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Sphere::Sphere
 * \param isNominal
 * \param parent
 */
Sphere::Sphere(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

}

/*!
 * \brief Sphere::Sphere
 * \param isNominal
 * \param center
 * \param radius
 * \param parent
 */
Sphere::Sphere(const bool &isNominal, const Position &center, const Radius &radius, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

    this->setSphere(center, radius);

}

/*!
 * \brief Sphere::Sphere
 * \param copy
 * \param parent
 */
Sphere::Sphere(const Sphere &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

    this->xyz = copy.xyz;
    this->radius = copy.radius;

}

/*!
 * \brief Sphere::operator =
 * \param copy
 * \return
 */
Sphere &Sphere::operator=(const Sphere &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setSphere(this);
    }

    this->xyz = copy.xyz;
    this->radius = copy.radius;

    return *this;

}

/*!
 * \brief Sphere::~Sphere
 */
Sphere::~Sphere(){

}

/*!
 * \brief Sphere::hasPosition
 * \return
 */
bool Sphere::hasPosition() const{
    return true;
}

/*!
 * \brief Sphere::hasRadius
 * \return
 */
bool Sphere::hasRadius() const{
    return true;
}


/*!
 * \brief Sphere::setSphere
 * \param center
 * \param radius
 */
void Sphere::setSphere(const Position &center, const Radius &radius){

    //set the given parameters
    this->xyz = center;
    this->radius = radius;

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Sphere::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Sphere::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<GeometryParameters, QString> parameters;

    parameters.insert(eUnknownX, this->getDisplayX(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownY, this->getDisplayY(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownZ, this->getDisplayZ(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownRadiusA, this->getDisplayRadiusA(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));

    return parameters;

}

/*!
 * \brief Sphere::setUnknownParameters
 * \param parameters
 */
void Sphere::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    OiVec position = this->xyz.getVector();
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
        case eUnknownRadiusA:
            radius = parameters.value(eUnknownRadiusA);
            break;
        }
    }

    //update sphere definition
    Position spherePosition(position);
    Radius sphereRadius(radius);
    this->setSphere(spherePosition, sphereRadius);

}

/*!
 * \brief Sphere::recalc
 */
void Sphere::recalc(){

    Geometry::recalc();

    //reset sphere definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->radius.setRadius(0.0);
    }

}

/*!
 * \brief Sphere::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Sphere::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement sphere = Geometry::toOpenIndyXML(xmlDoc);

    if(sphere.isNull()){
        return sphere;
    }

    sphere.setAttribute("type", getGeometryTypeName(eSphereGeometry));

    return sphere;

}

/*!
 * \brief Sphere::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Sphere::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set sphere attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius.setRadius(radius.attribute("value").toDouble());
        this->xyz.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}
