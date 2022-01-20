#ifndef CONE_H
#define CONE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

namespace oi{

/*!
 * \brief The Cone class
 * Defines a right circular cone
 */
class OI_CORE_EXPORT Cone : public Geometry
{
    Q_OBJECT

public:
    Cone(const bool &isNominal, QObject *parent = 0);
    Cone(const bool &isNominal, const Position &apex, const Direction &axis, const double &aperture, QObject *parent = 0);

    Cone(const Cone &copy, QObject *parent = 0);

    Cone &operator=(const Cone &copy);

    ~Cone();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum ConeUnknowns{
        unknownApexX = 0,
        unknownApexY,
        unknownApexZ,
        unknownAxisI,
        unknownAxisJ,
        unknownAxisK,
        unknownAperture
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //##########################
    //get or set cone parameters
    //##########################

    const double &getAperture() const;

    void setCone(const Position &apex, const Direction &axis, const double &aperture);

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


private:

    //###############
    //cone attributes
    //###############

    double aperture; //opening angle = 2 * angle between surface line and r

};

}

#endif // CONE_H
