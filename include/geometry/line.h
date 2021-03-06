#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

namespace oi{

/*!
 * \brief The Line class
 */
class OI_CORE_EXPORT Line : public Geometry
{
    Q_OBJECT
public:
    Line(const bool &isNominal, QObject *parent = 0);
    Line(const bool &isNominal, const Position &xyz, const Direction &axis, QObject *parent = 0);

    Line(const Line &copy, QObject *parent = 0);

    Line &operator=(const Line &copy);

    ~Line();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum LineUnknowns{
        unknownPositionX,
        unknownPositionY,
        unknownPositionZ,
        unknownAxisI,
        unknownAxisJ,
        unknownAxisK
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //##########################
    //get or set line parameters
    //##########################

    void setLine(const Position &xyz, const Direction &axis);

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

#endif // LINE_H
