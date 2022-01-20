#include "torus.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Torus::Torus
 * \param isNominal
 * \param parent
 */
Torus::Torus(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

}

/*!
 * \brief Torus::Torus
 * \param isNominal
 * \param center
 * \param normal
 * \param radiusA
 * \param radiusB
 * \param parent
 */
Torus::Torus(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radiusA, const Radius &radiusB, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

    this->setTorus(center, normal, radiusA, radiusB);

}

/*!
 * \brief Torus::Torus
 * \param copy
 * \param parent
 */
Torus::Torus(const Torus &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->radius = copy.radius;
    this->radiusB = copy.radiusB;

}

/*!
 * \brief Torus::operator =
 * \param copy
 * \return
 */
Torus &Torus::operator=(const Torus &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->radius = copy.radius;
    this->radiusB = copy.radiusB;

    return *this;

}

/*!
 * \brief Torus::~Torus
 */
Torus::~Torus(){

}

/*!
 * \brief Torus::hasDirection
 * \return
 */
bool Torus::hasDirection() const{
    return true;
}

/*!
 * \brief Torus::hasPosition
 * \return
 */
bool Torus::hasPosition() const{
    return true;
}

/*!
 * \brief Torus::hasRadius
 * \return
 */
bool Torus::hasRadius() const{
    return true;
}

/*!
 * \brief Torus::getSmallRadius
 * Returns the distance of the center curve to the torus surface (radiusA > radiusB)
 * \return
 */
const Radius &Torus::getSmallRadius() const{
    return this->radiusB;
}

/*!
 * \brief Torus::setTorus
 * \param center
 * \param normal
 * \param radiusA
 * \param radiusB
 */
void Torus::setTorus(const Position &center, const Direction &normal, const Radius &radiusA, const Radius &radiusB){

    //set the given parameters
    this->xyz = center;
    this->ijk = normal;
    this->radius = radiusA;
    this->radiusB = radiusB;

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Torus::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Torus::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<GeometryParameters, QString> parameters;

    parameters.insert(eUnknownX, this->getDisplayX(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownY, this->getDisplayY(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownZ, this->getDisplayZ(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownPrimaryI, this->getDisplayPrimaryI(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryJ, this->getDisplayPrimaryJ(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryK, this->getDisplayPrimaryK(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownRadiusA, this->getDisplayRadiusA(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));
    parameters.insert(eUnknownRadiusA, this->getDisplayRadiusA(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));

    return parameters;

}

/*!
 * \brief Torus::setUnknownParameters
 * \param parameters
 */
void Torus::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    OiVec position = this->xyz.getVector();
    OiVec direction = this->ijk.getVector();
    double radiusA = this->radius.getRadius();
    double radiusB = this->radiusB.getRadius();

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
            radiusA = parameters.value(eUnknownRadiusA);
            break;
        case eUnknownRadiusB:
            radiusB = parameters.value(eUnknownRadiusA);
            break;
        }
    }

    //update torus definition
    direction.normalize();
    Position torusPosition(position);
    Direction torusDirection(direction);
    Radius torusRadiusA(radiusA);
    Radius torusRadiusB(radiusB);
    this->setTorus(torusPosition, torusDirection, torusRadiusA, torusRadiusB);

}

/*!
 * \brief Torus::recalc
 */
void Torus::recalc(){

    Geometry::recalc();

    //reset torus definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->ijk.setVector(0.0, 0.0, 0.0);
        this->radius.setRadius(0.0);
        this->radiusB.setRadius(0.0);
    }

}

/*!
 * \brief Torus::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Torus::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement torus = Geometry::toOpenIndyXML(xmlDoc);

    if(torus.isNull()){
        return torus;
    }

    torus.setAttribute("type", getGeometryTypeName(eCircleGeometry));

    //set radius b
    QDomElement radiusB = xmlDoc.createElement("radiusB");
    if(this->isSolved || this->isNominal){
        radiusB.setAttribute("value", this->radiusB.getRadius());
    }else{
        radiusB.setAttribute("value", 0.0);
    }
    torus.appendChild(radiusB);

    return torus;

}

/*!
 * \brief Torus::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Torus::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set torus attributes
        QDomElement radiusA = xmlElem.firstChildElement("radius");
        QDomElement radiusB = xmlElem.firstChildElement("radiusB");
        QDomElement normal = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radiusA.isNull() || radiusB.isNull() || normal.isNull() || center.isNull()
                || !radiusA.hasAttribute("value") || !radiusB.hasAttribute("value")
                || !normal.hasAttribute("i") || normal.hasAttribute("j") || normal.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius.setRadius(radiusA.attribute("value").toDouble());
        this->radiusB.setRadius(radiusB.attribute("value").toDouble());
        this->ijk.setVector(normal.attribute("i").toDouble(),
                               normal.attribute("j").toDouble(),
                               normal.attribute("k").toDouble());
        this->xyz.setVector(center.attribute("x").toDouble(),
                               center.attribute("y").toDouble(),
                               center.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Torus::getDisplayRadiusB
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayRadiusB(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->radiusB.getRadius(), type), 'f', digits);
    }
    return QString("");
}
