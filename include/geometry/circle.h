#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

namespace oi{

/*!
 * \brief The Circle class
 */
class OI_CORE_EXPORT Circle : public Geometry
{
    Q_OBJECT

public:
    Circle(const bool &isNominal, QObject *parent = 0);
    Circle(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radius, QObject *parent = 0);

    Circle(const Circle &copy, QObject *parent = 0);

    Circle &operator=(const Circle &copy);

    ~Circle();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum CircleUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownNormalI,
        unknownNormalJ,
        unknownNormalK,
        unknownRadius
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;
    bool hasRadius() const;

    void setCircle(const Position &center, const Direction &normal, const Radius &radius);

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

#endif // CIRCLE_H
