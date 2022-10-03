#include <Arduino.h>
// 1-Wire sensor communication libary 
#include <OneWire.h> 
// DS18B20 sensor library 
#include <DallasTemperature.h> 
 
// Pin that the  DS18B20 is connected to 
const int oneWireBus = D3;      
 
// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWire(oneWireBus); 
 
// Pass our oneWire reference to Dallas Temperature sensor  
DallasTemperature DS18B20(&oneWire); 

DeviceAddress deviceAddress;

bool sensorConnected = false;
bool initalConnected = false;

void printAddress(DeviceAddress deviceAddress) {
  for (int i = 0; i < 8; i++) {
    Serial.print(deviceAddress[i], HEX);
  }
}

void setup() {
    // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // Start the DS18B20 sensor 
  DS18B20.begin(); 
  //DS18B20.getAddress(deviceAddress, 0);


  Serial.println("\n");
  Serial.println("Temperature Application");

  if (DS18B20.getAddress(deviceAddress, 0)) {
    Serial.print("Found DS18B20 sensor with address: ");
    printAddress(deviceAddress);
    Serial.print("\n");
    sensorConnected = true;
    initalConnected = true;
  } else {
    Serial.println("No DS18B20 temperature sensors are installed!");
    //exit(0);
  }

  
}

void loop() {
  float fTemp;
  String output;

  if (initalConnected) {
  // ask DS18B20 for the current temperature 
    DS18B20.requestTemperatures(); 
    // fetch the temperature.  We only have 1 sensor, so the index is 0. 
    fTemp = DS18B20.getTempCByIndex(0); 
    // print the temp to the USB serial monitor 
    if (fTemp == -127) {
      sensorConnected = false;
      //Serial.println("No DS18B20 temperature sensors are installed!");
    } else if (fTemp < 10) {
      output = "Cold!";
    } else if ((fTemp >= 10) && (fTemp < 15)) {
      output = "Cool";
    } else if ((fTemp >= 15) && (fTemp < 25)) {
      output = "Perfect";
    } else if ((fTemp >= 25) && (fTemp < 30)) {
      output = "Warm";
    } else if ((fTemp >= 30) && (fTemp <= 35)) {
      output = "Hot";
    } else if (fTemp > 35) {
      output = "Too Hot!";
    }
    if (sensorConnected) {
      Serial.printf("Current temperature is: %4.2f deg. C or %s \n", fTemp, output);
    } else {
      Serial.println("No DS18B20 temperature sensors are installed!");
    }
  } else {
    //do nothing
  }
  
  // wait 5s
  delay(5000); 
}


