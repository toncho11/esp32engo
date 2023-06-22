/*

https://github.com/ActiveLook/Activelook-API-Documentation/blob/main/ActiveLook_API.md
https://h2zero.github.io/esp-nimble-cpp/

The extension of this file must be .cpp and you need to use 'extern "C" void app_main(void)' in order to use esp-nimble-cpp.
*/

#include "esp_log.h"
#include "NimBLEDevice.h"
#include "inttypes.h"
#include "EngoServicesCharacteristics.h"
#include "ENGOCommands.h"


//delay function
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char* TAG_BLE = "esp32engo_main";

void ENGO_print_info(NimBLEClient *pClient)
{
	ESP_LOGI(TAG_BLE, "print_info entered");
	
	NimBLERemoteService *pService = pClient->getService(GenericAccessUUID);
                
    /*std::vector< NimBLERemoteCharacteristic * >* chars = pService->getCharacteristics(true);
	
	for (int c=0; c<chars->size(); c++)
	{
		NimBLERemoteCharacteristic* cc = chars[c].at(c);
		ESP_LOGI(TAG_BLE, "char: %s", cc->getUUID().toString().c_str());
	} */
	
	if (pService != nullptr) {
		
		ESP_LOGI(TAG_BLE, "Found GenericAccessUUID");
		NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic(GenericAccess_DeviceNameUUID);
		
		if (pCharacteristic != nullptr) {
			
			ESP_LOGI(TAG_BLE, "GenericAccess_DeviceName");
			std::string value = pCharacteristic->readValue();
			ESP_LOGI(TAG_BLE, "Device Name String: %s", value.c_str());
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

void AddHF(uint8_t** command, int length)
{
	bool bit5is1 = (*command)[2] & (1<<5); //Bit 5 (0bx0000) defines the size of Length, 1: length on 2 bytes, 0 length on 1 byte.
	
	if (length < 255 && bit5is1)
		ESP_LOGE(TAG_BLE, "Detected an error. The size of the length is not set correctly. Bit 5 must be 0!");
	
	if (length > 255 && !bit5is1)
		ESP_LOGE(TAG_BLE, "Detected an error. The size of the length is not set correctly. Bit 5 must be 1!");

	if (length > 532)
		ESP_LOGE(TAG_BLE, "Detected an error. Command is longer than the maximum allowed length of 533 bytes!");

	(*command)[0] = 0xFF; //start
	(*command)[length - 1] = 0xAA; //end
}

void ENGO_send_commands(NimBLEClient *pClient)
{
	ESP_LOGI(TAG_BLE, "send_command entered");
	
	NimBLERemoteService *pService = pClient->getService(ActiveLookCommandsInterfaceUUID);
	
	if (pService != nullptr) {
		
		ESP_LOGI(TAG_BLE, "Found service ActiveLookCommandsInterface");
		
		NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic(ActiveLookCommandsInterface_RXActiveLookUUID);
		
		if (pCharacteristic != nullptr) 
		{ 
			ESP_LOGI(TAG_BLE, "Found characteristic: ActiveLookCommandsInterface_RXActiveLookUUID");
			
			//TODO: maximum set to 160 currently
			uint8_t* command = (uint8_t*)malloc(160); //make a warning if a command goes beyond the maximum
			size_t* length = (size_t*)malloc(sizeof(size_t));//total length of the command
			
			bool state;
			
            /*  
			Demo(0, &command, length);
			AddHF(&command, *length);
			state = pCharacteristic->writeValue(command, *length, true); 
			free(command);
			*/
			
			int x = 300;
			int y = 100;

			for (int i=0; i < x ; i=i+4)
			{
				//ESP_LOGI(TAG_BLE, "Start i: %d", i);
				
				Text(i, 180, 4, 2, 15, "O2 34", &command, length);
				AddHF(&command, *length);
				state = pCharacteristic->writeValue(command, *length, true);

				Text(i, 150, 4, 2, 15, "CO2 22", &command, length);
				AddHF(&command, *length);
				state = pCharacteristic->writeValue(command, *length, true);

				vTaskDelay(50 / portTICK_PERIOD_MS);

				clear_screen(&command, length);
				AddHF(&command, *length);
				state = pCharacteristic->writeValue(command, *length, true);

				//ESP_LOGI(TAG_BLE, "End i: %d", i);
			}

			free(command);

			//ENGO is Big Edian
			//ESP32 is Little Endian
			//swap bytes only for values in the command that are bigger than 1 byte
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

void ENGO_connect(NimBLEClient **pClient, bool *found, bool *connected)
{   
    NimBLEDevice::init("");
    
    NimBLEScan *pScan = NimBLEDevice::getScan();
    NimBLEScanResults results = pScan->start(5); //scan for 10 seconds
    
    //NimBLEUUID serviceUuid(DeviceInformationServiceUUID); //TODO: better filter by manufacturer ID ending by 0x08F2
	
    for(int i = 0; i < results.getCount(); i++) {
		
        NimBLEAdvertisedDevice device = results.getDevice(i);
        
		std::string devName = "";
		
		*found = false;
		*connected = false;
		
		if (device.haveName())
		{
			devName = device.getName();
			ESP_LOGI(TAG_BLE, "Device name: %s", devName.c_str());
		}
		else
			continue;
		
		//if (!device.haveManufacturerData())
        //    continue;			
        
		//std::string manfData = device.getManufacturerData();
		
		//ESP_LOGI(TAG_BLE, "Manufacturer data: %s", manfData.c_str());
		
		std::size_t foundName = devName.find("ENGO");
		
        if (foundName != std::string::npos) { //TODO: better filter by manufacturer ID ending by 0x08F2
            
			*found = true;
			ESP_LOGI(TAG_BLE, "ENGO Device found.");
		
		    *pClient = NimBLEDevice::createClient();
            
            if ((*pClient)->connect(&device)) 
			{
				*connected = true;
				ENGO_print_info(*pClient);  
            } 
			else 
			{
                ESP_LOGE(TAG_BLE, "Could not connect to ENGO device!");
            }
            
			ESP_LOGI(TAG_BLE, "Device found.");
			
			break;
        }
	}
}


extern "C" void app_main(void)  
{
    NimBLEClient *pClient = NULL;
	
	bool found;
	bool connected;
	
	ENGO_connect(&pClient, &found, &connected);
	
	if (found && connected && pClient != NULL)
	{
		ENGO_send_commands(pClient);
		NimBLEDevice::deleteClient(pClient);
		ESP_LOGI(TAG_BLE, "Done.");
	}
	else
		ESP_LOGI(TAG_BLE, "Device not found. Exit.");
}