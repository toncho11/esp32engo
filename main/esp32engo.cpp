/*

https://github.com/ActiveLook/Activelook-API-Documentation/blob/main/ActiveLook_API.md
https://h2zero.github.io/esp-nimble-cpp/

The extension of this file must be .cpp and you need to use 'extern "C" void app_main(void)' in order to use esp-nimble-cpp.
*/

#pragma GCC diagnostic ignored "-Wuninitialized"

#include "esp_log.h"
#include "NimBLEDevice.h"
#include "EngoServicesCharacteristics.h"
#include "inttypes.h"
#include "endian.h" //for the host to be functions

const char* TAG_BLE = "esp32engo";

void print_info(NimBLEClient *pClient)
{
	ESP_LOGI(TAG_BLE, "print_info entered");
	
	NimBLERemoteService *pService = pClient->getService(GenericAccessUUID);
                
/* 	std::vector< NimBLERemoteCharacteristic * >* chars = pService->getCharacteristics(true);
	
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
	(*command)[0] = 0xFF; //start
	(*command)[length - 1] = 0xAA; //end
}

/* s16 x
s16 y
u8 r
u8 f
u8 c
str string[255] */
//TODO: MSB check

void Text(int16_t x, int16_t y, uint8_t r, uint8_t f, uint8_t c, const char* text, uint8_t** command, int* length)
{
	ESP_LOGI(TAG_BLE, "text_command entered");
	
	int str_length = strlen(text);
	
	*length = 13; //total length of command
	
	*command = (uint8_t*)malloc(*length);
	
	(*command)[1] = 0x37; //txt command
	
	(*command)[2] = 0; //TODO: calculate the text length and decide if the length of the command is 1 or 2 bytes
	
	(*command)[3] = *length;
	
	//next is data for the command - parameters for the text 
	
	//takes 2 bytes
	//x = htobe16(x);
	//(*command)[4] = (x >> (8*0)) & 0xff; 
	//(*command)[5] = (x >> (8*1)) & 0xff; 
	(*command)[4] = htobe16(x); //host to big endian 16
	
	//y = htobe16(y);
	//(*command)[6] = (y >> (8*0)) & 0xff; 
	//(*command)[7] = (y >> (8*1)) & 0xff; 
	
	//takes 2 bytes
	//*(command+6) = htobe16(y);
	(*command)[6] = htobe16(y);
	
	(*command)[8]  = r;
	(*command)[9]  = f;
	(*command)[10] = c;
	
	(*command)[11] = 'p'; //text
	
	//*length = total_length; //must also reserve 2 for header / footer		
}

void Demo(uint8_t demo_id, uint8_t** command, int* length)
{
	*command = (uint8_t*)malloc(6);
	
	(*command)[1] = 0x03; //command id - demo now
	(*command)[2] = 0;
	//*command[2] = 1; //Command Format, 
	//*command[2] << 4;
	//bit 5 is the size of the length, must be 0 for demo
	//bit 4 to 1 defines the size of the query id
	//query id must be set to 0 for the demo command, because there is no query id
	
	(*command)[3] = 6; //size of all is 6 now
	(*command)[4] = demo_id; //data for the command - parameter for the demo in this case
	
	*length = 6; //must also reserve 2 for header / footer		
}

void send_command(NimBLEClient *pClient)
{
	ESP_LOGI(TAG_BLE, "send_command entered");
	
	NimBLERemoteService *pService = pClient->getService(ActiveLookCommandsInterfaceUUID);
	
	if (pService != nullptr) {
		
		ESP_LOGI(TAG_BLE, "Found service ActiveLookCommandsInterface");
		
		NimBLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic(ActiveLookCommandsInterface_RXActiveLookUUID);
		
		if (pCharacteristic != nullptr) 
		{ 
			ESP_LOGI(TAG_BLE, "Found characteristic: ActiveLookCommandsInterface_RXActiveLookUUID");
			
			uint8_t* command = NULL;
			int* length = (int*)malloc(sizeof(int));//total length of the command
			
			bool state;
			
            Demo(0, &command, length);
			AddHF(&command, *length);
			free(command);
			state = pCharacteristic->writeValue(command, *length, true);
			
			Text(100, 100, 4, 2, 15, "text", &command, length);
			AddHF(&command, *length);
			state = pCharacteristic->writeValue(command, *length, true);
			free(command);
			
			//ENGO is Big Edian
			//ESP32 is Little Endian
			//const uint8_t *command = (uint8_t*)malloc(7);
			
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

extern "C" void app_main(void)  
{
    NimBLEDevice::init("");
    
    NimBLEScan *pScan = NimBLEDevice::getScan();
    NimBLEScanResults results = pScan->start(5); //scan for 10 seconds
    
    //NimBLEUUID serviceUuid(DeviceInformationServiceUUID); //TODO: better filter by manufacturer ID ending by 0x08F2
    
	bool ENGOfound = false;
	
    for(int i = 0; i < results.getCount(); i++) {
		
        NimBLEAdvertisedDevice device = results.getDevice(i);
        
		std::string devName = "";
		
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
            
			ENGOfound = true;
			ESP_LOGI(TAG_BLE, "ENGO Device found.");
			
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
            
            //NimBLEDevice::deleteClient(pClient);
			ESP_LOGI(TAG_BLE, "Device found. Exit.");
			
			break;
        }
    }
	
	if (!ENGOfound)
		ESP_LOGI(TAG_BLE, "Device not found. Exit.");
}