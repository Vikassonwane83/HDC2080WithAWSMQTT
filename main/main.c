#include <stdio.h>
#include "fileSystem.h"
#include "wifiConfig.h"
#include "awsMQTT.h"
#include "tempSensor.h"
#include "tempSensor.h"

void app_main(void)
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  test();

  fatFsInit();

  WifiBegin();

  initI2c();

  startAwsMqtt();
  
  temperature_task_init();
}
