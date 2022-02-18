#include "sensorworkermessage.h"

const QString SensorWorkerMessage::NO_SENSOR_INSTANCE = "no sensor instance";
const QString SensorWorkerMessage::CONNECTION_WAS_BLOCKED = "connection was blocked - please try again";
const QString SensorWorkerMessage::MEASUREMENT_DATA_RECEIVED = "measurement data received";
const QString SensorWorkerMessage::MEASUREMENT_DIT_NOT_DELIVER_ANY_RESULTS = "measurement did not deliver any results";

const QString SensorWorkerMessage::FAILED_TO_MEASURE = "failed to measure"; // also used as suffix

const QString SensorWorkerMessage::MEASUREMENT_FINISHED = "measurement finished";
const QString SensorWorkerMessage::SENSOR_IS_NOT_CONNECTED = "sensor is not connected";
const QString SensorWorkerMessage::SENSOR_DISCONNECTED = "sensor disconnected";
const QString SensorWorkerMessage::SENSOR_CONNECTED = "sensor connected";
const QString SensorWorkerMessage::SENSOR_IS_ALREADY_CONNECTED = "sensor is already connected";
const QString SensorWorkerMessage::FAILED_TO_CONNECT_SENSOR = "failed to connect sensor";
const QString SensorWorkerMessage::FAILED_TO_DISCONNECT_SENSOR = "failed to disconnect sensor";
const QString SensorWorkerMessage::FAILED_TO_MOVE_SENSOR = "failed to move sensor";

const QString SensorWorkerMessage::MOVING_SENSOR_FINISHED_PREFIX = "moving sensor finished";
const QString SensorWorkerMessage::MOVING_SENSOR_FINISHED_MEASUREMENT_FINISHED = SensorWorkerMessage::MOVING_SENSOR_FINISHED_PREFIX + ", " + SensorWorkerMessage::MEASUREMENT_FINISHED;
const QString SensorWorkerMessage::MOVING_SENSOR_FINISHED_FAILED_TO_MEASURE = SensorWorkerMessage::MOVING_SENSOR_FINISHED_PREFIX + ", " + SensorWorkerMessage::FAILED_TO_MEASURE;

const QString SensorWorkerMessage::FAILED_TO_INITIALIZE_SENSOR = "failed to initialize sensor";
const QString SensorWorkerMessage::INITIALIZING_FINISHED = "initializing finished";

const QString SensorWorkerMessage::FAILED_TO_CHANGE_MOTOR_STATE = "failed to change motor state";
const QString SensorWorkerMessage::CHANGING_MOTOR_STATE_FINISHED = "changing motor state finished";

const QString SensorWorkerMessage::FAILED_TO_SET_SENSOR_TO_HOME_POSITION = "failed to set sensor to home position";
const QString SensorWorkerMessage::SETTING_HOME_FINISHED = "setting home finished";

const QString SensorWorkerMessage::FAILED_TO_TOGGLE_SIGHT_ORIENTATION = "failed to toggle sight orientation";
const QString SensorWorkerMessage::TOGGLE_SIGHT_ORIENTATION_FINISHED = "toggle sight orientation finished";

const QString SensorWorkerMessage::FAILED_TO_START_COMPENSATION = "failed to start compensation";
const QString SensorWorkerMessage::STARTING_COMPENSATION_FINISHED = "starting compensation finished";

const QString SensorWorkerMessage::FAILED_TO_DO_SELF_DEFINED_ACTION = "failed to do self defined action";
const QString SensorWorkerMessage::SELF_DEFINED_ACTION_FINISHED = "self defined action finished";

const QString SensorWorkerMessage::SEARCH_FINISHED = "search finished";
const QString SensorWorkerMessage::FAILED_TO_SEARCH = "failed to search";

