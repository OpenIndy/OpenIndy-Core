#ifndef TYPES_H
#define TYPES_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QtCore/QtGlobal>

#if defined(OI_CORE_LIB)
#  define OI_CORE_EXPORT Q_DECL_EXPORT
#else
#  define OI_CORE_EXPORT Q_DECL_IMPORT
#endif

namespace oi{

//#######################
//define scalar constants
//#######################

#define PI 3.141592653589793
#define RHO_DEGREE  (180.0/PI)
#define RHO_GON (200.0/PI)

//#######################
//available message types
//#######################

//! all types of messages
enum MessageTypes{
    eInformationMessage = 0,
    eWarningMessage,
    eErrorMessage,
    eCriticalMessage,
    eQuestionMessage
};

//! used to specify where to print a message
enum MessageDestinations{
    eConsoleMessage = 0,
    eMessageBoxMessage,
    eStatusBarMessage
};

//#######################
//available element types
//#######################

//! all OpenIndy element types
enum ElementTypes{

    //geometry types
    eCircleElement = 0,
    eConeElement,
    eCylinderElement,
    eEllipseElement,
    eEllipsoidElement,
    eHyperboloidElement,
    eLineElement,
    eNurbsElement,
    eParaboloidElement,
    ePlaneElement,
    ePointElement,
    ePointCloudElement,
    eScalarEntityAngleElement,
    eScalarEntityDistanceElement,
    eScalarEntityMeasurementSeriesElement,
    eScalarEntityTemperatureElement,
    eSlottedHoleElement,
    eSphereElement,
    eTorusElement,

    //geometric element types
    eDirectionElement,
    ePositionElement,
    eRadiusElement,

    //general feature types
    eCoordinateSystemElement,
    eStationElement,
    eTrafoParamElement,

    //observation and reading types
    eObservationElement,
    eReadingCartesianElement,
    eReadingPolarElement,
    eReadingDistanceElement,
    eReadingDirectionElement,
    eReadingTemperatureElement,
    eReadingLevelElement,

    eUndefinedElement,
    eLevelElement

};

//! all OpenIndy feature types
enum FeatureTypes{
    // keep order !
    // geometry types
    eCircleFeature = 0,
    eConeFeature,
    eCylinderFeature,
    eEllipseFeature,
    eEllipsoidFeature,
    eHyperboloidFeature,
    eLineFeature,
    eNurbsFeature,
    eParaboloidFeature,
    ePlaneFeature,
    ePointFeature,
    ePointCloudFeature,
    eScalarEntityAngleFeature,
    eScalarEntityDistanceFeature,
    eScalarEntityMeasurementSeriesFeature,
    eScalarEntityTemperatureFeature,
    eSlottedHoleFeature,
    eSphereFeature,
    eTorusFeature,

    //general feature types
    eCoordinateSystemFeature,
    eStationFeature,
    eTrafoParamFeature,

    eUndefinedFeature,
    eLevelFeature,

};

//! all OpenIndy geometry types
enum GeometryTypes{

    eCircleGeometry = 0,
    eConeGeometry,
    eCylinderGeometry,
    eEllipseGeometry,
    eEllipsoidGeometry,
    eHyperboloidGeometry,
    eLineGeometry,
    eNurbsGeometry,
    eParaboloidGeometry,
    ePlaneGeometry,
    ePointGeometry,
    ePointCloudGeometry,
    eScalarEntityAngleGeometry,
    eScalarEntityDistanceGeometry,
    eScalarEntityMeasurementSeriesGeometry,
    eScalarEntityTemperatureGeometry,
    eSlottedHoleGeometry,
    eSphereGeometry,
    eTorusGeometry,

    ePlaneLevelGeometry, // special plane
    eUndefinedGeometry

};

//################################
//all available unknown parameters
//################################

//unknown parameters of all available geometry types
enum GeometryParameters{

    eUnknownX = 0, //point coordinates
    eUnknownY,
    eUnknownZ,
    eUnknownPrimaryI, //primary vector
    eUnknownPrimaryJ,
    eUnknownPrimaryK,
    eUnknownSecondaryI, //secondary vector
    eUnknownSecondaryJ,
    eUnknownSecondaryK,
    eUnknownRadiusA, //radius
    eUnknownRadiusB, //(torus)
    eUnknownAperture, //opening angle (cone)
    eUnknownA, //semi-major axis
    eUnknownB, //semi-minor axis
    eUnknownC, //(hyperboloid)
    eUnknownAngle, //scalar entities
    eUnknownDistance,
    eUnknownMeasurementSeries,
    eUnknownTemperature,
    eUnknownLength //(slotted hole)

};

//unknown parameters of trafo param features
enum TrafoParamParameters{

    eUnknownTX = 0, //translation
    eUnknownTY,
    eUnknownTZ,
    eUnknownRX, //rotation
    eUnknownRY,
    eUnknownRZ,
    eUnknownSX, //scale
    eUnknownSY,
    eUnknownSZ

};

//#######################
//available reading types
//#######################

enum ReadingTypes{

    eDistanceReading = 0,
    eCartesianReading,
    ePolarReading,
    eDirectionReading,
    eTemperatureReading,
    eLevelReading,
    eUndefinedReading,
    eCartesianReading6D,

};

//############################
//available display attributes
//############################

enum FeatureDisplayAttributes{

    //general attributes
    eFeatureDisplayType = 0,
    eFeatureDisplayName,
    eFeatureDisplayComment,
    eFeatureDisplayGroup,
    eFeatureDisplayIsSolved,
    eFeatureDisplayIsUpdated,
    eFeatureDisplayFunctions,
    eFeatureDisplayUsedFor,
    eFeatureDisplayPreviouslyNeeded,
    eFeatureDisplayStDev,
    // for new enum check range in util.cpp getIsFeatureDisplayAttribute && featureDisplayAttributes.append(i)

    //geometry specific
    eFeatureDisplayMeasurementConfig = 100,
    eFeatureDisplayObservations,
    eFeatureDisplayIsCommon,
    eFeatureDisplayIsActual,
    // for new enum check range in util.cpp getIsFeatureDisplayAttribute  && featureDisplayAttributes.append(i)

    //unknown parameters
    eFeatureDisplayX = 200,
    eFeatureDisplayY,
    eFeatureDisplayZ,
    eFeatureDisplayPrimaryI,
    eFeatureDisplayPrimaryJ,
    eFeatureDisplayPrimaryK,
    eFeatureDisplayRadiusA,
    eFeatureDisplayRadiusB,
    eFeatureDisplaySecondaryI,
    eFeatureDisplaySecondaryJ,
    eFeatureDisplaySecondaryK,
    eFeatureDisplayAperture,
    eFeatureDisplayA,
    eFeatureDisplayB,
    eFeatureDisplayC,
    eFeatureDisplayAngle,
    eFeatureDisplayDistance,
    eFeatureDisplayMeasurementSeries,
    eFeatureDisplayTemperature,
    eFeatureDisplayLength,
    eFeatureDisplayFormError,
    // for new enum check range in util.cpp getIsFeatureDisplayAttribute && featureDisplayAttributes.append(i)

    //coordinate system specific
    eFeatureDisplayExpansionOriginX = 300,
    eFeatureDisplayExpansionOriginY,
    eFeatureDisplayExpansionOriginZ
    // for new enum check range in util.cpp getIsFeatureDisplayAttribute && featureDisplayAttributes.append(i)

};

enum TrafoParamDisplayAttributes{

    //general attributes
    eTrafoParamDisplayType = 0,
    eTrafoParamDisplayName,
    eTrafoParamDisplayComment,
    eTrafoParamDisplayGroup,
    eTrafoParamDisplayIsSolved,
    eTrafoParamDisplayIsUpdated,
    eTrafoParamDisplayFunctions,
    eTrafoParamDisplayUsedFor,
    eTrafoParamDisplayPreviouslyNeeded,
    eTrafoParamDisplayStDev,

    //trafo param specific attributes
    eTrafoParamDisplayStartSystem = 400,
    eTrafoParamDisplayDestinationSystem,
    eTrafoParamDisplayTranslationX,
    eTrafoParamDisplayTranslationY,
    eTrafoParamDisplayTranslationZ,
    eTrafoParamDisplayRotationX,
    eTrafoParamDisplayRotationY,
    eTrafoParamDisplayRotationZ,
    eTrafoParamDisplayScaleX,
    eTrafoParamDisplayScaleY,
    eTrafoParamDisplayScaleZ,
    eTrafoParamDisplayIsUsed,
    eTrafoParamDisplayValidTime,
    eTrafoParamDisplayIsMovement,
    eTrafoParamDisplayIsDatumTransformation
    // for new enum check range featureDisplayAttributes.append(i)
};

enum ObservationDisplayAttributes{

    eObservationDisplayId = 0,
    eObservationDisplayStation,
    eObservationDisplayTargetGeometries,
    eObservationDisplayX,
    eObservationDisplayY,
    eObservationDisplayZ,
    eObservationDisplayI,
    eObservationDisplayJ,
    eObservationDisplayK,
    eObservationDisplaySigmaX,
    eObservationDisplaySigmaY,
    eObservationDisplaySigmaZ,
    eObservationDisplaySigmaI,
    eObservationDisplaySigmaJ,
    eObservationDisplaySigmaK,
    eObservationDisplayIsValid,
    eObservationDisplayIsSolved,
    eObservationDisplayVX,
    eObservationDisplayVY,
    eObservationDisplayVZ,
    eObservationDisplayV,
    eObservationDisplayVR,
    eObservationDisplayVI,
    eObservationDisplayVJ,
    eObservationDisplayVK,
    eObservationDisplayIsUsed,
    eObservationDisplayIsDummyPoint,
    eObservationDisplayReadingTime

};

enum ReadingDisplayAttributes{
    // order corresponds to the display order
    eReadingDisplayId = 0,
    eReadingDisplayType,
    eReadingDisplayTime,
    eReadingDisplaySensorConfigName,
    eReadingDisplayMeasurementConfigName,
    eReadingDisplaySide,
    eReadingDisplayAzimuth,
    eReadingDisplayZenith,
    eReadingDisplayDistance,
    eReadingDisplayX,
    eReadingDisplayY,
    eReadingDisplayZ,
    eReadingDisplayI,
    eReadingDisplayJ,
    eReadingDisplayK,
    eReadingDisplayTemperature,
    eReadingDisplaySigmaAzimuth,
    eReadingDisplaySigmaZenith,
    eReadingDisplaySigmaDistance,
    eReadingDisplaySigmaX,
    eReadingDisplaySigmaY,
    eReadingDisplaySigmaZ,
    eReadingDisplaySigmaI,
    eReadingDisplaySigmaJ,
    eReadingDisplaySigmaK,
    eReadingDisplaySigmaTemperature,
    eReadingDisplayImported

};

//###############
//available units
//###############

enum UnitType{

    //metric unit types
    eUnitMilliMeter = 0,
    eUnitMeter,
    eUnitInch,

    //angular unit types
    eUnitRadiant,
    eUnitMilliRadians,
    eUnitGon,
    eUnitDecimalDegree,
    eUnitArcSeconds,

    //temperature unit types
    eUnitGrad,
    eUnitKelvin,
    eUnitFahrenheit,

    eNoUnit

};

enum DimensionType{

    eMetric = 0,
    eAngular,
    eTemperature,
    eDimensionless

};

//#####################
//sensor specific enums
//#####################

enum SensorTypes{

    eLaserTracker = 0,
    eTotalStation,
    eUndefinedSensor

};

enum SensorFaces{

    eFrontSide = 0,
    eBackSide,
    eUndefinedSide // e.g. not measured

};

enum ConnectionTypes{

    eNetworkConnection = 0,
    eSerialConnection

};

enum SensorFunctions{

    eMoveAngle = 0,
    eMoveXYZ,
    eMotorState,
    eToggleSight,
    eInitialize,
    eHome,
    eCompensation,
    eMeasure,
    eConnect,
    eDisconnect,
    eStream,
    eSearch
};

//################################
//tool types (categories of tools)
//################################

enum ToolTypes{

    eReportTool = 0,
    eUnknownTool

};

//#############################
//actual nominal filter options
//#############################

enum ActualNominalFilter{

    eFilterActualNominal = 0,
    eFilterActual,
    eFilterNominal

};

//######################################
//materials for temperature compensation
//######################################
enum MaterialsTempComp{

    eMaterialSteel = 0,
    eMaterialAluminum,
    eMaterialPlumb,
    eMaterialIron,
    eMaterialGrayCastIron,
    eMaterialCopper,
    eMaterialBrass,
    eMaterialZinc,
    eMaterialPlatinum,
    eMaterialConcrete,
    eMaterialReinforcedConcrete
};

//##############
//helper classes
//##############

/*!
 * \brief The ScalarInputParams class
 * Save user specified non-element scalar input parameters
 */
class ScalarInputParams{
public:
    ScalarInputParams(){
        isValid = false;
    }

    bool isValid;
    QMap<QString, double> doubleParameter;
    QMap<QString, int> intParameter;
    QMap<QString, QString> stringParameter;
};

enum SensorStatus{

    eUnknown = 0,
    eReadyForMeasurement,    // target present, device ready, measurement arm ready
    eNotReadyForMeasurement, // target not present or device not ready or measurement arm not ready
    eSensorActionInProgress,
    eClearStatus // TODO later: define behaviour !!!
};

}

#endif // TYPES_H
