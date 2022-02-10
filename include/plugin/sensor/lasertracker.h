#ifndef LASERTRACKER_H
#define LASERTRACKER_H

#include "sensor.h"

namespace oi{

/*!
 * \brief The LaserTracker class
 */
class OI_CORE_EXPORT LaserTracker : public Sensor
{
    Q_OBJECT

public:
    explicit LaserTracker(QObject *parent = 0) : Sensor(parent){}

    virtual ~LaserTracker(){}

    //##########################################
    //calls the appropriate laser tracker method
    //##########################################

    bool accept(const SensorFunctions &method, const SensorAttributes &sAttr){

        switch(method){
        case(eMoveAngle):
            return this->move(sAttr.moveAzimuth, sAttr.moveZenith, sAttr.moveDistance, sAttr.moveIsRelative);
        case(eMoveXYZ):
            return this->move(sAttr.moveX, sAttr.moveY, sAttr.moveZ);
        case(eInitialize):
            return this->initialize();
        case(eHome):
            return this->home();
        case(eToggleSight):
            return this->toggleSightOrientation();
        case(eMotorState):
            return this->changeMotorState();
        case(eCompensation):
            return this->compensation();
        case(eSearch):
            return this->search();
        }
        return false;

    }

protected:

    //#####################
    //laser tracker methods
    //#####################

    /*!
     * \brief initialize
     * \return
     */
    virtual bool initialize(){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: initialization method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }

    /*!
     * \brief move
     * \param azimuth
     * \param zenith
     * \param distance
     * \param isrelativ
     * \return
     */
    virtual bool move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: move method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }

    /*!
     * \brief move
     * \param x
     * \param y
     * \param z
     * \return
     */
    virtual bool move(const double &x, const double &y, const double &z){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: move method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }

    /*!
     * \brief home
     * \return
     */
    virtual bool home(){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: home method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }

    /*!
     * \brief changeMotorState
     * \return
     */
    virtual bool changeMotorState(){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: motor state method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }

    /*!
     * \brief toggleSightOrientation
     * \return
     */
    virtual bool toggleSightOrientation(){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: toggle sight method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }

    /*!
     * \brief compensation
     * \return
     */
    virtual bool compensation(){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: compensation method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }

    virtual bool search(){
        emit this->sensorMessage(QString("Laser Tracker not implemented correctly: search method missing"), eCriticalMessage, eMessageBoxMessage);
        return false;
    }
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

#define LASERTRACKER_PREFIX de.openIndy.plugin.sensor.laserTracker.v
#define LaserTracker_iidd STR2(CONCAT2(LASERTRACKER_PREFIX, PLUGIN_INTERFACE_VERSION))

#endif // LASERTRACKER_H
