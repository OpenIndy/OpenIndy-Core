#ifndef EXCHANGEINTERFACE_H
#define EXCHANGEINTERFACE_H

#include <QObject>
#include <QPointer>
#include <QIODevice>
#include <QStringList>

#include "pluginmetadata.h"
#include "coordinatesystem.h"
#include "featurewrapper.h"

namespace oi{

/*!
 * \brief The OiExchangeInterface class
 * Base class for implementing import/export plugins
 */
class OI_CORE_EXPORT ExchangeInterface : public QObject
{
    Q_OBJECT

public:
    explicit ExchangeInterface(QObject *parent = 0);

    virtual ~ExchangeInterface();

    //##############################
    //exchange initialization method
    //##############################

    virtual void init();

    //#########################################
    //methods to get or set further information
    //#########################################

    const PluginMetaData &getMetaData() const;
    const QList<GeometryTypes> &getSupportedGeometries();

    const QPointer<QIODevice> &getDevice() const;
    void setDevice(const QPointer<QIODevice> &device);

    const QList<QPointer<FeatureWrapper> > &getFeatures() const;
    void setFeatures(const QList<QPointer<FeatureWrapper> > &features);

    const bool &getExportObservations() const;
    void setExportObservations(const bool &exportObservations);

    const QString &getGroupName() const;
    void setGroupName(const QString &groupName);

    const QPointer<CoordinateSystem> &getNominalSystem() const;
    void setNominalSystem(const QPointer<CoordinateSystem> &nominalSystem);

    const QMap<DimensionType, UnitType> &getUnits() const;
    void setUnit(const DimensionType &dimension, const UnitType &unit);

public slots:

    //#########################
    //import and export methods
    //#########################

    virtual void importOiData();
    virtual void exportOiData();

signals:

    //################################################
    //signals to inform OpenIndy about exchange issues
    //################################################

    void sendMessage(const QString &msg);
    void updateProgress(const int &progress, const QString &msg); // 0 <= progress <= 100
    void importFinished(const bool &success);
    void exportFinished(const bool &success);

protected:

    //###########################
    //input and output parameters
    //###########################

    QMap<DimensionType, UnitType> units;

    bool exportObservations; //true if only the observations of the selected geometries shall be exported
    QString groupName; //group that the imported nominals shall be added to
    QPointer<CoordinateSystem> nominalSystem; //nominal coordinate system the imported geometries are referenced to

    QPointer<QIODevice> device; //the device used for the im- or export (e.g. a file)
    QList<QPointer<FeatureWrapper> > features; //the features that shall be exported or were imported

    PluginMetaData metaData;
    QList<GeometryTypes> supportedGeometries;

};

}

#ifdef PLUGIN_INTERFACE_VERSION
#define Exchange_iidd "de.openIndy.plugin.exchange.v" PLUGIN_INTERFACE_VERSION
#endif

#endif // EXCHANGEINTERFACE_H

