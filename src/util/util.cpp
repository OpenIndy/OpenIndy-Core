#include "util.h"

#include "featurewrapper.h"

namespace oi{

//#################################################
//maps with element enums and corresponding strings
//#################################################

namespace internal{

QMap<ElementTypes, QString> elementTypesMap;
QMap<ElementTypes, QString> elementTypesPluralMap;
QMap<FeatureTypes, QString> featureTypesMap;
QMap<GeometryTypes, QString> geometryTypesMap;

QMap<ElementTypes, QString> elementTypeIcons;
QMap<FeatureTypes, QString> featureTypeIcons;

QMap<ReadingTypes, QString> readingTypesMap;

QMap<FeatureDisplayAttributes, QString> featureDisplayAttributesMap;
QMap<TrafoParamDisplayAttributes, QString> trafoParamDisplayAttributesMap;
QMap<ObservationDisplayattributes, QString> observationDisplayAttributesMap;
QMap<ReadingDisplayAttributes, QString> readingDisplayAttributesMap;

QList<int> featureDisplayAttributes;
QList<ObservationDisplayattributes> observationDisplayAttributes;
QList<ReadingDisplayAttributes> readingDisplayAttributes;

QMap<UnitType, QString> unitTypesMap;

QMap<SensorTypes, QString> sensorTypesMap;

QMap<SensorFunctions, QString> sensorFunctionsMap;

QMap<ConnectionTypes, QString> connectionTypesMap;

QMap<ToolTypes, QString> toolTypesMap;

QMap<MessageTypes, QString> messageTypesMap;

QMap<UnknownParameters, QString> unknownParametersMap;

QString undefined = "";

bool isInit = false;

void init(){

    //clear maps
    elementTypesMap.clear();
    elementTypesPluralMap.clear();
    featureTypesMap.clear();
    geometryTypesMap.clear();
    readingTypesMap.clear();
    featureDisplayAttributesMap.clear();
    observationDisplayAttributesMap.clear();
    readingDisplayAttributesMap.clear();
    trafoParamDisplayAttributesMap.clear();
    featureDisplayAttributes.clear();
    observationDisplayAttributes.clear();
    readingDisplayAttributes.clear();
    unitTypesMap.clear();
    sensorTypesMap.clear();
    toolTypesMap.clear();
    messageTypesMap.clear();
    unknownParametersMap.clear();

    //fill element map
    elementTypesMap.insert(eCircleElement, "circle");
    elementTypesMap.insert(eConeElement, "cone");
    elementTypesMap.insert(eCylinderElement, "cylinder");
    elementTypesMap.insert(eEllipseElement, "ellipse");
    elementTypesMap.insert(eEllipsoidElement, "ellipsoid");
    elementTypesMap.insert(eHyperboloidElement, "hyperboloid");
    elementTypesMap.insert(eLineElement, "line");
    elementTypesMap.insert(eNurbsElement, "nurbs");
    elementTypesMap.insert(eParaboloidElement, "paraboloid");
    elementTypesMap.insert(ePlaneElement, "plane");
    elementTypesMap.insert(ePointElement, "point");
    elementTypesMap.insert(ePointCloudElement, "point cloud");
    elementTypesMap.insert(eScalarEntityAngleElement, "angle");
    elementTypesMap.insert(eScalarEntityDistanceElement, "distance");
    elementTypesMap.insert(eScalarEntityMeasurementSeriesElement, "measurement series");
    elementTypesMap.insert(eScalarEntityTemperatureElement, "temperature");
    elementTypesMap.insert(eSlottedHoleElement, "slotted hole");
    elementTypesMap.insert(eSphereElement, "sphere");
    elementTypesMap.insert(eTorusElement, "torus");
    elementTypesMap.insert(eDirectionElement, "direction");
    elementTypesMap.insert(ePositionElement, "position");
    elementTypesMap.insert(eRadiusElement, "radius");
    elementTypesMap.insert(eCoordinateSystemElement, "coordinate system");
    elementTypesMap.insert(eStationElement, "station");
    elementTypesMap.insert(eTrafoParamElement, "transformation parameters");
    elementTypesMap.insert(eObservationElement, "observation");
    elementTypesMap.insert(eReadingCartesianElement, "cartesian reading");
    elementTypesMap.insert(eReadingPolarElement, "polar reading");
    elementTypesMap.insert(eReadingDistanceElement, "distance reading");
    elementTypesMap.insert(eReadingDirectionElement, "direction reading");
    elementTypesMap.insert(eReadingTemperatureElement, "temperature reading");
    elementTypesMap.insert(eReadingLevelElement, "level reading");

    //fill element plural map
    elementTypesPluralMap.insert(eCircleElement, "circles");
    elementTypesPluralMap.insert(eConeElement, "cones");
    elementTypesPluralMap.insert(eCylinderElement, "cylinders");
    elementTypesPluralMap.insert(eEllipseElement, "ellipses");
    elementTypesPluralMap.insert(eEllipsoidElement, "ellipsoids");
    elementTypesPluralMap.insert(eHyperboloidElement, "hyperboloids");
    elementTypesPluralMap.insert(eLineElement, "lines");
    elementTypesPluralMap.insert(eNurbsElement, "nurbs");
    elementTypesPluralMap.insert(eParaboloidElement, "paraboloids");
    elementTypesPluralMap.insert(ePlaneElement, "planes");
    elementTypesPluralMap.insert(ePointElement, "points");
    elementTypesPluralMap.insert(ePointCloudElement, "point clouds");
    elementTypesPluralMap.insert(eScalarEntityAngleElement, "angles");
    elementTypesPluralMap.insert(eScalarEntityDistanceElement, "distances");
    elementTypesPluralMap.insert(eScalarEntityMeasurementSeriesElement, "measurement series");
    elementTypesPluralMap.insert(eScalarEntityTemperatureElement, "temperatures");
    elementTypesPluralMap.insert(eSlottedHoleElement, "slotted holes");
    elementTypesPluralMap.insert(eSphereElement, "spheres");
    elementTypesPluralMap.insert(eTorusElement, "tori");
    elementTypesPluralMap.insert(eDirectionElement, "directions");
    elementTypesPluralMap.insert(ePositionElement, "positions");
    elementTypesPluralMap.insert(eRadiusElement, "radii");
    elementTypesPluralMap.insert(eCoordinateSystemElement, "coordinate systems");
    elementTypesPluralMap.insert(eStationElement, "stations");
    elementTypesPluralMap.insert(eTrafoParamElement, "transformation parameters");
    elementTypesPluralMap.insert(eObservationElement, "observations");
    elementTypesPluralMap.insert(eReadingCartesianElement, "cartesian readings");
    elementTypesPluralMap.insert(eReadingPolarElement, "polar readings");
    elementTypesPluralMap.insert(eReadingDistanceElement, "distance readings");
    elementTypesPluralMap.insert(eReadingDirectionElement, "direction readings");
    elementTypesPluralMap.insert(eReadingTemperatureElement, "temperature readings");
    elementTypesPluralMap.insert(eReadingLevelElement, "level readings");

    //fill feature map
    featureTypesMap.insert(eCircleFeature, "circle");
    featureTypesMap.insert(eConeFeature, "cone");
    featureTypesMap.insert(eCylinderFeature, "cylinder");
    featureTypesMap.insert(eEllipseFeature, "ellipse");
    featureTypesMap.insert(eEllipsoidFeature, "ellipsoid");
    featureTypesMap.insert(eHyperboloidFeature, "hyperboloid");
    featureTypesMap.insert(eLineFeature, "line");
    featureTypesMap.insert(eNurbsFeature, "nurbs");
    featureTypesMap.insert(eParaboloidFeature, "paraboloid");
    featureTypesMap.insert(ePlaneFeature, "plane");
    featureTypesMap.insert(ePointFeature, "point");
    featureTypesMap.insert(ePointCloudFeature, "point cloud");
    featureTypesMap.insert(eScalarEntityAngleFeature, "angle");
    featureTypesMap.insert(eScalarEntityDistanceFeature, "distance");
    featureTypesMap.insert(eScalarEntityMeasurementSeriesFeature, "measurement series");
    featureTypesMap.insert(eScalarEntityTemperatureFeature, "temperature");
    featureTypesMap.insert(eSlottedHoleFeature, "slotted hole");
    featureTypesMap.insert(eSphereFeature, "sphere");
    featureTypesMap.insert(eTorusFeature, "torus");
    featureTypesMap.insert(eCoordinateSystemFeature, "coordinate system");
    featureTypesMap.insert(eStationFeature, "station");
    featureTypesMap.insert(eTrafoParamFeature, "transformation parameters");

    //fill geometry map
    geometryTypesMap.insert(eCircleGeometry, "circle");
    geometryTypesMap.insert(eConeGeometry, "cone");
    geometryTypesMap.insert(eCylinderGeometry, "cylinder");
    geometryTypesMap.insert(eEllipseGeometry, "ellipse");
    geometryTypesMap.insert(eEllipsoidGeometry, "ellipsoid");
    geometryTypesMap.insert(eHyperboloidGeometry, "hyperboloid");
    geometryTypesMap.insert(eLineGeometry, "line");
    geometryTypesMap.insert(eNurbsGeometry, "nurbs");
    geometryTypesMap.insert(eParaboloidGeometry, "paraboloid");
    geometryTypesMap.insert(ePlaneGeometry, "plane");
    geometryTypesMap.insert(ePointGeometry, "point");
    geometryTypesMap.insert(ePointCloudGeometry, "point cloud");
    geometryTypesMap.insert(eScalarEntityAngleGeometry, "angle");
    geometryTypesMap.insert(eScalarEntityDistanceGeometry, "distance");
    geometryTypesMap.insert(eScalarEntityMeasurementSeriesGeometry, "measurement series");
    geometryTypesMap.insert(eScalarEntityTemperatureGeometry, "temperature");
    geometryTypesMap.insert(eSlottedHoleGeometry, "slotted hole");
    geometryTypesMap.insert(eSphereGeometry, "sphere");
    geometryTypesMap.insert(eTorusGeometry, "torus");

    //fill element type icons
    elementTypeIcons.insert(eCircleElement, ":/Images/icons/Circle.svg");
    elementTypeIcons.insert(eConeElement, ":/Images/icons/Cone.svg");
    elementTypeIcons.insert(eCylinderElement, ":/Images/icons/Cylinder.svg");
    elementTypeIcons.insert(eEllipseElement, ":/Images/icons/Ellipse.svg");
    elementTypeIcons.insert(eEllipsoidElement, ":/Images/icons/Ellipsoid.svg");
    elementTypeIcons.insert(eHyperboloidElement, ":/Images/icons/Hyperboloid.svg");
    elementTypeIcons.insert(eLineElement, ":/Images/icons/Line.svg");
    elementTypeIcons.insert(eNurbsElement, ":/Images/icons/Nurbs Surface.svg");
    elementTypeIcons.insert(eParaboloidElement, ":/Images/icons/Paraboloid.svg");
    elementTypeIcons.insert(ePlaneElement, ":/Images/icons/Plane.svg");
    elementTypeIcons.insert(ePointElement, ":/Images/icons/Point.svg");
    elementTypeIcons.insert(ePointCloudElement, ":/Images/icons/Point Cloud.svg");
    elementTypeIcons.insert(eScalarEntityAngleElement, ":/Images/icons/Scalar Entities.svg");
    elementTypeIcons.insert(eScalarEntityDistanceElement, ":/Images/icons/Scalar Entities.svg");
    elementTypeIcons.insert(eScalarEntityMeasurementSeriesElement, ":/Images/icons/Scalar Entities.svg");
    elementTypeIcons.insert(eScalarEntityTemperatureElement, ":/Images/icons/Scalar Entities.svg");
    elementTypeIcons.insert(eSlottedHoleElement, ":/Images/icons/Slotted Hole.svg");
    elementTypeIcons.insert(eSphereElement, ":/Images/icons/Sphere.svg");
    elementTypeIcons.insert(eTorusElement, ":/Images/icons/Torus.svg");
    elementTypeIcons.insert(eDirectionElement, "");
    elementTypeIcons.insert(ePositionElement, "");
    elementTypeIcons.insert(eRadiusElement, "");
    elementTypeIcons.insert(eCoordinateSystemElement, ":/Images/icons/Coordinatesystem.svg");
    elementTypeIcons.insert(eStationElement, ":/Images/icons/Station.svg");
    elementTypeIcons.insert(eTrafoParamElement, ":/Images/icons/TrafoParam.svg");
    elementTypeIcons.insert(eObservationElement, "");
    elementTypeIcons.insert(eReadingCartesianElement, "");
    elementTypeIcons.insert(eReadingPolarElement, "");
    elementTypeIcons.insert(eReadingDistanceElement, "");
    elementTypeIcons.insert(eReadingDirectionElement, "");
    elementTypeIcons.insert(eReadingTemperatureElement, "");
    elementTypeIcons.insert(eReadingLevelElement, "");

    //fill feature type icons
    featureTypeIcons.insert(eCircleFeature, ":/Images/icons/Circle.svg");
    featureTypeIcons.insert(eConeFeature, ":/Images/icons/Cone.svg");
    featureTypeIcons.insert(eCylinderFeature, ":/Images/icons/Cylinder.svg");
    featureTypeIcons.insert(eEllipseFeature, ":/Images/icons/Ellipse.svg");
    featureTypeIcons.insert(eEllipsoidFeature, ":/Images/icons/Ellipsoid.svg");
    featureTypeIcons.insert(eHyperboloidFeature, ":/Images/icons/Hyperboloid.svg");
    featureTypeIcons.insert(eLineFeature, ":/Images/icons/Line.svg");
    featureTypeIcons.insert(eNurbsFeature, ":/Images/icons/Nurbs Surface.svg");
    featureTypeIcons.insert(eParaboloidFeature, ":/Images/icons/Paraboloid.svg");
    featureTypeIcons.insert(ePlaneFeature, ":/Images/icons/Plane.svg");
    featureTypeIcons.insert(ePointFeature, ":/Images/icons/Point.svg");
    featureTypeIcons.insert(ePointCloudFeature, ":/Images/icons/Point Cloud.svg");
    featureTypeIcons.insert(eScalarEntityAngleFeature, ":/Images/icons/Scalar Entities.svg");
    featureTypeIcons.insert(eScalarEntityDistanceFeature, ":/Images/icons/Scalar Entities.svg");
    featureTypeIcons.insert(eScalarEntityMeasurementSeriesFeature, ":/Images/icons/Scalar Entities.svg");
    featureTypeIcons.insert(eScalarEntityTemperatureFeature, ":/Images/icons/Scalar Entities.svg");
    featureTypeIcons.insert(eSlottedHoleFeature, ":/Images/icons/Slotted Hole.svg");
    featureTypeIcons.insert(eSphereFeature, ":/Images/icons/Sphere.svg");
    featureTypeIcons.insert(eTorusFeature, ":/Images/icons/Torus.svg");
    featureTypeIcons.insert(eCoordinateSystemFeature, ":/Images/icons/Coordinatesystem.svg");
    featureTypeIcons.insert(eStationFeature, ":/Images/icons/Station.svg");
    featureTypeIcons.insert(eTrafoParamFeature, ":/Images/icons/TrafoParam.svg");

    //fill reading map
    readingTypesMap.insert(eDistanceReading, "distance");
    readingTypesMap.insert(eCartesianReading, "cartesian");
    readingTypesMap.insert(ePolarReading, "polar");
    readingTypesMap.insert(eDirectionReading, "direction");
    readingTypesMap.insert(eTemperatureReading, "temperature");
    readingTypesMap.insert(eLevelReading, "level");

    //fill feature display attributes map
    featureDisplayAttributesMap.insert(eFeatureDisplayType, "type");
    featureDisplayAttributesMap.insert(eFeatureDisplayName, "name");
    featureDisplayAttributesMap.insert(eFeatureDisplayComment, "comment");
    featureDisplayAttributesMap.insert(eFeatureDisplayGroup, "group");
    featureDisplayAttributesMap.insert(eFeatureDisplayIsSolved, "solved");
    featureDisplayAttributesMap.insert(eFeatureDisplayIsUpdated, "updated");
    featureDisplayAttributesMap.insert(eFeatureDisplayFunctions, "functions");
    featureDisplayAttributesMap.insert(eFeatureDisplayUsedFor, "used for");
    featureDisplayAttributesMap.insert(eFeatureDisplayPreviouslyNeeded, "previously needed");
    featureDisplayAttributesMap.insert(eFeatureDisplayStDev, "stdev");
    featureDisplayAttributesMap.insert(eFeatureDisplayMeasurementConfig, "measurement config");
    featureDisplayAttributesMap.insert(eFeatureDisplayObservations, "observations");
    featureDisplayAttributesMap.insert(eFeatureDisplayX, "x");
    featureDisplayAttributesMap.insert(eFeatureDisplayY, "y");
    featureDisplayAttributesMap.insert(eFeatureDisplayZ, "z");
    featureDisplayAttributesMap.insert(eFeatureDisplayPrimaryI, "i");
    featureDisplayAttributesMap.insert(eFeatureDisplayPrimaryJ, "j");
    featureDisplayAttributesMap.insert(eFeatureDisplayPrimaryK, "k");
    featureDisplayAttributesMap.insert(eFeatureDisplayRadiusA, "radius");
    featureDisplayAttributesMap.insert(eFeatureDisplayRadiusB, "radius 2");
    featureDisplayAttributesMap.insert(eFeatureDisplaySecondaryI, "i 2");
    featureDisplayAttributesMap.insert(eFeatureDisplaySecondaryJ, "j 2");
    featureDisplayAttributesMap.insert(eFeatureDisplaySecondaryK, "k 2");
    featureDisplayAttributesMap.insert(eFeatureDisplayAperture, "aperture");
    featureDisplayAttributesMap.insert(eFeatureDisplayA, "a");
    featureDisplayAttributesMap.insert(eFeatureDisplayB, "b");
    featureDisplayAttributesMap.insert(eFeatureDisplayC, "c");
    featureDisplayAttributesMap.insert(eFeatureDisplayAngle, "angle");
    featureDisplayAttributesMap.insert(eFeatureDisplayDistance, "distance");
    featureDisplayAttributesMap.insert(eFeatureDisplayMeasurementSeries, "measurement series");
    featureDisplayAttributesMap.insert(eFeatureDisplayTemperature, "temperature");
    featureDisplayAttributesMap.insert(eFeatureDisplayLength, "length");
    featureDisplayAttributesMap.insert(eFeatureDisplayExpansionOriginX, "expansion x");
    featureDisplayAttributesMap.insert(eFeatureDisplayExpansionOriginY, "expansion y");
    featureDisplayAttributesMap.insert(eFeatureDisplayExpansionOriginZ, "expansion z");

    //fill observation display attributes map
    observationDisplayAttributesMap.insert(eObservationDisplayId, "id");
    observationDisplayAttributesMap.insert(eObservationDisplayStation, "station");
    observationDisplayAttributesMap.insert(eObservationDisplayTargetGeometries, "target geometries");
    observationDisplayAttributesMap.insert(eObservationDisplayX, "x");
    observationDisplayAttributesMap.insert(eObservationDisplayY, "y");
    observationDisplayAttributesMap.insert(eObservationDisplayZ, "z");
    observationDisplayAttributesMap.insert(eObservationDisplaySigmaX, "sigma x");
    observationDisplayAttributesMap.insert(eObservationDisplaySigmaY, "sigma y");
    observationDisplayAttributesMap.insert(eObservationDisplaySigmaZ, "sigma z");
    observationDisplayAttributesMap.insert(eObservationDisplayIsValid, "valid");
    observationDisplayAttributesMap.insert(eObservationDisplayIsSolved, "solved");

    //fill reading display attributes map
    readingDisplayAttributesMap.insert(eReadingDisplayId, "id");
    readingDisplayAttributesMap.insert(eReadingDisplayType, "type");
    readingDisplayAttributesMap.insert(eReadingDisplayTime, "time");
    readingDisplayAttributesMap.insert(eReadingDisplaySensor, "sensor");
    readingDisplayAttributesMap.insert(eReadingDisplayIsFrontSide, "front side");
    readingDisplayAttributesMap.insert(eReadingDisplayAzimuth, "azimuth");
    readingDisplayAttributesMap.insert(eReadingDisplayZenith, "zenith");
    readingDisplayAttributesMap.insert(eReadingDisplayDistance, "distance");
    readingDisplayAttributesMap.insert(eReadingDisplayX, "x");
    readingDisplayAttributesMap.insert(eReadingDisplayY, "y");
    readingDisplayAttributesMap.insert(eReadingDisplayZ, "z");
    readingDisplayAttributesMap.insert(eReadingDisplayRX, "RX");
    readingDisplayAttributesMap.insert(eReadingDisplayRY, "RY");
    readingDisplayAttributesMap.insert(eReadingDisplayRZ, "RZ");
    readingDisplayAttributesMap.insert(eReadingDisplayTemperature, "temperature");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaAzimuth, "sigma azimuth");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaZenith, "sigma zenith");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaDistance, "sigma distance");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaX, "sigma x");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaY, "sigma y");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaZ, "sigma z");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaRX, "sigma RX");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaRY, "sigma RY");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaRZ, "sigma RZ");
    readingDisplayAttributesMap.insert(eReadingDisplaySigmaTemperature, "sigma temperature");

    //fill trafo param display attributes map
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayType, "type");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayName, "name");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayComment, "comment");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayGroup, "group");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsSolved, "solved");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsUpdated, "updated");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayFunctions, "functions");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayUsedFor, "used for");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayPreviouslyNeeded, "previously needed");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayStDev, "stdev");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayStartSystem, "start");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayDestinationSystem, "destination");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayTranslationX, "tx");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayTranslationY, "ty");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayTranslationZ, "tz");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayRotationX, "rx");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayRotationY, "ry");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayRotationZ, "rz");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayScaleX, "sx");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayScaleY, "sy");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayScaleZ, "sz");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsUsed, "used");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayValidTime, "time");
    trafoParamDisplayAttributesMap.insert(eTrafoParamDisplayIsMovement, "movement");

    //fill available display attributes
    for(int i = 0; i < 10; i++){ //general feature attributes
        featureDisplayAttributes.append(i);
    }
    for(int i = 100; i < 102; i++){ //geometry specific attributes
        featureDisplayAttributes.append(i);
    }
    for(int i = 200; i < 220; i++){ //unknown geometry parameters
        featureDisplayAttributes.append(i);
    }
    for(int i = 300; i < 303; i++){ //coordinate system specific
        featureDisplayAttributes.append(i);
    }
    for(int i = 400; i < 414; i++){ //trafo param specific
        featureDisplayAttributes.append(i);
    }

    //fill available observation display attributes
    foreach(const ObservationDisplayattributes &attr, observationDisplayAttributesMap.keys()){
        observationDisplayAttributes.append(attr);
    }

    //fill available reading display attributes
    foreach(const ReadingDisplayAttributes &attr, readingDisplayAttributesMap.keys()){
        readingDisplayAttributes.append(attr);
    }

    //fill unit types map
    unitTypesMap.insert(eUnitArcSeconds, "[arcsec]");
    unitTypesMap.insert(eUnitDecimalDegree, "[°]");
    unitTypesMap.insert(eUnitFahrenheit, "[°F]");
    unitTypesMap.insert(eUnitGon, "[gon]");
    unitTypesMap.insert(eUnitGrad, "[°C]");
    unitTypesMap.insert(eUnitInch, "[inch]");
    unitTypesMap.insert(eUnitKelvin, "[°K]");
    unitTypesMap.insert(eUnitMeter, "[m]");
    unitTypesMap.insert(eUnitMilliMeter, "[mm]");
    unitTypesMap.insert(eUnitMilliRadians, "[mrad]");
    unitTypesMap.insert(eUnitRadiant, "[rad]");

    //fill sensor types map
    sensorTypesMap.insert(eLaserTracker, "laser tracker");
    sensorTypesMap.insert(eTotalStation, "total station");

    //fill sensor functions map
    sensorFunctionsMap.insert(eMoveAngle, "move (polar)");
    sensorFunctionsMap.insert(eMoveXYZ, "move (cartesian)");
    sensorFunctionsMap.insert(eMotorState, "change motor state");
    sensorFunctionsMap.insert(eToggleSight, "toggle sight orientation");
    sensorFunctionsMap.insert(eInitialize, "initialize");
    sensorFunctionsMap.insert(eHome, "home");
    sensorFunctionsMap.insert(eCompensation, "compensation");

    //fill connection types map
    connectionTypesMap.insert(eSerialConnection, "serial");
    connectionTypesMap.insert(eNetworkConnection, "network");

    //fill tool types map
    toolTypesMap.insert(eReportTool, "report");
    toolTypesMap.insert(eUnknownTool, "tool");

    //fill message types map
    messageTypesMap.insert(eInformationMessage, "information");
    messageTypesMap.insert(eWarningMessage, "warning");
    messageTypesMap.insert(eErrorMessage, "error");
    messageTypesMap.insert(eCriticalMessage, "critical");

    //fill unknown parameters map
    unknownParametersMap.insert(eUnknownX, "x");
    unknownParametersMap.insert(eUnknownY, "y");
    unknownParametersMap.insert(eUnknownZ, "z");
    unknownParametersMap.insert(eUnknownPrimaryI, "i");
    unknownParametersMap.insert(eUnknownPrimaryJ, "j");
    unknownParametersMap.insert(eUnknownPrimaryK, "k");
    unknownParametersMap.insert(eUnknownSecondaryI, "i 2");
    unknownParametersMap.insert(eUnknownSecondaryJ, "j 2");
    unknownParametersMap.insert(eUnknownSecondaryK, "k 2");
    unknownParametersMap.insert(eUnknownRadiusA, "radius");
    unknownParametersMap.insert(eUnknownRadiusB, "radius 2");
    unknownParametersMap.insert(eUnknownAperture, "aperture");
    unknownParametersMap.insert(eUnknownA, "a");
    unknownParametersMap.insert(eUnknownB, "b");
    unknownParametersMap.insert(eUnknownC, "c");
    unknownParametersMap.insert(eUnknownAngle, "angle");
    unknownParametersMap.insert(eUnknownDistance, "distance");
    unknownParametersMap.insert(eUnknownMeasurementSeries, "measurement series");
    unknownParametersMap.insert(eUnknownTemperature, "temperature");
    unknownParametersMap.insert(eUnknownLength, "length");

    isInit = true;

}

}

//#######################################
//global functions to query element types
//#######################################

/*!
 * \brief getAvailableElementTypes
 * \return
 */
QList<ElementTypes> getAvailableElementTypes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::elementTypesMap.keys();

}

/*!
 * \brief getAvailableFeatureTypes
 * \return
 */
QList<FeatureTypes> getAvailableFeatureTypes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::featureTypesMap.keys();

}

/*!
 * \brief getAvailableGeometryTypes
 * \return
 */
QList<GeometryTypes> getAvailableGeometryTypes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::geometryTypesMap.keys();

}

/*!
 * \brief getElementTypeName
 * \param type
 * \return
 */
const QString &getElementTypeName(const ElementTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding element name
    if(internal::elementTypesMap.contains(type)){
        return internal::elementTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getElementTypePluralName
 * \param type
 * \return
 */
const QString &getElementTypePluralName(const ElementTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding element name
    if(internal::elementTypesPluralMap.contains(type)){
        return internal::elementTypesPluralMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getElementTypeEnum
 * \param name
 * \return
 */
ElementTypes getElementTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding element enum value
    ElementTypes type = internal::elementTypesMap.key(name, eUndefinedElement);
    if(type == eUndefinedElement){
        type = internal::elementTypesPluralMap.key(name, eUndefinedElement);;
    }

    return type;

}

/*!
 * \brief getElementTypeEnum
 * Converts FeatureTypes to ElementTypes
 * \param type
 * \return
 */
ElementTypes getElementTypeEnum(const FeatureTypes &type){

    switch(type){
    case eCircleFeature:
        return eCircleElement;
    case eConeFeature:
        return eConeElement;
    case eCylinderFeature:
        return eCylinderElement;
    case eEllipseFeature:
        return eEllipseElement;
    case eEllipsoidFeature:
        return eEllipsoidElement;
    case eHyperboloidFeature:
        return eHyperboloidElement;
    case eLineFeature:
        return eLineElement;
    case eNurbsFeature:
        return eNurbsElement;
    case eParaboloidFeature:
        return eParaboloidElement;
    case ePlaneFeature:
        return ePlaneElement;
    case ePointFeature:
        return ePointElement;
    case ePointCloudFeature:
        return ePointCloudElement;
    case eScalarEntityAngleFeature:
        return eScalarEntityAngleElement;
    case eScalarEntityDistanceFeature:
        return eScalarEntityDistanceElement;
    case eScalarEntityMeasurementSeriesFeature:
        return eScalarEntityMeasurementSeriesElement;
    case eScalarEntityTemperatureFeature:
        return eScalarEntityTemperatureElement;
    case eSlottedHoleFeature:
        return eSlottedHoleElement;
    case eSphereFeature:
        return eSphereElement;
    case eTorusFeature:
        return eTorusElement;
    case eCoordinateSystemFeature:
        return eCoordinateSystemElement;
    case eStationFeature:
        return eStationElement;
    case eTrafoParamFeature:
        return eTrafoParamElement;
    default:
        return eUndefinedElement;
    }

}

/*!
 * \brief getElementTypeEnum
 * convert ReadingTypes to ElementTypes
 * \param type
 * \return
 */
ElementTypes getElementTypeEnum(const ReadingTypes &type){

    switch(type){
    case eCartesianReading:
        return eReadingCartesianElement;
    case eDistanceReading:
        return eReadingDistanceElement;
    case ePolarReading:
        return eReadingPolarElement;
    case eDirectionReading:
        return eReadingDirectionElement;
    case eTemperatureReading:
        return eReadingTemperatureElement;
    case eLevelReading:
        return eReadingLevelElement;
    }

    return eUndefinedElement;

}

/*!
 * \brief getFeatureTypeName
 * \param type
 * \return
 */
const QString &getFeatureTypeName(const FeatureTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding feature name
    if(internal::featureTypesMap.contains(type)){
        return internal::featureTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getFeatureTypeEnum
 * \param name
 * \return
 */
FeatureTypes getFeatureTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding feature enum value
    return internal::featureTypesMap.key(name, eUndefinedFeature);

}

/*!
 * \brief getGeometryTypeName
 * \param type
 * \return
 */
const QString &getGeometryTypeName(const GeometryTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding geometry name
    if(internal::geometryTypesMap.contains(type)){
        return internal::geometryTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getGeometryTypeEnum
 * \param name
 * \return
 */
GeometryTypes getGeometryTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding geometry enum value
    return internal::geometryTypesMap.key(name, eUndefinedGeometry);

}

/*!
 * \brief getGeometryTypeEnum
 * \param type
 * \return
 */
GeometryTypes getGeometryTypeEnum(const FeatureTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    switch(type){
    case eCircleFeature:
        return eCircleGeometry;
    case eConeFeature:
        return eConeGeometry;
    case eCylinderFeature:
        return eCylinderGeometry;
    case eEllipseFeature:
        return eEllipseGeometry;
    case eEllipsoidFeature:
        return eEllipsoidGeometry;
    case eHyperboloidFeature:
        return eHyperboloidGeometry;
    case eLineFeature:
        return eLineGeometry;
    case eNurbsFeature:
        return eNurbsGeometry;
    case eParaboloidFeature:
        return eParaboloidGeometry;
    case ePlaneFeature:
        return ePlaneGeometry;
    case ePointFeature:
        return ePointGeometry;
    case ePointCloudFeature:
        return ePointCloudGeometry;
    case eScalarEntityAngleFeature:
        return eScalarEntityAngleGeometry;
    case eScalarEntityDistanceFeature:
        return eScalarEntityDistanceGeometry;
    case eScalarEntityMeasurementSeriesFeature:
        return eScalarEntityMeasurementSeriesGeometry;
    case eScalarEntityTemperatureFeature:
        return eScalarEntityTemperatureGeometry;
    case eSlottedHoleFeature:
        return eSlottedHoleGeometry;
    case eSphereFeature:
        return eSphereGeometry;
    case eTorusFeature:
        return eTorusGeometry;
    default:
        return eUndefinedGeometry;
    }

}

/*!
 * \brief getIsFeature
 * \param type
 * \return
 */
bool getIsFeature(const ElementTypes &type){
    if(type == eObservationElement
            || type == eReadingCartesianElement
            || type == eReadingPolarElement
            || type == eReadingDistanceElement
            || type == eReadingDirectionElement
            || type == eReadingTemperatureElement
            || type == eReadingLevelElement
            || type == eDirectionElement
            || type == ePositionElement
            || type == eRadiusElement
            || type == eUndefinedElement){
        return false;
    }
    return true;
}

/*!
 * \brief getIsReading
 * \param type
 * \return
 */
bool getIsReading(const ElementTypes &type){
    if(type == eReadingCartesianElement
            || type == eReadingDirectionElement
            || type == eReadingDistanceElement
            || type == eReadingLevelElement
            || type == eReadingPolarElement
            || type == eReadingTemperatureElement){
        return true;
    }
    return false;
}

/*!
 * \brief getIsObservation
 * \param type
 * \return
 */
bool getIsObservation(const ElementTypes &type){
    if(type == eObservationElement){
        return true;
    }
    return false;
}

/*!
 * \brief getIsGeometry
 * \param type
 * \return
 */
bool getIsGeometry(const FeatureTypes &type){
    if(type == eCoordinateSystemFeature
            || type == eStationFeature
            || type == eTrafoParamFeature){
        return false;
    }
    return true;
}

/*!
 * \brief getIsScalarEntity
 * \param type
 * \return
 */
bool getIsScalarEntity(const FeatureTypes &type){
    if(type == eScalarEntityAngleFeature
            || type == eScalarEntityDistanceFeature
            || type == eScalarEntityMeasurementSeriesFeature
            || type == eScalarEntityTemperatureFeature){
        return true;
    }
    return false;
}

/*!
 * \brief getElementTypeIconPath
 * \param type
 * \return
 */
const QString &getElementTypeIconPath(const ElementTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding element type icon
    if(internal::elementTypeIcons.contains(type)){
        return internal::elementTypeIcons[type];
    }
    return internal::undefined;

}

/*!
 * \brief getFeatureTypeIconPath
 * \param type
 * \return
 */
const QString &getFeatureTypeIconPath(const FeatureTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding feature type icon
    if(internal::featureTypeIcons.contains(type)){
        return internal::featureTypeIcons[type];
    }
    return internal::undefined;

}

/*!
 * \brief getReadingTypeName
 * \param type
 * \return
 */
const QString &getReadingTypeName(const ReadingTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading name
    if(internal::readingTypesMap.contains(type)){
        return internal::readingTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getReadingTypeEnum
 * \param name
 * \return
 */
ReadingTypes getReadingTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading enum value
    return internal::readingTypesMap.key(name, eUndefinedReading);

}

/*!
 * \brief getFeatureDisplayAttributes
 * Returns a list of enum values (feature attributes and trafo param attributes)
 * \return
 */
const QList<int> &getFeatureDisplayAttributes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::featureDisplayAttributes;

}

/*!
 * \brief getObservationDisplayAttributes
 * \return
 */
const QList<ObservationDisplayattributes> &getObservationDisplayAttributes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::observationDisplayAttributes;

}

/*!
 * \brief getReadingDisplayAttributes
 * \return
 */
const QList<ReadingDisplayAttributes> &getReadingDisplayAttributes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::readingDisplayAttributes;

}

/*!
 * \brief getIsFeatureDisplayAttribute
 * Returns true if the given attribute is a feature attribute
 * \param attr
 * \return
 */
bool getIsFeatureDisplayAttribute(const int &attr){
    if( (attr >= 0 && attr < 10) || (attr >= 100 && attr < 102) || (attr >= 200 && attr < 220) || (attr >= 300 && attr < 303) ){
        return true;
    }
    return false;
}

/*!
 * \brief getIsTrafoParamDisplayAttribute
 * Returns true if the given attribute is a trafo param attribute
 * \param attr
 * \return
 */
bool getIsTrafoParamDisplayAttribute(const int &attr){
    if( (attr >= 0 && attr < 10) || (attr >= 400 && attr < 414) ){
        return true;
    }
    return false;
}

/*!
 * \brief getDisplayAttributeName
 * Returns the name of the attribute represented by the given enum value
 * \param attr
 * \return
 */
const QString &getFeatureDisplayAttributeName(const int &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    if(getIsFeatureDisplayAttribute(attr)){ //feature display attribute
        return internal::featureDisplayAttributesMap[(FeatureDisplayAttributes)attr];
    }else if(getIsTrafoParamDisplayAttribute(attr)){ //trafo param display attribute
        return internal::trafoParamDisplayAttributesMap[(TrafoParamDisplayAttributes)attr];
    }

    return internal::undefined;

}

/*!
 * \brief getDisplayAttributeName
 * \param attr
 * \return
 */
const QString &getFeatureDisplayAttributeName(const FeatureDisplayAttributes &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::featureDisplayAttributesMap[attr];

}

/*!
 * \brief getDisplayAttributeName
 * \param attr
 * \return
 */
const QString &getFeatureDisplayAttributeName(const TrafoParamDisplayAttributes &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::trafoParamDisplayAttributesMap[attr];

}

/*!
 * \brief getReadingDisplayAttributeVisibility
 * \param attr
 * \param type
 * \return
 */
bool getReadingDisplayAttributeVisibility(const ReadingDisplayAttributes &attr, const ReadingTypes &type){

    switch(attr){
    case eReadingDisplayAzimuth:
        if(type != ePolarReading && type != eDirectionReading){
            return false;
        }
        break;
    case eReadingDisplayZenith:
        if(type != ePolarReading && type != eDirectionReading){
            return false;
        }
        break;
    case eReadingDisplayDistance:
        if(type != ePolarReading && type != eDistanceReading){
            return false;
        }
        break;
    case eReadingDisplayX:
        if(type != eCartesianReading){
            return false;
        }
        break;
    case eReadingDisplayY:
        if(type != eCartesianReading){
            return false;
        }
        break;
    case eReadingDisplayZ:
        if(type != eCartesianReading){
            return false;
        }
        break;
    case eReadingDisplayRX:
        if(type != eLevelReading){
            return false;
        }
        break;
    case eReadingDisplayRY:
        if(type != eLevelReading){
            return false;
        }
        break;
    case eReadingDisplayRZ:
        if(type != eLevelReading){
            return false;
        }
        break;
    case eReadingDisplayTemperature:
        if(type != eTemperatureReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaAzimuth:
        if(type != ePolarReading && type != eDirectionReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaZenith:
        if(type != ePolarReading && type != eDirectionReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaDistance:
        if(type != ePolarReading && type != eDistanceReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaX:
        if(type != eCartesianReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaY:
        if(type != eCartesianReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaZ:
        if(type != eCartesianReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaRX:
        if(type != eLevelReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaRY:
        if(type != eLevelReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaRZ:
        if(type != eLevelReading){
            return false;
        }
        break;
    case eReadingDisplaySigmaTemperature:
        if(type != eTemperatureReading){
            return false;
        }
        break;
    }

    return true;

}

/*!
 * \brief getObservationDisplayAttributesName
 * \param attr
 * \return
 */
const QString &getObservationDisplayAttributesName(const ObservationDisplayattributes &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::observationDisplayAttributesMap[attr];

}

/*!
 * \brief getReadingDisplayAttributeName
 * \param attr
 * \return
 */
const QString &getReadingDisplayAttributeName(const ReadingDisplayAttributes &attr){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::readingDisplayAttributesMap[attr];

}

/*!
 * \brief convertToDefault
 * Convert the given value of unit type to the default unit
 * \param value
 * \param type
 * \return
 */
double convertToDefault(const double &value, const UnitType &type){

    switch(type){
    case eUnitMilliMeter:
        return value * (1.0 / 1000.0);
    case eUnitInch:
        return value * (1.0 / 39.37007874);
    case eUnitMilliRadians:
        return value * (1.0 / 1000.0);
    case eUnitGon:
        return value * (1.0 / RHO_GON);
    case eUnitArcSeconds:
        return value * (1.0 / 206264.8062471);
    case eUnitDecimalDegree:
        return value * (1.0 / RHO_DEGREE);
    case eUnitFahrenheit:
        return (value - 32.0) / 1.8;
    case eUnitKelvin:
        return value - 273.15;
    default:
        return value;
    }

}

/*!
 * \brief convertFromDefault
 * Convert the given value of default unit to the specified unit type
 * \param value
 * \param type
 * \return
 */
double convertFromDefault(const double &value, const UnitType &type){

    switch(type){
    case eUnitMilliMeter:
        return value * 1000.0;
    case eUnitInch:
        return value * 39.37007874;
    case eUnitMilliRadians:
        return value * 1000.0;
    case eUnitGon:
        return value * RHO_GON;
    case eUnitArcSeconds:
        return value * 206264.8062471;
    case eUnitDecimalDegree:
        return value * RHO_DEGREE;
    case eUnitFahrenheit:
        return value * 1.8 + 32.0;
    case eUnitKelvin:
        return value + 273.15;
    default:
        return value;
    }

}

/*!
 * \brief getUnitTypeName
 * \param type
 * \return
 */
const QString &getUnitTypeName(const UnitType &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading name
    if(internal::unitTypesMap.contains(type)){
        return internal::unitTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getUnitTypeEnum
 * \param name
 * \return
 */
UnitType getUnitTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading enum value
    return internal::unitTypesMap.key(name, eNoUnit);

}

/*!
 * \brief getAvailableSensorTypes
 * \return
 */
QList<SensorTypes> getAvailableSensorTypes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::sensorTypesMap.keys();

}

/*!
 * \brief getSensorTypeName
 * \param type
 * \return
 */
const QString &getSensorTypeName(const SensorTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding sensor type name
    if(internal::sensorTypesMap.contains(type)){
        return internal::sensorTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getSensorTypeEnum
 * \param name
 * \return
 */
SensorTypes getSensorTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding reading enum value
    return internal::sensorTypesMap.key(name, eUndefinedSensor);

}

/*!
 * \brief getAvailableSensorFunctions
 * \return
 */
QList<SensorFunctions> getAvailableSensorFunctions(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::sensorFunctionsMap.keys();

}

/*!
 * \brief getSensorFunctionName
 * \param type
 * \return
 */
const QString &getSensorFunctionName(const SensorFunctions &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding sensor function name
    if(internal::sensorFunctionsMap.contains(type)){
        return internal::sensorFunctionsMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getSensorFunctionEnum
 * \param name
 * \return
 */
SensorFunctions getSensorFunctionEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding sensor function enum value
    return internal::sensorFunctionsMap.key(name, eInitialize);

}

/*!
 * \brief getAvailableConnectionTypes
 * \return
 */
QList<ConnectionTypes> getAvailableConnectionTypes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::connectionTypesMap.keys();

}

/*!
 * \brief getConnectionTypeName
 * \param type
 * \return
 */
const QString &getConnectionTypeName(const ConnectionTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding connection type name
    if(internal::connectionTypesMap.contains(type)){
        return internal::connectionTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getConnectionTypeEnum
 * \param name
 * \return
 */
ConnectionTypes getConnectionTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding connection type enum value
    return internal::connectionTypesMap.key(name, eNetworkConnection);

}

/*!
 * \brief getAvailableToolTypes
 * \return
 */
QList<ToolTypes> getAvailableToolTypes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::toolTypesMap.keys();

}

/*!
 * \brief getToolTypeName
 * \param type
 * \return
 */
const QString &getToolTypeName(const ToolTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding tool type name
    if(internal::toolTypesMap.contains(type)){
        return internal::toolTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getToolTypeEnum
 * \param name
 * \return
 */
ToolTypes getToolTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding tool type enum value
    return internal::toolTypesMap.key(name, eUnknownTool);

}

/*!
 * \brief getAvailableMessageTypes
 * \return
 */
QList<MessageTypes> getAvailableMessageTypes(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::messageTypesMap.keys();

}

/*!
 * \brief getMessageTypeName
 * \param type
 * \return
 */
const QString &getMessageTypeName(const MessageTypes &type){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding message type name
    if(internal::messageTypesMap.contains(type)){
        return internal::messageTypesMap[type];
    }
    return internal::undefined;

}

/*!
 * \brief getMessageTypeEnum
 * \param name
 * \return
 */
MessageTypes getMessageTypeEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding message type enum value
    return internal::messageTypesMap.key(name, eInformationMessage);

}

/*!
 * \brief getAvailableUnknownParameters
 * \return
 */
QList<UnknownParameters> getAvailableUnknownParameters(){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    return internal::unknownParametersMap.keys();

}

/*!
 * \brief getUnknownParameterName
 * \param parameter
 * \return
 */
const QString &getUnknownParameterName(const UnknownParameters &parameter){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding unknown parameter name
    if(internal::unknownParametersMap.contains(parameter)){
        return internal::unknownParametersMap[parameter];
    }
    return internal::undefined;

}

/*!
 * \brief getUnknownParameterEnum
 * \param name
 * \return
 */
UnknownParameters getUnknownParameterEnum(const QString &name){

    //fill helper maps if not yet done
    if(!internal::isInit){
        internal::init();
    }

    //get the corresponding unknown parameter enum value
    return internal::unknownParametersMap.key(name, eUnknownX);

}

/*!
 * \brief getUnknownParameters
 * \param type
 * \return
 */
QList<UnknownParameters> getUnknownParameters(const GeometryTypes &type){

    QList<UnknownParameters> unknownParameters;

    switch(type){
    case eCircleGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownRadiusA);
        break;
    case eConeGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownAperture);
        break;
    case eCylinderGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownRadiusA);
        break;
    case eEllipseGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownA);
        unknownParameters.append(eUnknownB);
        unknownParameters.append(eUnknownSecondaryI);
        unknownParameters.append(eUnknownSecondaryJ);
        unknownParameters.append(eUnknownSecondaryK);
        break;
    case eEllipsoidGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownA);
        unknownParameters.append(eUnknownB);
        break;
    case eHyperboloidGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownA);
        unknownParameters.append(eUnknownC);
        break;
    case eLineGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        break;
    case eNurbsGeometry:
        break;
    case eParaboloidGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownA);
        break;
    case ePlaneGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        break;
    case ePointGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        break;
    case ePointCloudGeometry:
        break;
    case eScalarEntityAngleGeometry:
        unknownParameters.append(eUnknownAngle);
        break;
    case eScalarEntityDistanceGeometry:
        unknownParameters.append(eUnknownDistance);
        break;
    case eScalarEntityMeasurementSeriesGeometry:
        unknownParameters.append(eUnknownMeasurementSeries);
        break;
    case eScalarEntityTemperatureGeometry:
        unknownParameters.append(eUnknownTemperature);
        break;
    case eSlottedHoleGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownRadiusA);
        unknownParameters.append(eUnknownLength);
        unknownParameters.append(eUnknownSecondaryI);
        unknownParameters.append(eUnknownSecondaryJ);
        unknownParameters.append(eUnknownSecondaryK);
        break;
    case eSphereGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownRadiusA);
        break;
    case eTorusGeometry:
        unknownParameters.append(eUnknownX);
        unknownParameters.append(eUnknownY);
        unknownParameters.append(eUnknownZ);
        unknownParameters.append(eUnknownPrimaryI);
        unknownParameters.append(eUnknownPrimaryJ);
        unknownParameters.append(eUnknownPrimaryK);
        unknownParameters.append(eUnknownRadiusA);
        unknownParameters.append(eUnknownRadiusB);
        break;
    }

    return unknownParameters;

}

/*!
 * \brief getUnknownParameters
 * \param feature
 * \param displayUnits
 * \param displayDigits
 * \return
 */
QMap<UnknownParameters, QString> getUnknownParameters(const QPointer<FeatureWrapper> &feature, const QMap<DimensionType, UnitType> &displayUnits, const QMap<DimensionType, int> &displayDigits){

    QMap<UnknownParameters, QString> parameters;

    //check geometry
    if(feature.isNull() || feature->getGeometry().isNull()){
        return parameters;
    }

    //get and check type of geometry
    GeometryTypes geometryType = getGeometryTypeEnum(feature->getFeatureTypeEnum());
    if(geometryType == eUndefinedGeometry){
        return parameters;
    }

    switch(geometryType){
    case eCircleGeometry:
        if(feature->getCircle().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getCircle()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getCircle()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getCircle()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getCircle()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getCircle()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getCircle()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownRadiusA, feature->getCircle()->getDisplayRadiusA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case eConeGeometry:
        if(feature->getCone().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getCone()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getCone()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getCone()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getCone()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getCone()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getCone()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownAperture, feature->getCone()->getDisplayAperture(displayUnits.value(eAngular), displayDigits.value(eMetric)));
        break;
    case eCylinderGeometry:
        if(feature->getCylinder().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getCylinder()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getCylinder()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getCylinder()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getCylinder()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getCylinder()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getCylinder()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownRadiusA, feature->getCylinder()->getDisplayRadiusA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case eEllipseGeometry:
        if(feature->getEllipse().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getEllipse()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getEllipse()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getEllipse()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getEllipse()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getEllipse()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getEllipse()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownA, feature->getEllipse()->getDisplayA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownB, feature->getEllipse()->getDisplayB(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownSecondaryI, feature->getEllipse()->getDisplaySecondaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownSecondaryJ, feature->getEllipse()->getDisplaySecondaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownSecondaryK, feature->getEllipse()->getDisplaySecondaryK(displayDigits.value(eMetric)));
        break;
    case eEllipsoidGeometry:
        if(feature->getEllipsoid().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getEllipsoid()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getEllipsoid()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getEllipsoid()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getEllipsoid()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getEllipsoid()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getEllipsoid()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownA, feature->getEllipsoid()->getDisplayA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownB, feature->getEllipsoid()->getDisplayB(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case eHyperboloidGeometry:
        if(feature->getHyperboloid().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getHyperboloid()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getHyperboloid()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getHyperboloid()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getHyperboloid()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getHyperboloid()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getHyperboloid()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownA, feature->getHyperboloid()->getDisplayA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownC, feature->getHyperboloid()->getDisplayC(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case eLineGeometry:
        if(feature->getLine().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getLine()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getLine()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getLine()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getLine()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getLine()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getLine()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        break;
    case eNurbsGeometry:
        if(feature->getNurbs().isNull()){
            return parameters;
        }
        break;
    case eParaboloidGeometry:
        if(feature->getParaboloid().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getParaboloid()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getParaboloid()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getParaboloid()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getParaboloid()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getParaboloid()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getParaboloid()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownA, feature->getParaboloid()->getDisplayA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case ePlaneGeometry:
        if(feature->getPlane().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getPlane()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getPlane()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getPlane()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getPlane()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getPlane()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getPlane()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        break;
    case ePointGeometry:
        if(feature->getPoint().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getPoint()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getPoint()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getPoint()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case ePointCloudGeometry:
        if(feature->getPointCloud().isNull()){
            return parameters;
        }
        break;
    case eScalarEntityAngleGeometry:
        if(feature->getScalarEntityAngle().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownAngle, feature->getScalarEntityAngle()->getDisplayAngle(displayUnits.value(eAngular), displayDigits.value(eMetric)));
        break;
    case eScalarEntityDistanceGeometry:
        if(feature->getScalarEntityDistance().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownDistance, feature->getScalarEntityDistance()->getDisplayDistance(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case eScalarEntityMeasurementSeriesGeometry:
        if(feature->getScalarEntityMeasurementSeries().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownMeasurementSeries, feature->getScalarEntityMeasurementSeries()->getDisplayMeasurementSeries(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case eScalarEntityTemperatureGeometry:
        if(feature->getScalarEntityTemperature().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownTemperature, feature->getScalarEntityTemperature()->getDisplayTemperature(displayUnits.value(eTemperature), displayDigits.value(eMetric)));
        break;
    case eSlottedHoleGeometry:
        if(feature->getSlottedHole().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getSlottedHole()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getSlottedHole()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getSlottedHole()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getSlottedHole()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getSlottedHole()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getSlottedHole()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownRadiusA, feature->getSlottedHole()->getDisplayRadiusA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownLength, feature->getSlottedHole()->getDisplayLength(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownSecondaryI, feature->getSlottedHole()->getDisplaySecondaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownSecondaryJ, feature->getSlottedHole()->getDisplaySecondaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownSecondaryK, feature->getSlottedHole()->getDisplaySecondaryK(displayDigits.value(eMetric)));
        break;
    case eSphereGeometry:
        if(feature->getSphere().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getSphere()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getSphere()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getSphere()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownRadiusA, feature->getSphere()->getDisplayRadiusA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    case eTorusGeometry:
        if(feature->getTorus().isNull()){
            return parameters;
        }
        parameters.insert(eUnknownX, feature->getTorus()->getDisplayX(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownY, feature->getTorus()->getDisplayY(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownZ, feature->getTorus()->getDisplayZ(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryI, feature->getTorus()->getDisplayPrimaryI(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryJ, feature->getTorus()->getDisplayPrimaryJ(displayDigits.value(eMetric)));
        parameters.insert(eUnknownPrimaryK, feature->getTorus()->getDisplayPrimaryK(displayDigits.value(eMetric)));
        parameters.insert(eUnknownRadiusA, feature->getTorus()->getDisplayRadiusA(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        parameters.insert(eUnknownRadiusB, feature->getTorus()->getDisplayRadiusB(displayUnits.value(eMetric), displayDigits.value(eMetric)));
        break;
    }

    return parameters;

}

}
