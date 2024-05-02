#include "fileSystem.h"
#include "esp_vfs_fat.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"

#include "wear_levelling.h"


void fatFsInit(void){

    esp_err_t err;

    const esp_vfs_fat_mount_config_t mountCfg = {
        .max_files = 4,
        .format_if_mount_failed = true,
        .allocation_unit_size = 20
    };
    wl_handle_t wl_handle;
    err = esp_vfs_fat_spiflash_mount("/spiflash", "storage", &mountCfg, &wl_handle);
    if (err != ESP_OK)
    {
        ESP_LOGI("FATFS", "Error while mounting the spiflash mount");
    }
    else
    {
        ESP_LOGI("FATFS", "Sucess mounting the spiflash mount");
    }
}


bool fileWrite(const char * fName, const char* msg){
    // /spiflash/hello.txt
    FILE* f = fopen(fName, "w");
    if (f == NULL) {
        ESP_LOGI("FATFS","file writing fail");
        return false;
    }
    fprintf(f, "%s\n", msg);
    ESP_LOGI("FATFS","file written");
    fclose(f);
    return true;
}

char* fileRead(const char* fileName){
    FILE* f = fopen(fileName , "r");
    int count = 0 ;

    if(f == NULL){
        ESP_LOGI("FATFS","Failed to open file for reading");
        return "-1";
    }
    else{
        fgets(line, sizeof(line), f);
        fclose(f);
    }
    return line;
}

void fileRemove(const char* fileName) {
	   if (remove(fileName) == 0) {
      ESP_LOGI("FS","Deleted successfully"); 
	   }
   else{
      ESP_LOGI("FS","Unable to delete the file"); 
   }
}