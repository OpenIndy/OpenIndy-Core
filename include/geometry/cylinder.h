#ifndef CYLINDER_H
#define CYLINDER_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

namespace oi{

/*!
 * \brief The Cylinder class
 * Defines a right circular cylinder
 */
class OI_CORE_EXPORT Cylinder : public Geometry
{
    Q_OBJECT

public:
    Cylinder(const bool &isNominal, QObject *parent = 0);
    Cylinder(const bool &isNominal, const Position &axisPoint, const Direction &axis, const Radius &radius, QObject *parent = 0);

    Cylinder(const Cylinder &copy, QObject *parent = 0);

    Cylinder &operator=(const Cylinder &copy);

    ~Cylinder();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum CylinderUnknowns{
        unknownAxisPointX = 0,
        unknownAxisPointY,
        unknownAxisPointZ,
        unknownAxisI,
        unknownAxisJ,
        unknownAxisK,
        unknownRadius
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;
    bool hasRadius() const;

    //##############################
    //get or set cylinder parameters
    //##############################

    void setCylinder(const Position &axisPoint, const Direction &axis, const Radius &radius);

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

};

}

#endif // CYLINDER_H
