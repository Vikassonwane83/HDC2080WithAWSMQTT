#include "tempSensor.h"
#include "esp_log.h"
#include "driver/i2c.h"
//  esp_err_t initI2c(void);
const char* ADCTAG = "ADC";
esp_err_t initI2c(void){
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .clk_flags = 0,
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port,conf.mode, I2C_MASTER_RX_BUF_DISABLE,I2C_MASTER_RX_BUF_DISABLE, 0 );
}

// static esp_err_t hdc2080RegisterRead(uint8_t regAddr, uint8_t *data, size_t len){
//     return i2c_master_write_read_device(I2C_MASTER_NUM,regAddr)
// }

bool startSensor(uint8_t addr) {
    if(addr == HDC2080_ADDRESS_1 || addr == HDC2080_ADDRESS_2 ){
       i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
        esp_err_t ret = i2c_master_stop(cmd);
        if (ret == ESP_OK) {
            return true; 
        }
    }
    return false;
}

float readTemprature(){
    uint8_t byte[2];
    uint16_t temp;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2080_ADDRESS_1 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, TEMP_LOW, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2080_ADDRESS_1 << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &byte[0], I2C_MASTER_ACK);
    i2c_master_read_byte(cmd, &byte[1], I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    temp = byte[1];
    temp = (temp << 8) | byte[0];
    float f = temp;
    f = ((f * 165.0f) / 65536.0f) - 40.0f;
    
    return f;
}
void setSensorMode(int mode) {
    uint8_t configContents;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2080_ADDRESS_1 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, MEASUREMENT_CONF, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2080_ADDRESS_1 << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &configContents, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    switch (mode) {
        case TEMP_AND_HUMID:
            configContents &= 0xF9;
            break;

        case TEMP_ONLY:
            configContents &= 0xFC;
            configContents |= 0x02;
            break;

        case HUMID_ONLY:
            configContents &= 0xFD;
            configContents |= 0x04;
            break;

        default:
            configContents &= 0xF9;
    }
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2080_ADDRESS_1 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, MEASUREMENT_CONF, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

void resetSensor(){
    uint8_t configContents;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2080_ADDRESS_1 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, CONFIGID0, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (HDC2080_ADDRESS_1 << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &configContents, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
}

void temperature_task_init(void)
{
  /* Create Temperature Task itself */
  if (xTaskCreate(temeprature_task, "temperature_task", 2048, NULL, 5, NULL) != pdPASS)
  {
    printf("[ERROR]\t temperature_handler: Fail to create temperature_task !");
  }
}

void temeprature_task(void* param){
    resetSensor();

    if(startSensor(HDC2080_ADDRESS_1))
    {
        setSensorMode(TEMP_ONLY);
        while (1)
        {
            temp = readTemprature();
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        } 
    }
}