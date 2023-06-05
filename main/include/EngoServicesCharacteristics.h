/*

The following services are implemented in the BLE interface (four are standard services and two are 
custom for dedicated ActiveLook commands and firmware updates).

*/

#include "inttypes.h"
#include "NimBLEDevice.h"

/* Services */

//1
const uint16_t GenericAttributeUUID = 0x1801; //Without characteristics

//2 Generic Access
static BLEUUID GenericAccessUUID((uint16_t)0x1800);

//2.1 Characteristics
//static BLEUUID GenericAccess_DeviceNameUUID("00002A00-0000-1000-8000-00805F9B34FB");
static BLEUUID GenericAccess_DeviceNameUUID((uint16_t)0x2a00);
const char* GenericAccess_AppearanceUUID = "00002A01-0000-1000-8000-00805F9B34FB"; //possible not correct
const char* GenericAccess_PeripheralUUID = "00002A04-0000-1000-8000-00805F9B34FB"; //possibly not correct

//3 Device Information Service
const uint16_t DeviceInformationServiceUUID = 0x180A;

//3.1 Characteristics
const char* DeviceInformationService_ManufacturerUUID	= "00002A29-0000-1000-8000-00805F9B34DB";
const char* DeviceInformationService_ModelUUID			= "00002A24-0000-1000-8000-00805F9B34DB";
const char* DeviceInformationService_SerialUUID		    = "00002A25-0000-1000-8000-00805F9B34DB";
const char* DeviceInformationService_HardwareUUID		= "00002A27-0000-1000-8000-00805F9B34DB";
const char* DeviceInformationService_FirmwareUUID		= "00002A26-0000-1000-8000-00805F9B34DB";
const char* DeviceInformationService_SoftwareUUID		= "00002A28-0000-1000-8000-00805F9B34DB";

//4 Battery Service
const uint16_t BatteryServiceUUID = 0x180F;

//4.1 Characteristics
const char* BatteryService_BatteryLevelUUID	= "00002A19-0000-1000-8000-00805F9B34FB"; //battery level (in %)

//5 Custom Service ActiveLook Command Interface
static BLEUUID ActiveLookCommandsInterfaceUUID("0783b03e-8535-b5a0-7140-a304d2495cb7");

//5.1 Characteristics
const char* ActiveLookCommandsInterface_TXActiveLookUUID = "0783b03e-8535-b5a0-7140-a304d2495cb8";

static BLEUUID ActiveLookCommandsInterface_RXActiveLookUUID("0783b03e-8535-b5a0-7140-a304d2495cbA");

const char* ActiveLookCommandsInterface_ControlUUID	     = "0783b03e-8535-b5a0-7140-a304d2495cb9";
const char* ActiveLookCommandsInterface_GestureEventUUID = "0783b03e-8535-b5a0-7140-a304d2495cbb";
const char* ActiveLookCommandsInterface_TouchEventUUID   = "0783b03e-8535-b5a0-7140-a304d2495cbc";

//6 Custom Service Firmware Update
const char* FirmwareUpdateServiceUUID = "0000FEF5-0000-1000-8000-00805F9B34FB";
