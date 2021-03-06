#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

namespace oi{

/*!
 * \brief The Sphere class
 */
class OI_CORE_EXPORT Sphere : public Geometry
{
    Q_OBJECT

public:
    Sphere(const bool &isNominal, QObject *parent = 0);
    Sphere(const bool &isNominal, const Position &center, const Radius &radius, QObject *parent = 0);

    Sphere(const Sphere &copy, QObject *parent = 0);

    Sphere &operator=(const Sphere &copy);

    ~Sphere();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum SphereUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownRadius
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasPosition() const;
    bool hasRadius() const;

    //############################
    //get or set sphere parameters
    //############################

    void setSphere(const Position &center, const Radius &radius);

    //#############################
    //get or set unknown parameters
    //#############################

    virtual QMap<GeometryParameters, QString> getUnknownParameters(const QMap<DimensionType, UnitType> &displayUnits,
                                                          const QMap<DimensionType, int> &displayDigits) const;
    virtual void setUnknownParameters(const QMap<GeometryParameters, double> &parameters);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

};

}

#endif // SPHERE_H
