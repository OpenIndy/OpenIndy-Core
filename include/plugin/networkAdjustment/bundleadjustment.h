#ifndef BUNDLEADJUSTMENT_H
#define BUNDLEADJUSTMENT_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QMap>
#include <QMultiMap>

#include "pluginmetadata.h"
#include "oijob.h"
#include "types.h"

namespace oi{

/*!
 * \brief The BundleTransformation class
 * Defines a transformation for bundle adjustment
 */
class BundleTransformation{
public:
    BundleTransformation() : id(-1){}

    bool operator == (const BundleTransformation &other){
        if(this->id == other.id){
            return true;
        }
        return false;
    }

    //station feature id
    int id;

    //transformation parameters
    QMap<TrafoParamParameters, double> parameters;

};

/*!
 * \brief The BundleGeometry class
 * Defines a geometry for bundle adjustment
 */
class BundleGeometry{
public:
    BundleGeometry() : id(-1){}

    bool operator == (const BundleGeometry &other){
        if(this->id == other.id){
            return true;
        }
        return false;
    }

    //geometry feature id
    int id;

    //geometry parameters
    QMap<GeometryParameters, double> parameters;

};

/*!
 * \brief The BundleStation class
 * Defines a station used for bundle adjustment
 */
class BundleStation{
public:
    BundleStation() : id(-1), tx(true), ty(true), tz(true),
    rx(true), ry(true), rz(true), m(true){}

    bool operator == (const BundleStation &other){
        if(this->id == other.id){
            return true;
        }
        return false;
    }

    //station feature id
    int id;

    //defines which parameters to calculate
    bool tx, ty, tz;
    bool rx, ry, rz;
    bool m;

    //geometries solved in station system
    QList<Geometry> geometries;

};

/*!
 * \brief The BundleAdjustment class
 * Interface for implementing bundle adjustment plugins.
 */
class OI_CORE_EXPORT BundleAdjustment : public QObject
{
    Q_OBJECT

public:
    BundleAdjustment(QObject *parent = 0);
    virtual ~BundleAdjustment();

    //############################
    //bundle initialization method
    //############################

    virtual void init();

    //################################
    //calculates the bundle adjustment
    //################################

    virtual bool runBundle();

    //###########################################################
    //methods to get or set further information to solve a bundle
    //###########################################################

    //meta data
    const PluginMetaData &getMetaData() const;

    //available scalar parameters
    const QMap<QString, int> &getIntegerParameter() const;
    const QMap<QString, double> &getDoubleParameter() const;
    const QMultiMap<QString, QString> &getStringParameter() const;

    //scalar input parameters
    const ScalarInputParams &getScalarInputParams();
    void setScalarInputParams(const ScalarInputParams &params);

    //input stations
    const QList<BundleStation> &getInputStations() const;
    void setInputStations(const QList<BundleStation> &stations);
    const BundleStation &getBaseStation() const;
    void setBaseStation(const BundleStation &station);

    //bundle coordinate system
    const QPointer<CoordinateSystem> &getBundleSystem() const;
    void setBundleSystem(const QPointer<CoordinateSystem> &system);

    //OpenIndy job
    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //###############
    //general getters
    //###############

    //id
    const int &getId() const;

    //bundle results
    const QList<BundleGeometry> &getOutputGeometries() const;
    const QList<BundleTransformation> &getOutputTransformations() const;

    //#############
    //clear results
    //#############

    virtual void clear();
    virtual void clearResults();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

signals:

    //################################################
    //signals to inform OpenIndy about function issues
    //################################################

    //messaging
    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

    //input parameters changed
    void scalarInputParametersChanged();
    void inputStationsChanged();
    void baseStationChanged();
    void bundleSystemChanged();
    void jobChanged();

private:

    //###########
    //connect job
    //###########

    void connectJob();
    void disconnectJob();

protected:

    //###########################
    //input and output parameters
    //###########################

    //meta information about the bundle
    PluginMetaData metaData;

    //the needed scalar input parameters including default values
    QMap<QString, int> integerParameters;
    QMap<QString, double> doubleParameters;
    QMultiMap<QString, QString> stringParameters;

    //user defined scalar input parameters
    ScalarInputParams scalarInputParams;

    //input stations
    QList<BundleStation> stations;
    BundleStation baseSystem;

    //output geometries and transformation parameters
    QList<BundleGeometry> geometries;
    QList<BundleTransformation> transformations;

    //bundle coordinate system
    QPointer<CoordinateSystem> bundleSystem;

    //OpenIndy job
    QPointer<OiJob> currentJob;

    //##################
    //general attributes
    //##################

    int id;

};

}

Q_DECLARE_METATYPE( oi::BundleAdjustment* )

#ifndef STR
#define STR(x) #x
#endif
#ifndef STR2
#define STR2(x) STR(x)
#endif

#ifndef CONCAT
#define CONCAT(a, b) a ## b
#endif
#ifndef CONCAT2
#define CONCAT2(a, b) CONCAT(a, b)
#endif

#define BUNDLEADJUSTMENT_PREFIX de.openIndy.plugin.bundleAdjustment.v
#define BundleAdjustment_iidd STR2(CONCAT2(BUNDLEADJUSTMENT_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // BUNDLEADJUSTMENT_H
