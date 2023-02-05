#include<iostream>
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

// esp_err_t event_handler - is a function prototype like int, float, void, etc...

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

