Communication between the ESP32 and [ENGO smart glasses](https://us.engoeyewear.com). You can for example collect data with ESP32 and display it as text on the ENGO device. The two devices use BLE and this repository implements parts of the protocol created by ActiveLook for the visualisation of data.

## Documenation:
* [ENGO documentation](https://github.com/ActiveLook/Activelook-API-Documentation/blob/main/ActiveLook_API.md#1-introduction)
* [esp-nimble-cpp documentation](https://h2zero.github.io/esp-nimble-cpp)

## Endianness:
ENGO is big-edian while ESP32 is little-endian.
If a command parameter is 2 bytes then the two bytes must be sent inverted. 

This is how the 2 byte parameter x is prepared for sending to ENGO:
```
(*command)[4] = (x >> 8) & 0xFF; //second byte as first one
(*command)[5] = (x >> 0) & 0xFF; //first byte as second one
```
## Compilation on Windows:

From the start menu choose : "ESP-IDF 4.4 CMD" to enter a pre-configured console. ```cd``` to the base of this repository.

```idf.py build``` - to compile the code

## Flashing:

start the device manager (devmgmt.msc) and check the COM port used by the ESP32. Look for "Silicon Labs CP210x USB to UART Bridge" or similar
From the start menu choose : "ESP-IDF 4.4 CMD" to enter a pre-configured console. Use the following commands from the base of this repository:

```idf.py -p COM4 flash``` - to compile and flash on COM4

## Configuration of ESP32

```idf.py menuconfig``` - to congiure the project, enable BLE for example
