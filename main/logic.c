#include "logic.h"
#include "cJSON.h"
#include "awsMQTT.h"
#include "tempSensor.h"
#include "fileSystem.h"

void sentTempDataMqtt(){
    float temprature;
    temprature = readTemprature();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature",(int)temprature);
    char *json_str = cJSON_Print(root);
    sendMQTT(json_str);

    fileWrite("/spiflash/temp.txt",json_str);
    char* pswd = fileRead("/spiflash/temp.txt");
    ESP_LOGI("SENT ","The Temprature data [ %f ] is sent on mqtt ", temprature);
    cJSON_Delete(root);

}