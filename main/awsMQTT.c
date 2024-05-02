#include "awsMQTT.h"
#include "string.h"
#include "logic.h"

static const char *TAG = "MQTT";
char* awsTopic = "ESP32/TX";

static IoT_Error_t rc = FAILURE;

static AWS_IoT_Client AWS_client;
static IoT_Client_Init_Params mqttInitParams  ;
static IoT_Client_Connect_Params connectParams ;

// extern const uint8_t aws_root_ca_pem_start[] asm("_binary_aws_root_ca_pem_start");
// extern const uint8_t aws_root_ca_pem_end[] asm("_binary_aws_root_ca_pem_end");
// extern const uint8_t certificate_pem_crt_start[] asm("_binary_certificate_pem_crt_start");
// extern const uint8_t certificate_pem_crt_end[] asm("_binary_certificate_pem_crt_end");
// extern const uint8_t private_pem_key_start[] asm("_binary_private_pem_key_start");
// extern const uint8_t private_pem_key_end[] asm("_binary_private_pem_key_end");

void iot_subscribe_callback_handler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData) {
    ESP_LOGI(TAG, "Subscribe callback");
    ESP_LOGI(TAG, "%.*s\t%.*s", topicNameLen, topicName, (int) params->payloadLen, (char *)params->payload);
    char* msg = (char *)params->payload;
}
void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data) {
    ESP_LOGW(TAG, "MQTT Disconnect");
    IoT_Error_t rc = FAILURE;
    if(NULL == pClient) {
        return;
    }

    if(aws_iot_is_autoreconnect_enabled(pClient)) {
        ESP_LOGI(TAG, "Auto Reconnect is enabled, Reconnecting attempt will start now");
    } else {
        ESP_LOGW(TAG, "Auto Reconnect not enabled. Starting manual reconnect...");
        rc = aws_iot_mqtt_attempt_reconnect(pClient);
        if(NETWORK_RECONNECTED == rc) {
            ESP_LOGW(TAG, "Manual Reconnect Successful");
        } else {
            ESP_LOGW(TAG, "Manual Reconnect Failed - %d", rc);
        }
    }
}

void awsIotInit(){
    ESP_LOGI("MQTT","awsIotInit -- >");

    mqttInitParams.enableAutoReconnect = true;
    mqttInitParams.pHostURL = HOSTURL;
    mqttInitParams.port = PORT;

    // mqttInitParams.pRootCALocation = (const char *)aws_root_ca_pem_start;
    // mqttInitParams.pDeviceCertLocation = (const char *)certificate_pem_crt_start;
    // mqttInitParams.pDevicePrivateKeyLocation = (const char *)private_pem_key_start;

    mqttInitParams.mqttCommandTimeout_ms = 80000;
    mqttInitParams.tlsHandshakeTimeout_ms = 60000;
    mqttInitParams.isSSLHostnameVerify = true;
    mqttInitParams.disconnectHandler = disconnectCallbackHandler;
    mqttInitParams.disconnectHandlerData = NULL;

    rc = aws_iot_mqtt_init(&AWS_client, &mqttInitParams);

    if(SUCCESS != rc){
        ESP_LOGE(TAG, "aws_iot_mqtt_init returned error : %d ", rc);
        abort();
    }
    connectParams.keepAliveIntervalInSec = 100;
    connectParams.isCleanSession = true;
    connectParams.pClientID = "ESP32";
    connectParams.clientIDLen = (uint16_t) strlen("ESP32");
    ESP_LOGI(TAG, "Connecting to AWS...");
     do {
        rc = aws_iot_mqtt_connect(&AWS_client, &connectParams);
        if(SUCCESS != rc) {
            ESP_LOGE(TAG, "Error(%d) connecting to %s:%d", rc, mqttInitParams.pHostURL, mqttInitParams.port);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    } while(SUCCESS != rc);

    rc = aws_iot_mqtt_autoreconnect_set_status(&AWS_client, true);
    if(SUCCESS != rc) {
        ESP_LOGE(TAG, "Unable to set Auto Reconnect to true - %d", rc);
        abort();
    }


    ESP_LOGI(TAG, "Subscribing...");
    rc = aws_iot_mqtt_subscribe(&AWS_client, awsTopic, strlen(awsTopic), QOS1, iot_subscribe_callback_handler, NULL);
    if(SUCCESS != rc) {
        ESP_LOGE(TAG, "Error subscribing : %d ", rc);
        abort();
    }ESP_LOGI(TAG, "Subscribed to ESP32/TX");
    aws_iot_mqtt_yield(&AWS_client, 100);

}

 void sendMQTT(const char* msg) {
    IoT_Publish_Message_Params paramsQOS1;
    paramsQOS1.qos = QOS1;
    paramsQOS1.payload = (void *) msg;
    paramsQOS1.payloadLen = strlen(msg);
    paramsQOS1.isRetained = 0;

    IoT_Error_t rc = FAILURE;
    if(aws_iot_mqtt_is_client_connected(&AWS_client) == true){
        rc = aws_iot_mqtt_publish(&AWS_client, awsTopic, strlen(awsTopic), &paramsQOS1);
        ESP_LOGI(TAG,"value of rc :%d",rc);
        if (rc == MQTT_REQUEST_TIMEOUT_ERROR) {
            ESP_LOGW(TAG, "QOS1 publish ack not received.");
            rc = SUCCESS;
        }else{
            ESP_LOGI(TAG," send on mqtt : %s ",(char *)paramsQOS1.payload);
        }
    }else{
        ESP_LOGW(TAG, "MQTT -> Disconnected skip publishing !");
    }
    
}

void MQTT_deinit(){
    aws_iot_mqtt_disconnect(&AWS_client);
    aws_iot_mqtt_free(&AWS_client);
    vTaskDelay(100/ portTICK_PERIOD_MS);
    
}

void startAwsMqtt()
{
    xTaskCreate(&mqtt_task,"mqtt task",2048*2,NULL,5,NULL);
}

void mqtt_task(void *params)
{
    awsIotInit();
    while (true)
    {
        sentTempDataMqtt();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

