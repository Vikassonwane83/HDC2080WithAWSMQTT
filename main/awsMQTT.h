#ifndef AWSMQTT_H
#define AWSMQTT_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"

#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"

#define HOSTURL "mqtt://www.xyz.in"
#define PORT 1883
#define USER "dfghttgg"
#define PASSWORDD "SDFGFH"

extern char* awsTopic;

void awsIotInit();
// esp_err_t initI2c();
extern void mqtt_task(void *params);

extern void sendMQTT(const char* msg);

void MQTT_deinit();

void startAwsMqtt();

#endif