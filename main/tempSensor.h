#ifndef TEMPSENSOR_C
#define TEMPSENSOR_C

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h" 

#define I2C_MASTER_SCL_IO           19      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           18      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

static esp_adc_cal_characteristics_t adc1_chars;
// const char* ADCTAG = "ADC";

#define HDC2080_ADDRESS_1  0x40
#define HDC2080_ADDRESS_2  0x41

#define TEMP_LOW                0x00 // Temperature Low
#define TEMP_HIGH               0x01 // Temperature High
#define HUMIDITY_LOW            0x02 // Humidity Low
#define HUMIDITY_HIGH           0x03 // Humidity High
#define INTERRUPT_DRDY          0x04 // Interrupt/DRDY
#define TEMP_MAX                0x05 // Maximum measured temperature  (Not supported in Auto Measurement Mode)
#define HUMIDITY_MAX            0x06 // Maximum measured humidity (Not supported in Auto Measurement Mode)
#define INTERRUPT_ENABLE        0x07 // Interrupt Enable
#define TEMP_OFFSET_ADJUST      0x08 // Temperature offset adjustment
#define HUM_OFFSET_ADJUST       0x09 // Humidity offset adjustment
#define TEMP_THR_L              0x0A // Temperature Threshold Low
#define TEMP_THR_H              0x0B // Temperature Threshold High
#define RH_THR_L                0x0C // Humidity threshold Low
#define RH_THR_H                0x0D // Humidity threshold High
#define RESET_DRDY_INT_CONF     0x0E // Reset and Interrupt Configuration
#define MEASUREMENT_CONF        0x0F // Measurement Configuration
#define MANUFACTURER_ID_LOW     0xFC // Manufacturer ID Low
#define MANUFACTURER_ID_HIGH    0xFD // Manufacturer ID High
#define DEVICE_ID_LOW           0xFE // Device ID Low
#define DEVICE_ID_HIGH          0xFF // Device ID High

// sensor modes
#define TEMP_AND_HUMID 0
#define TEMP_ONLY 1
#define HUMID_ONLY 2
#define ACTIVE_LOW 0
#define ACTIVE_HIGH 1
#define LEVEL_MODE 0
#define COMPARATOR_MODE 1

float temp;
esp_err_t initI2c();
bool startSensor(uint8_t addr);
float readTemprature();
void setSensorMode(int mode) ;
void resetSensor();
void temperature_task_init(void);
void temeprature_task(void* param);


#endif