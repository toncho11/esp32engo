/*

The following services are implemented in the BLE interface (four are standard services and two are 
custom for dedicated ActiveLook commands and firmware updates).

*/

#ifndef ENGO_SERVICES_H
#define ENGO_SERVICES_H

#include "inttypes.h"
#include "NimBLEDevice.h"

/* Services */

//1
static BLEUUID GenericAttributeUUID((uint16_t)0x1801); //Without characteristics

//2 Generic Access
static BLEUUID GenericAccessUUID((uint16_t)0x1800);

//2.1 Characteristics
static BLEUUID GenericAccess_DeviceNameUUID((uint16_t)0x2a00); //currently the UUID is different in the documenation
static BLEUUID GenericAccess_AppearanceUUID((uint16_t)0x2a01); //currently the UUID is different in the documenation
static BLEUUID GenericAccess_PeripheralUUID((uint16_t)0x2A04); //currently the UUID is different in the documenation

//3 Device Information Service
static BLEUUID  DeviceInformationServiceUUID((uint16_t)0x180A);

//3.1 Characteristics
static BLEUUID DeviceInformationService_ManufacturerUUID("00002A29-0000-1000-8000-00805F9B34DB");
static BLEUUID DeviceInformationService_ModelUUID("00002A24-0000-1000-8000-00805F9B34DB");
static BLEUUID DeviceInformationService_SerialUUID("00002A25-0000-1000-8000-00805F9B34DB");
static BLEUUID DeviceInformationService_HardwareUUID("00002A27-0000-1000-8000-00805F9B34DB");
static BLEUUID DeviceInformationService_FirmwareUUID("00002A26-0000-1000-8000-00805F9B34DB");
static BLEUUID DeviceInformationService_SoftwareUUID("00002A28-0000-1000-8000-00805F9B34DB");

//4 Battery Service
static BLEUUID BatteryServiceUUID((uint16_t)0x180F);

//4.1 Characteristics
static BLEUUID BatteryService_BatteryLevelUUID("00002A19-0000-1000-8000-00805F9B34FB"); //battery level (in %)

//5 Custom Service ActiveLook Command Interface
static BLEUUID ActiveLookCommandsInterfaceUUID("0783b03e-8535-b5a0-7140-a304d2495cb7");

//5.1 Characteristics
static BLEUUID ActiveLookCommandsInterface_TXActiveLookUUID("0783b03e-8535-b5a0-7140-a304d2495cb8");
static BLEUUID ActiveLookCommandsInterface_RXActiveLookUUID("0783b03e-8535-b5a0-7140-a304d2495cbA");
static BLEUUID ActiveLookCommandsInterface_ControlUUID("0783b03e-8535-b5a0-7140-a304d2495cb9");
static BLEUUID ActiveLookCommandsInterface_GestureEventUUID("0783b03e-8535-b5a0-7140-a304d2495cbb");
static BLEUUID ActiveLookCommandsInterface_TouchEventUUID("0783b03e-8535-b5a0-7140-a304d2495cbc");

//6 Custom Service Firmware Update
static BLEUUID FirmwareUpdateServiceUUID("0000FEF5-0000-1000-8000-00805F9B34FB");

#endif
