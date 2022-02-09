#ifndef SENSORWORKERMESSAGE_H
#define SENSORWORKERMESSAGE_H
#include <QString>

struct SensorWorkerMessage {
    static const QString NO_SENSOR_INSTANCE;
    static const QString CONNECTION_WAS_BLOCKED;
    static const QString MEASUREMENT_DATA_RECEIVED;
    static const QString MEASUREMENT_DIT_NOT_DELIVER_ANY_RESULTS;

    static const QString FAILED_TO_MEASURE; // also used as suffix

    static const QString MEASUREMENT_FINISHED;  // also used as suffix
    static const QString SENSOR_IS_NOT_CONNECTED;
    static const QString SENSOR_DISCONNECTED;
    static const QString SENSOR_CONNECTED;
    static const QString SENSOR_IS_ALREADY_CONNECTED;
    static const QString FAILED_TO_CONNECT_SENSOR;
    static const QString FAILED_TO_DISCONNECT_SENSOR;
    static const QString FAILED_TO_MOVE_SENSOR;

    static const QString MOVING_SENSOR_FINISHED_PREFIX; // prefix
    static const QString MOVING_SENSOR_FINISHED_MEASUREMENT_FINISHED;
    static const QString MOVING_SENSOR_FINISHED_FAILED_TO_MEASURE;

    static const QString FAILED_TO_INITIALIZE_SENSOR;
    static const QString INITIALIZING_FINISHED;

    static const QString FAILED_TO_CHANGE_MOTOR_STATE;
    static const QString CHANGING_MOTOR_STATE_FINISHED;

    static const QString FAILED_TO_SET_SENSOR_TO_HOME_POSITION;
    static const QString SETTING_HOME_FINISHED;

    static const QString FAILED_TO_TOGGLE_SIGHT_ORIENTATION;
    static const QString TOGGLE_SIGHT_ORIENTATION_FINISHED;

    static const QString FAILED_TO_START_COMPENSATION;
    static const QString STARTING_COMPENSATION_FINISHED;

    static const QString FAILED_TO_DO_SELF_DEFINED_ACTION;
    static const QString SELF_DEFINED_ACTION_FINISHED;
};
#endif // SENSORWORKERMESSAGE_H
