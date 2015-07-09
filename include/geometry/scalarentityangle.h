#ifndef SCALARENTITYANGLE_H
#define SCALARENTITYANGLE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"

namespace oi{

/*!
 * \brief The ScalarEntityAngle class
 */
class OI_CORE_EXPORT ScalarEntityAngle : public Geometry
{
    Q_OBJECT

public:
    ScalarEntityAngle(const bool &isNominal, QObject *parent = 0);
    ScalarEntityAngle(const bool &isNominal, const double &angle, QObject *parent = 0);

    ScalarEntityAngle(const ScalarEntityAngle &copy, QObject *parent = 0);

    ScalarEntityAngle &operator=(const ScalarEntityAngle &copy);

    ~ScalarEntityAngle();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum AngleUnknowns{
        unknownAngle = 0
    };

    //###########################
    //get or set angle parameters
    //###########################

    const double &getAngle() const;

    void setAngle(const double &angle);

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

    QString getDisplayAngle(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //################
    //angle attributes
    //################

    double angle;

};

}

#endif // SCALARENTITYANGLE_H
