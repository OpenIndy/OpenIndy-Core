#ifndef SCALARENTITYDISTANCE_H
#define SCALARENTITYDISTANCE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"

namespace oi{

/*!
 * \brief The ScalarEntityDistance class
 */
class OI_CORE_EXPORT ScalarEntityDistance : public Geometry
{
    Q_OBJECT

public:
    ScalarEntityDistance(const bool &isNominal, QObject *parent = 0);
    ScalarEntityDistance(const bool &isNominal, const double &distance, QObject *parent = 0);

    ScalarEntityDistance(const ScalarEntityDistance &copy, QObject *parent = 0);

    ScalarEntityDistance &operator=(const ScalarEntityDistance &copy);

    ~ScalarEntityDistance();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum DistanceUnknowns{
        unknownDistance = 0
    };

    //##############################
    //get or set distance parameters
    //##############################

    const double &getDistance() const;

    void setDistance(const double &distance);

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

    QString getDisplayDistance(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //###################
    //distance attributes
    //###################

    double distance;

};

}

#endif // SCALARENTITYDISTANCE_H
