#ifndef SCALARENTITYMEASUREMENTSERIES_H
#define SCALARENTITYMEASUREMENTSERIES_H

#include <QObject>
#include <QtXml>

#include "geometry.h"

namespace oi{

/*!
 * \brief The ScalarEntityMeasurementSeries class
 */
class OI_CORE_EXPORT ScalarEntityMeasurementSeries : public Geometry
{
    Q_OBJECT

public:
    ScalarEntityMeasurementSeries(const bool &isNominal, QObject *parent = 0);
    ScalarEntityMeasurementSeries(const bool &isNominal, const double &seriesValue, QObject *parent = 0);

    ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy, QObject *parent = 0);

    ScalarEntityMeasurementSeries &operator=(const ScalarEntityMeasurementSeries &copy);

    ~ScalarEntityMeasurementSeries();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum MeasurementSeriesUnknowns{
        unknownMeasurementSeries = 0
    };

    //########################################
    //get or set measurement series parameters
    //########################################

    const double &getSeriesValue() const;

    void setSeriesValue(const double &seriesValue);

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

    QString getDisplayMeasurementSeries(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //#############################
    //measurement series attributes
    //#############################

    double seriesValue;

};

}

#endif // SCALARENTITYMEASUREMENTSERIES_H
