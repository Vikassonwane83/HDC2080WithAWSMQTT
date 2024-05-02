#include "wifiConfig.h"
#include "fileSystem.h"

static EventGroupHandle_t wifiEventGroup;
static int wifiRetry = 0;

static void eventHandler(void * arg, esp_event_base_t event_base, int32_t eventId, void* eventData)
{
    if (event_base == WIFI_EVENT && eventId == WIFI_EVENT_STA_START)    
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (wifiRetry < 5)
        {
            esp_wifi_connect();
            wifiRetry++;
            ESP_LOGI("WIFI", "retry to connect to the AP");
        }
        else{
            xEventGroupSetBits(wifiEventGroup, BIT1);
        }
        ESP_LOGI("WIFI","Connect to ap fail");
        
    }
    else if (event_base == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) eventData;
        // ESP_LOGI("WIFI","GOT_IP" IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI("WIFI","GOT_IP  %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
        fileWrite("/spiflash/ssid.txt",WIFI_SSID);
        fileWrite("/spiflash/pswd.txt",PASSWORD);
        char* ssid = fileRead("/spiflash/ssid.txt");
        char* pswd = fileRead("/spiflash/pswd.txt");
        ESP_LOGI("CONNECTED ","The ssid [ %s ] and the password [ %s ]  is saved !", ssid,pswd);
        
        wifiRetry = 0;
        xEventGroupSetBits(wifiEventGroup, BIT0);
    }        
}

void WifiBegin(void)
{
    wifiEventGroup = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandler, NULL,&instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandler, NULL,&instance_got_ip));

    wifi_config_t wifiConfig = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("WIFI", "WIFI Initialized");

    EventBits_t bits = xEventGroupWaitBits(wifiEventGroup,BIT0 | BIT1,pdFALSE,pdFALSE,portMAX_DELAY);

    if (bits & BIT0) {
        ESP_LOGI("TAG", "connected to ap SSID:%s password:%s",
                 WIFI_SSID, PASSWORD);
    } else if (bits & BIT1) {
        ESP_LOGI("TAG", "Failed to connect to SSID:%s, password:%s",
                 WIFI_SSID, PASSWORD);
    } else {
        ESP_LOGE("TAG", "UNEXPECTED EVENT");
    }
}

void test(void)
{
    for(int i = 0; i < 2; i++)
    {
        printf("Start\n");
    }
    
}