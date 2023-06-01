# esp32engo
Communication between the ESP32 and ENGO smart glasses. ENGO's documentation is available [here](https://github.com/ActiveLook/Activelook-API-Documentation/blob/main/ActiveLook_API.md#1-introduction).

# Compilation on Windows:

From the start menu choose : "ESP-IDF 4.4 CMD" to enter a pre-configured console
cd to the e_velo folder
idf.py build

# Flashing:

start the device manager (devmgmt.msc) and check the COM port used by the ESP32. Look for "Silicon Labs CP210x USB to UART Bridge" or similar
From the start menu choose : "ESP-IDF 4.4 CMD" to enter a pre-configured console

cd to the esp32engo folder

idf.py build - only to compile

idf.py -p COM4 flash - if you are using COM4, execute from the folder of esp32engo

# Configuration of ESP32

idf.py menuconfig - from the folder of esp32engo
