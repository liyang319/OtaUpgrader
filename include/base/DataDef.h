#ifndef DATADEF_H
#define DATADEF_H

#include "Base.h"
#include <map>

// #define DEFAULT_VERSION_PATH "../app/VERSION"
#define DEFAULT_VERSION_PATH "/opt/version"
// #define DEVICE_SN "4854604D7765A027"
#define APP_NAME "WeighBox"
#define CONFIG_NAME "sensor.ini"
#define UPDATER_NAME "OtaUpgrader"
#define RESTORE_SCRIPT_NAME "restore.sh"
#define UPDATE_REPLACE_SCRIPT_NAME "update_replace.sh"
#define APP_BASE_PATH "/home/ubuntu/workdir/app/"
#define DEFAULT_OTA_SAVE_PATH "/home/ubuntu/workdir/app/ota_save/"
#define DEFAULT_OTA_BACKUP_PATH "/home/ubuntu/workdir/app/ota_backup/"
#define DEFAULT_APP_PATH "/home/ubuntu/workdir/app/WeighBox"
#define DEFAULT_APP_RIGHTS "777"
#define DEFAULT_LOGFILE_PREFIX "weighbox_"
// #define USE_DOCKER

#ifdef USE_DOCKER
#define URL_CHECK_OTA "http://218.94.69.218:8901/otacheck"
#define URL_UPLOAD_LOG "http://218.94.69.218:8901/upload"
#define URL_CHECK_LOG "http://218.94.69.218:8901/logcheck"
#else
#define URL_CHECK_OTA "http://192.168.80.178:8901/otacheck"
#define URL_UPLOAD_LOG "http://192.168.80.178:8901/upload"
#define URL_CHECK_LOG "http://192.168.80.178:8901/logcheck"
#endif

#define DEFAULT_SN_FILE_PATH "/opt/sn"
#define LOGVAL_NEED_UPLOAD "upload"
#define MAX_LAUNCH_TRY_TIME 3
#define DEFAULT_LOG_PATH "/home/ubuntu/workdir/app"

#define MQTT_PUBLISH_TOPIC_INDEX_DATA 1
#define MQTT_PUBLISH_TOPIC_INDEX_RESPONSE 2
#define MQTT_PUBLISH_TOPIC_INDEX_OFFLINEDATA 3

#define MSG_CMD_OTA "OTA"
#define MSG_CMD_RESTART "RESTART"

typedef struct _MqttPublishUnit
{
    int topicIndex;
    std::string content;
} MqttPublishUnit;

#endif // DATADEF_H