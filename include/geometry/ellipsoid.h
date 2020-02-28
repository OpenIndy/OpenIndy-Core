#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

namespace oi{

/*!
 * \brief The Ellipsoid class
 * Defines an ellipsoid of rotation
 */
class OI_CORE_EXPORT Ellipsoid : public Geometry
{
    Q_OBJECT

public:
    Ellipsoid(const bool &isNominal, QObject *parent = 0);
    Ellipsoid(const bool &isNominal, const Position &center, const Direction &majorAxis,
              const double &a, const double &b, QObject *parent = 0);

    Ellipsoid(const Ellipsoid &copy, QObject *parent = 0);

    Ellipsoid &operator=(const Ellipsoid &copy);

    ~Ellipsoid();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum EllipsoidUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownMajorAxisI,
        unknownMajorAxisJ,
        unknownMajorAxisK,
        unknownA,
        unknownB
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //###############################
    //get or set ellipsoid parameters
    //###############################

    const double &getA() const;
    const double &getB() const;

    void setEllipsoid(const Position &center, const Direction &majorAxis,
                      const double &a, const double &b);

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

    QString getDisplayA(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayB(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //####################
    //ellipsoid attributes
    //####################

    double a; //semi-major axis
    double b; //semi-minor axis

};

}

#endif // ELLIPSOID_H
