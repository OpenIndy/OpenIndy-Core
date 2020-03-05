#include "paraboloid.h"

#include "featurewrapper.h"

using namespace oi;
using namespace oi::math;

/*!
 * \brief Paraboloid::Paraboloid
 * \param isNominal
 * \param parent
 */
Paraboloid::Paraboloid(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

}

/*!
 * \brief Paraboloid::Paraboloid
 * \param isNominal
 * \param apex
 * \param axis
 * \param a
 * \param parent
 */
Paraboloid::Paraboloid(const bool &isNominal, const Position &apex, const Direction &axis, const double &a, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->setParaboloid(apex, axis, a);

}

/*!
 * \brief Paraboloid::Paraboloid
 * \param copy
 * \param parent
 */
Paraboloid::Paraboloid(const Paraboloid &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;

}

/*!
 * \brief Paraboloid::operator =
 * \param copy
 * \return
 */
Paraboloid &Paraboloid::operator=(const Paraboloid &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->xyz = copy.xyz;
    this->ijk = copy.ijk;

    return *this;

}

/*!
 * \brief Paraboloid::~Paraboloid
 */
Paraboloid::~Paraboloid(){

}

/*!
 * \brief Paraboloid::hasDirection
 * \return
 */
bool Paraboloid::hasDirection() const{
    return true;
}

/*!
 * \brief Paraboloid::hasPosition
 * \return
 */
bool Paraboloid::hasPosition() const{
    return true;
}

/*!
 * \brief Paraboloid::getA
 * Returns the compression or streatch parameter
 * \return
 */
const double &Paraboloid::getA() const{
    return this->a;
}

/*!
 * \brief Paraboloid::setParaboloid
 * \param apex
 * \param axis
 * \param a
 */
void Paraboloid::setParaboloid(const Position &apex, const Direction &axis, const double &a){

    //set the given parameters
    this->xyz = apex;
    this->ijk = axis;
    this->a = a;

    emit this->geomParametersChanged(this->id);

}

/*!
 * \brief Paraboloid::getUnknownParameters
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<GeometryParameters, QString> Paraboloid::getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits) const{

    QMap<GeometryParameters, QString> parameters;

    parameters.insert(eUnknownX, this->getDisplayX(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric), 0));
    parameters.insert(eUnknownY, this->getDisplayY(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric), 0));
    parameters.insert(eUnknownZ, this->getDisplayZ(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric), 0));
    parameters.insert(eUnknownPrimaryI, this->getDisplayPrimaryI(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryJ, this->getDisplayPrimaryJ(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownPrimaryK, this->getDisplayPrimaryK(displayDigits.value(eDimensionless, 0)));
    parameters.insert(eUnknownA, this->getDisplayRadiusA(displayUnits.value(eMetric, eUnitMeter), displayDigits.value(eMetric, 0)));

    return parameters;

}

/*!
 * \brief Paraboloid::setUnknownParameters
 * \param parameters
 */
void Paraboloid::setUnknownParameters(const QMap<GeometryParameters, double> &parameters){

    //get current parameters
    OiVec position = this->xyz.getVector();
    OiVec direction = this->ijk.getVector();
    double a = this->a;

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
        case eUnknownA:
            a = parameters.value(eUnknownA);
            break;
        }
    }

    //update paraboloid definition
    direction.normalize();
    Position paraboloidPosition(position);
    Direction paraboloidDirection(direction);
    this->setParaboloid(paraboloidPosition, paraboloidDirection, a);

}

/*!
 * \brief Paraboloid::recalc
 */
void Paraboloid::recalc(){

    Geometry::recalc();

    //reset paraboloid definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->xyz.setVector(0.0, 0.0, 0.0);
        this->ijk.setVector(0.0, 0.0, 0.0);
        this->a = 0.0;
    }

}

/*!
 * \brief Paraboloid::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Paraboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement paraboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(paraboloid.isNull()){
        return paraboloid;
    }

    paraboloid.setAttribute("type", getGeometryTypeName(eParaboloidGeometry));

    //set a
    QDomElement a = xmlDoc.createElement("a");
    if(this->isSolved || this->isNominal){
        a.setAttribute("value", this->a);
    }else{
        a.setAttribute("value", 0.0);
    }
    paraboloid.appendChild(a);

    return paraboloid;

}

/*!
 * \brief Paraboloid::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Paraboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set paraboloid attributes
        QDomElement a = xmlElem.firstChildElement("a");
        QDomElement axis = xmlElem.firstChildElement("spatialDirection");
        QDomElement apex = xmlElem.firstChildElement("coordinates");

        if(a.isNull() || axis.isNull() || apex.isNull() || !a.hasAttribute("value")
                || !axis.hasAttribute("i") || axis.hasAttribute("j") || axis.hasAttribute("k")
                || !apex.hasAttribute("x") || !apex.hasAttribute("y") || !apex.hasAttribute("z")){
            return false;
        }

        this->a = a.attribute("value").toDouble();
        this->ijk.setVector(axis.attribute("i").toDouble(),
                               axis.attribute("j").toDouble(),
                               axis.attribute("k").toDouble());
        this->xyz.setVector(apex.attribute("x").toDouble(),
                             apex.attribute("y").toDouble(),
                             apex.attribute("z").toDouble());

    }

    return result;

}

/*!
 * \brief Paraboloid::getDisplayA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->a, type), 'f', digits);
    }
    return QString("");
}
