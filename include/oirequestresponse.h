#ifndef OIREQUESTRESPONSE_H
#define OIREQUESTRESPONSE_H

#include <QString>
#include <QtXml>

#include "types.h"

namespace oi{

/*!
 * \brief The OiRequestResponse class
 * This class holds the XML request and response corresponding to a special request type
 */
class OiRequestResponse
{
public:

    enum RequestType{

        eGetProject = 0,
        eGetActiveFeature,
        eSetActiveFeature,
        eGetActiveStation,
        eSetActiveStation,
        eGetActiveCoordinateSystem,
        eSetActiveCoordinateSystem,
        eAim,
        eMeasure,
        eStartWatchwindow,
        eStopWatchwindow,
        eOiToolRequest,
        eGetFeatures,
        eAddFeatures,
        eGetObservations,
        eRemoveObservations,
        eGetParameters,
        eGetMeasurementConfigs,
        eGetMeasurementConfig,
        eSetMeasurementConfig,
        eGetCoordinateSystems,

        eUnknownRequest = 999,

        eSensorActionStarted = 1001,
        eSensorActionFinished,
        eMessageBox,
        eRealTimeReading,
        eActiveFeatureChanged,
        eActiveStationChanged,
        eActiveCoordinateSystemChanged,
        eFeatureSetChanged,
        eFeatureAttributesChanged

    };

    enum ErrorCode{

        eNoError = 0,
        eNoJob,
        eWrongFormat,
        eUnknownRequestType,
        eNoActiveFeature,
        eNoActiveStation,
        eNoActiveCoordinateSystem,
        eNoFeatureWithId,
        eNoTransformationParameters,
        eTaskInProcess,
        eNoTask,
        eNoSensor,
        eNoTaskWithId,
        eCannotMeasureNominal,
        eMeasurementError,
        eNoSensorConnected,
        eFeatureNotSolved,
        eCreateFeatureError,
        eNoMeasurementConfigManager,
        eNoSensorConfigManager,
        eNoMeasurementConfig

    };

    RequestType myRequestType; //defines the type of request
    QDomDocument request; //holds the XML structure of the request
    QDomDocument response; //holds the XML structure of the response
    int requesterId; //identifies the requester so that the response is send only to him

};

}

Q_DECLARE_METATYPE( oi::OiRequestResponse )
Q_DECLARE_METATYPE( oi::OiRequestResponse* )

#endif // OIREQUESTRESPONSE_H
