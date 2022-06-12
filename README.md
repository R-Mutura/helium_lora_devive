# helium_lora_devive
#### The repo contains all the firmware codes for the project and instructions on how to load them to the hardware

## Requirements
1) Set up the IDE, for this application you will need to download the Arduino ide [here](https://www.arduino.cc/en/software) if you don't have it already installed for your distributions
2) Navigate to `(File > Preferences)`, `(Arduino > Preferences)` on MacOS.
3) Add this URL in the text box `https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/0.0.5/package_heltec_esp32_index.json`
4) Navigate to `(Tools > Boards > Boards Manager...)` and Search for `Heltec ESP32`
5) Select the newest version and click Install
for futher instruction follow this [link](https://docs.helium.com/use-the-network/devices/development/heltec/wifi-lora-32-v2/arduino/)
6) Download [this](https://github.com/HelTecAutomation/ESP32_LoRaWAN/archive/master.zip) find into the directory below and unzip. in the folder mac os: `Documents/Arduino/libraries` and windows: `Documents\Arduino\libraries`
7)  Install Serial Driver as shown[here](https://heltec-automation-docs.readthedocs.io/en/latest/general/establish_serial_connection.html)
 
 ## Download the files 
 
 Download all the files on the `Code`:arrow_down_small: on this platform, In your downloads you will get the two folders :file_folder:  this will have:
     - DEVICE_FIRMWARE -code
     - lora-helium-moisture-sensor - hardware setup instructions and diagrams
</br> and inside the DEVICE_FIRMWARE folder `Libraries` zip file -this contains all the related libraries used in the project, `GET_ID_TEST` contains code to get the ID of the board to be used to set up the board to the network and`LORA_HELIUM_CONNECTION_DEVICE` folder containing the actual code that will be used to send data to the cloud.
 ## Library Installation
8)  On library installation unzip the folder :file_folder: `libraries` in the downloaded folder :file_folder:, copy and paste all the folders to your directory `documents/arduino/library` folder :file_folder:
9)  Open the `DEVICE_FIRMWARE` then `GET_ID_TEST` and open the file by double clicking on `GET_ID_TEST.ino`..connect the Heltec board to the computer and select the appropriate COM PORT to and press the arrow button.
10)  In the serial monitor you should see the device ID displayed. Save it for use in the device to helium setup.
11)  Open the `DEVICE_FIRMWARE` then `LORA_HELIUM_CONNECTION_DEVICE` in `LORA_HELIUM_CONNECTION_DEVICE.ino`folder :file_folder: with the downloaded ide, compile and upload to the hardware 

you should be able to see data being sent over the network with the helium console/app
For detailed explanation of setup please follow `(procedure on how to use the ide is on the tutorial link)`:link:    [tutorial](https://docs.helium.com/use-the-network/devices/development/heltec/wifi-lora-32-v2/arduino/)

