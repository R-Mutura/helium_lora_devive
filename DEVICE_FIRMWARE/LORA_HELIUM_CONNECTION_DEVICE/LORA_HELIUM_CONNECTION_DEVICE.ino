#include "DHTesp.h"
//#include "heltec.h"
#include <ESP32_LoRaWAN.h>
#include "Arduino.h"

int gain = 11;
struct{
  float humidity;
  float temperature;
  float moisture;
  }all_data;

/*license for Heltec ESP32 LoRaWan, quary your ChipID relevant license: http://resource.heltec.cn/search */
uint32_t  license[4] = {0xC1670CF8, 0x19C71AD5, 0x6CE47540, 0x8CF267EC};
/* OTAA para*/
uint8_t DevEui[] = {  };
uint8_t AppEui[] = {  };
uint8_t AppKey[] = {  };

/* ABP para*/
uint8_t NwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t AppSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t DevAddr =  ( uint32_t )0x007e6ae1;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR enable*/
bool loraWanAdr = false;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = false;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6]={ 0xFF00,0x0000,0x0000,0x0000,0x0000,0x0000 };


/* Application port */
uint8_t appPort = 2;

/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 8;

/*LoraWan debug level, select in arduino IDE tools.
* None : print basic info.
* Freq : print Tx and Rx freq, DR info.
* Freq && DIO : print Tx and Rx freq, DR, DIO0 interrupt and DIO1 interrupt info.
* Freq && DIO && PW: print Tx and Rx freq, DR, DIO0 interrupt, DIO1 interrupt, MCU sleep and MCU wake info.
*/
uint8_t debugLevel = LoRaWAN_DEBUG_LEVEL;

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;


static void prepareTxFrame( uint8_t port )
{
    appDataSize = 16;//AppDataSize max value is 64
    appData[0] = port;
//    appData[1] = port;
//    appData[2] = port;
//    appData[3] = port;
}

DHTesp dht;

#define SensorPin 36
// Add your initialization code here
void setup()
{
//  if(mcuStarted==0)
//  {
//    LoRaWAN.displayMcuInit();
//  }
  Serial.begin(115200);
  while (!Serial);
  SPI.begin(SCK,MISO,MOSI,SS);
  Mcu.init(SS,RST_LoRa,DIO0,DIO1,license);
  deviceState = DEVICE_STATE_INIT;
  dht.setup(27, DHTesp::DHT11); //SETUP DHT SENSOR
}

// The loop function is called in an endless loop
void loop()
{

    all_data.temperature = dht.getTemperature();
    all_data.humidity = dht.getHumidity();
    all_data.moisture = get_moisture();
    
    uint8_t mydata[12];
    
    mydata[0] = all_data.temperature;
    mydata[4] = all_data.humidity;
    mydata[8] = all_data.moisture;
    
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
      LoRaWAN.init(loraWanClass,loraWanRegion);
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.displayJoining();
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      Serial.println("sending data");
      prepareTxFrame( *mydata );
      //LoRaWAN.send(mydata);
      LoRaWAN.send(loraWanClass);
      
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.displayAck();
      LoRaWAN.sleep(loraWanClass,debugLevel);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}

float get_moisture(){
    int item_volt;
    for (int i = 0; i <= 100; i++) 
      { 
        item_volt = item_volt + analogRead(SensorPin); 
        delay(1); 
      } 

      float sensor_value = ((item_volt/100)*3.3)/4095;
      Serial.print("Sensor Voltage = ");
      Serial.println(sensor_value);
      float item_resistance = (10000*(3.3/(3.3-sensor_value)))/(1000000*gain); //getting resistance value of the wood
      //to get moisture content approximation
      float moisture_content = ((3101/75)-(item_resistance*4.666666667));

      return moisture_content;
    
  }
