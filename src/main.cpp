//I, Cameron Randall, 000820614 guanantee that this is my work and my work only.

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

//Device address
DeviceAddress deviceAddress;

//variables for later use, if the sensor is connected
bool sensorConnected = false;
bool initalConnected = false;

//simple loop to print out DeviceAddress array
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

  //make some space
  Serial.println("\n");
  Serial.println("Temperature Application");

  //getAddress returns a boolean & passes a value into a given variable
  //so if we have an address, print it & set our connected variables to true
  if (DS18B20.getAddress(deviceAddress, 0)) {
    Serial.print("Found DS18B20 sensor with address: ");
    printAddress(deviceAddress);
    Serial.print("\n");
    sensorConnected = true;
    initalConnected = true;
  //otherwise it's an error
  } else {
    Serial.println("No DS18B20 temperature sensors are installed!");
  }

  
}

void loop() {
  float fTemp;
  String output;

  //initialConnected is only modified once at start time
  if (initalConnected) {
    // ask DS18B20 for the current temperature 
    DS18B20.requestTemperatures(); 
    // fetch the temperature.  We only have 1 sensor, so the index is 0. 
    fTemp = DS18B20.getTempCByIndex(0); 
    // if no sensor is connected, set our other sensor bool to false
    //otherwise set our output depending on value range
    if (fTemp == -127) {
      sensorConnected = false;
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
    //if sensor is connected, print formatted output
    if (sensorConnected) {
      Serial.printf("Current temperature is: %4.2f deg. C or %s \n", fTemp, output);
    //if it's not, print an error message
    } else {
      Serial.println("No DS18B20 temperature sensors are installed!");
    }
  //initalConnected - if the sensor was *never* connected at start-time
  } else {
    //do nothing
  }
  
  // wait 5s
  delay(5000); 
}


