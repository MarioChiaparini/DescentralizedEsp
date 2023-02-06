#include<iostream>
#include <cstdlib>
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "esp_modem.h"

// create a static pointer which the value is "descentralized modem"
static const char * TAG = "descentralized_modem";

// esp_err_t  - is a function prototype like int, float, void, etc...

// esp_err_t is the retun type of the function for error in the code

// *ctx pointer is actually use to pass the data for the event handler function 

esp_err_t event_handler(void *ctx, system_event_t event)
{
    switch(event -> event_id){
        case SYSTEM_EVENT_STA_START: // Event generated starts as Wifi Station 
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        esp_wifi_connect();
        break;
        
        case SYSTEM_EVENT_STA_GOT_IP: // Event generated starts as Wifi Station 
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        ESP_LOGI(TAG, "Got IP: %s \n" , esp_ip4addr_ntoa(&event -> event_info.got.ip.ip_info.ip));
        break;

        case SYSTEM_EVENT_STA_DISCONNECTED: // Case: got disconnected from wifi
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
        esp_wifi_connect(); //calling the process f connecting 
        break;

        case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "Device disconnected from the ESPwifi")
        
        default:
        break;
    }
    return ESP_OK;
}

class Credentials{
    public:
    Credentials(string password, string SSID){
        this->password = password;
        this->SSID = SSID;
    }
}

int main(int argc, char** argv){

    // Create a object eith the class "Credentials"
    Credentials credentials("12345", "wifi");
    
    string pass = credentials.password;
    string ssid = credentials.SSID;

    std::cout << "ESP32 Descentralized Modem" << std::endl;

    // Non-VOlatil Storage (NVS) flash ESP32
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        // There any error erase the flash
        ESP_ERROR_CHECK(nvs_flash_erase());
        // Proper initialization 
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    // TCP/IP Modem
    tcpip_adapter_init();
    //initialize the modem
    esp_modem_init();
    // Checking the errors  
    ESP_ERROR_CHECK(esp_event_loop( event_handler, NULL));
    // Wifi start
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERRROR_CHECK(esp_wifi_init(&config));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    // Setting the credentials 
    wifi_config_t wifi_conf = {};
    strcpy((char*)wifi_conf.sta.ssid, ssid);
    strcpy((char*)wifi_conf.sta.password, pass);
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    while (true){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return 0;
    }

