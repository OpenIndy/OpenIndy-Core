#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "types.h"

namespace oi{

//#######################################
//global functions to query element types
//#######################################

QList<ElementTypes> OI_CORE_EXPORT getAvailableElementTypes();
QList<FeatureTypes> OI_CORE_EXPORT getAvailableFeatureTypes();
QList<GeometryTypes> OI_CORE_EXPORT getAvailableGeometryTypes();

const QString OI_CORE_EXPORT &getElementTypeName(const ElementTypes &type);
const QString OI_CORE_EXPORT &getElementTypePluralName(const ElementTypes &type);
ElementTypes OI_CORE_EXPORT getElementTypeEnum(const QString &name);
ElementTypes OI_CORE_EXPORT getElementTypeEnum(const FeatureTypes &type);
ElementTypes OI_CORE_EXPORT getElementTypeEnum(const ReadingTypes &type);

const QString OI_CORE_EXPORT &getFeatureTypeName(const FeatureTypes &type);
FeatureTypes OI_CORE_EXPORT getFeatureTypeEnum(const QString &name);

const QString OI_CORE_EXPORT &getGeometryTypeName(const GeometryTypes &type);
GeometryTypes OI_CORE_EXPORT getGeometryTypeEnum(const QString &name);
GeometryTypes OI_CORE_EXPORT getGeometryTypeEnum(const FeatureTypes &type);

bool OI_CORE_EXPORT getIsFeature(const ElementTypes &type);
bool OI_CORE_EXPORT getIsReading(const ElementTypes &type);
bool OI_CORE_EXPORT getIsObservation(const ElementTypes &type);

bool OI_CORE_EXPORT getIsGeometry(const FeatureTypes &type);
bool OI_CORE_EXPORT getIsScalarEntity(const FeatureTypes &type);

//#########################################################
//global function to query icon resources for element types
//#########################################################

const QString OI_CORE_EXPORT &getElementTypeIconPath(const ElementTypes &type);
const QString OI_CORE_EXPORT &getFeatureTypeIconPath(const FeatureTypes &type);

//#######################################
//global functions to query reading types
//#######################################

const QString OI_CORE_EXPORT &getReadingTypeName(const ReadingTypes &type);
ReadingTypes OI_CORE_EXPORT getReadingTypeEnum(const QString &name);

//############################################
//global functions to query display attributes
//############################################

const QList<int> OI_CORE_EXPORT &getFeatureDisplayAttributes();
const QList<ObservationDisplayattributes> OI_CORE_EXPORT &getObservationDisplayAttributes();
const QList<ReadingDisplayAttributes> OI_CORE_EXPORT &getReadingDisplayAttributes();

bool OI_CORE_EXPORT getIsFeatureDisplayAttribute(const int &attr);
bool OI_CORE_EXPORT getIsTrafoParamDisplayAttribute(const int &attr);

const QString OI_CORE_EXPORT &getFeatureDisplayAttributeName(const int &attr);
const QString OI_CORE_EXPORT &getFeatureDisplayAttributeName(const FeatureDisplayAttributes &attr);
const QString OI_CORE_EXPORT &getFeatureDisplayAttributeName(const TrafoParamDisplayAttributes &attr);

bool OI_CORE_EXPORT getReadingDisplayAttributeVisibility(const ReadingDisplayAttributes &attr, const ReadingTypes &type);

const QString OI_CORE_EXPORT &getObservationDisplayAttributesName(const ObservationDisplayattributes &attr);
const QString OI_CORE_EXPORT &getReadingDisplayAttributeName(const ReadingDisplayAttributes &attr);

//###############################
//global functions to query units
//###############################

double OI_CORE_EXPORT convertToDefault(const double &value, const UnitType &type);
double OI_CORE_EXPORT convertFromDefault(const double &value, const UnitType &type);

const QString OI_CORE_EXPORT &getUnitTypeName(const UnitType &type);
UnitType OI_CORE_EXPORT getUnitTypeEnum(const QString &name);

//######################################
//global functions to query sensor types
//######################################

QList<SensorTypes> OI_CORE_EXPORT getAvailableSensorTypes();
const QString OI_CORE_EXPORT &getSensorTypeName(const SensorTypes &type);
SensorTypes OI_CORE_EXPORT getSensorTypeEnum(const QString &name);

//##########################################
//global functions to query sensor functions
//##########################################

QList<SensorFunctions> OI_CORE_EXPORT getAvailableSensorFunctions();
const QString OI_CORE_EXPORT &getSensorFunctionName(const SensorFunctions &type);
SensorFunctions OI_CORE_EXPORT getSensorFunctionEnum(const QString &name);

//#########################################
//global function to query connection types
//#########################################

QList<ConnectionTypes> OI_CORE_EXPORT getAvailableConnectionTypes();
const QString OI_CORE_EXPORT &getConnectionTypeName(const ConnectionTypes &type);
ConnectionTypes OI_CORE_EXPORT getConnectionTypeEnum(const QString &name);

//####################################
//global functions to query tool types
//####################################

QList<ToolTypes> OI_CORE_EXPORT getAvailableToolTypes();
const QString OI_CORE_EXPORT &getToolTypeName(const ToolTypes &type);
ToolTypes OI_CORE_EXPORT getToolTypeEnum(const QString &name);

//#######################################
//global functions to query message types
//#######################################

QList<MessageTypes> OI_CORE_EXPORT getAvailableMessageTypes();
const QString OI_CORE_EXPORT &getMessageTypeName(const MessageTypes &type);
MessageTypes OI_CORE_EXPORT getMessageTypeEnum(const QString &name);

//############################################
//global functions to query unknown parameters
//############################################

QList<UnknownParameters> OI_CORE_EXPORT getAvailableUnknownParameters();
const QString OI_CORE_EXPORT &getUnknownParameterName(const UnknownParameters &parameter);
UnknownParameters OI_CORE_EXPORT getUnknownParameterEnum(const QString &name);

QList<UnknownParameters> OI_CORE_EXPORT getUnknownParameters(const GeometryTypes &type);

}

#endif // UTIL_H
