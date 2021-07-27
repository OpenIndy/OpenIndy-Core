#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QWidget>
#include <QCloseEvent>

#include "types.h"
#include "oirequestresponse.h"
#include "pluginmetadata.h"
#include "oijob.h"

namespace oi{

class OI_CORE_EXPORT Tool : public QWidget
{
    Q_OBJECT

public:
    explicit Tool(QWidget *parent = 0);

    virtual ~Tool();

    //##########################
    //tool initialization method
    //##########################

    virtual void init();

    virtual bool saveProjectEnabled();

    //#########################################
    //methods to get or set further information
    //#########################################

    const PluginMetaData &getMetaData() const;

    const ToolTypes &getToolType() const;

    const QPointer<OiJob> &getJob() const;
    void setJob(const QPointer<OiJob> &job);

    virtual void toOpenIndyXML(QDomDocument &project);
    virtual void fromOpenIndyXML(QDomDocument project);

    //#########################
    //override QWidget methods
    //#########################

    void closeEvent(QCloseEvent *event);

public slots:

    //#####################################
    //special events a tool could listen on
    //#####################################

    virtual void customXmlRequest(const QPointer<OiRequestResponse> &request);
    virtual void watchWindowKeyPressed(const Qt::Key &key);

    // receive emitted job
    virtual void setCurrentJob(const QPointer<OiJob> &job);

signals:

    //############################################
    //signals to inform OpenIndy about tool issues
    //############################################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);
    void customXmlResponse(const QPointer<OiRequestResponse> &response);

    //###############################
    //signals to start sensor actions
    //###############################

    void startConnect();
    void startDisconnect();
    void startMeasurement();
    void startMove(const Reading &reading);
    void startAim();
    void startAimAndMeasure();
    void startToggleSight();
    void startInitialize();
    void startHome();
    void startCompensation();
    void startChangeMotorState();
    void startCustomAction(const QString &task);

    void addFeatures(const FeatureAttributes &attributes);

protected:

    //###########################
    //input and output parameters
    //###########################

    PluginMetaData metaData;

    ToolTypes toolType;

    QPointer<OiJob> job; //the OpenIndy job that this tool works on

};

}

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

#define TOOL_PREFIX de.openIndy.plugin.tool.v
#define Tool_iidd STR2(CONCAT2(TOOL_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // TOOL_H
