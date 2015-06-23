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
        eGetProject,
        eSetProject,
        eGetActiveFeature,
        eSetActiveFeature,
        eGetActiveStation,
        eSetActiveStation,
        eGetActiveCoordinateSystem,
        eSetActiveCoordinateSystem,
        eAim,
        eMove,
        eMeasure,
        eStartWatchwindow,
        eStopWatchwindow,
        eOiToolRequest
    };

    enum ErrorCode{
        eNoError,
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
        eFeatureNotSolved
    };

    RequestType myRequestType; //defines the type of request
    QDomDocument request; //holds the XML structure of the request
    QDomDocument response; //holds the XML structure of the response
    int requesterId; //identifies the requester so that the response is send only to him

};

}

#endif // OIREQUESTRESPONSE_H
