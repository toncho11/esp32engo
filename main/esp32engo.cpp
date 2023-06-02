/*

https://github.com/ActiveLook/Activelook-API-Documentation/blob/main/ActiveLook_API.md
https://h2zero.github.io/esp-nimble-cpp/

The extension of this file must be .cpp and you need to use 'extern "C" void app_main(void)' in order to use esp-nimble-cpp.
*/

#include "esp_log.h"
#include "NimBLEDevice.h"
#include "EngoServicesCharacteristics.h"

const char* TAG_BLE = "esp32engo";

void print_info(NimBLEClient *pClient)
{
	NimBLERemoteService *pService = pClient->getService(DeviceInformationServiceUUID);
                
	if (pService != nullptr) {
		
		ESP_LOGI(TAG_BLE, "Found DeviceInformationService");
		NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic(DeviceInformationService_ManufacturerUUID);
		
		if (pCharacteristic != nullptr) {
			
			ESP_LOGI(TAG_BLE, "Found DeviceInformationService_Manufacturer");
			std::string value = pCharacteristic->readValue();
			ESP_LOGI(TAG_BLE, "Manufacturer Name String: %s", value.c_str());
		}
		else
		{
			ESP_LOGE(TAG_BLE, "No characteristic: DeviceInformationService_Manufacturer");
		}		
	}
	else
	{
		ESP_LOGE(TAG_BLE, "No service: DeviceInformationService");
	}
}

void send_command(NimBLEClient *pClient)
{
	NimBLERemoteService *pService = pClient->getService(ActiveLookCommandsInterfaceUUID);
	
	if (pService != nullptr) {
		
		ESP_LOGI(TAG_BLE, "Found service ActiveLookCommandsInterface");
		
		NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic(ActiveLookCommandsInterface_RXActiveLookUUID);
		
		if (pCharacteristic != nullptr) 
		{ 
			ESP_LOGI(TAG_BLE, "Found characteristic: ActiveLookCommandsInterface_RXActiveLookUUID");
			
			//construct and send 
			
			//ENGO is Big Edian
			//ESP32 is Little Endian
			const uint8_t *command = (uint8_t*)malloc(7);
			
			//swap bytes only for values in the command that are bigger than 1 byte
			
			bool state = pCharacteristic->writeValue(command, 7, true);
		}
		else
		{
			ESP_LOGE(TAG_BLE, "No characteristic: ActiveLookCommandsInterface_RXActiveLook");
		}		
	}
	else
	{
		ESP_LOGE(TAG_BLE, "No service: ActiveLookCommandsInterface");
	}
	
	//put header, footer, length
	
	//draw rectangle
}

extern "C" void app_main(void)  
{
    NimBLEDevice::init("");
    
    NimBLEScan *pScan = NimBLEDevice::getScan();
    NimBLEScanResults results = pScan->start(10); //scan for 10 seconds
    
    NimBLEUUID serviceUuid(DeviceInformationServiceUUID); //TODO: better filter by manufacturer ID ending by 0x08F2
    
    for(int i = 0; i < results.getCount(); i++) {
		
        NimBLEAdvertisedDevice device = results.getDevice(i);
        
        if (device.isAdvertisingService(serviceUuid)) { //TODO: better filter by manufacturer ID ending by 0x08F2
            
			NimBLEClient *pClient = NimBLEDevice::createClient();
            
            if (pClient->connect(&device)) 
			{
				print_info(pClient);
				send_command(pClient);     
            } 
			else 
			{
                ESP_LOGE(TAG_BLE, "Could not connect to device!");
            }
            
            NimBLEDevice::deleteClient(pClient);
			ESP_LOGI(TAG_BLE, "Exit");
        }
    }
}