/******************************************************************************
sense.ino
Based on an example for the TMP102 I2C Temperature Sensor by Alex Wende @ SparkFun Electronics
Author: Conor Farrell
Date: December 5 2017
~

This sketch configures uses a TMP102 temperature sensor and a series of other sensors to monitor an aquarium.
The full setup can be found at <insert Gitgub here>

Resources:
Wire.h (included with Arduino IDE)
SparkFunTMP102.h

Development environment specifics:
Arduino 1.0+
Hardware Version 13

Hardware Required:
Arduino
Breadboard
Button/Switch
LDR - Light Dependent Resistor
TMP102
Connecting wires and resistors
Potentiometer (replace with water level sensor)
Second pot (replace with C02 sensor)
Light LED - (replace with relay connected to lights)
Heater Status LED (add a relay to control the heater)


Distributed as-is; no warranty is given.   
******************************************************************************/

#include <Wire.h> // Used to establied serial communication on the I2C bus
#include "SparkFunTMP102.h" // Used to send and recieve specific information from our sensor

// Connections for TMP102
// VCC = 3.3V
// GND = GND
// SDA = A4
// SCL = A5

// Define I/O
const int ALERT_PIN = A3;   //Not used
const int LDR_PIN = A1;  //Measure the light level
const int CO2_SENSOR_PIN = A2; // Measure the C02 level

const int LIGHT_PIN = 5;   //Connect to relay tp control lights
const int WATER_SENSOR_PIN = A0;  // Connect to water level sensor
const int FEEDER_PIN = 2;    //Used to monitor an automatic feeder and check if it is empty
const int HEATER_STATUS_PIN = 4;   // Connect to a relay connected to a heater


int tempThreshold = 20; /*Define a temperature threshold, If the temp is below this
value, turn on the heater. If the value is equal or greater than this value, tuen off the heater.
*/
 
TMP102 sensor0(0x48); // Initialize sensor at I2C address 0x48
// Sensor address can be changed with an external jumper to:
// ADD0 - Address
//  VCC - 0x49
//  SDA - 0x4A
//  SCL - 0x4B

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  
  pinMode(ALERT_PIN,INPUT);  // Declare alertPin as an input
  pinMode(LDR_PIN,INPUT);  // Declare lightLevelPin as an input
  pinMode(WATER_SENSOR_PIN,INPUT);  // Declare waterLevelSensorPin as an input
  pinMode(CO2_SENSOR_PIN,INPUT);  // Declare co2SensorPin as an input

  pinMode(FEEDER_PIN,INPUT);  // Declare feederPin as an input

  pinMode(LIGHT_PIN,OUTPUT);      // Connected to a LED for demo, connected to light relay in final version
  pinMode(HEATER_STATUS_PIN,OUTPUT);      // Connected to a LED for demo, connected to heater relay in final version




  sensor0.begin();  // Join I2C bus

  // Initialize sensor0 settings
  // These settings are saved in the sensor, even if it loses power

  // set the number of consecutive faults before triggering alarm.
  // 0-3: 0:1 fault, 1:2 faults, 2:4 faults, 3:6 faults.
  sensor0.setFault(0);  // Trigger alarm immediately

  // set the polarity of the Alarm. (0:Active LOW, 1:Active HIGH).
  sensor0.setAlertPolarity(1); // Active HIGH

  // set the sensor in Comparator Mode (0) or Interrupt Mode (1).
  sensor0.setAlertMode(0); // Comparator Mode.

  // set the Conversion Rate (how quickly the sensor gets a new reading)
  //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
  sensor0.setConversionRate(2);

  //set Extended Mode.
  //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
  sensor0.setExtendedMode(0);

  //set T_HIGH, the upper limit to trigger the alert on
 // sensor0.setHighTempF(85.0);  // set T_HIGH in F
  sensor0.setHighTempC(32); // set T_HIGH in C

  //set T_LOW, the lower limit to shut turn off the alert
  //sensor0.setLowTempF(84.0);  // set T_LOW in F
  sensor0.setLowTempC(8); // set T_LOW in C
}

void loop()
{
  int temperature, lightLevel, light, co2Level, waterLevel;
  boolean alertPinState, alertRegisterState, feederIsEmpty, heaterStatus, lightStatus;
  /*
   * Read from the serial to get values for LEDS
   */

  // Turn sensor on to start temperature measurement.
  // Current consumtion typically ~10uA.
  sensor0.wakeup();

  // read temperature data
  //temperature = sensor0.readTempF();
  temperature = sensor0.readTempC();

  // Check for Alert
  alertPinState = digitalRead(ALERT_PIN); // read the Alert from pin
  alertRegisterState = sensor0.alert();   // read the Alert from register

  // Place sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor0.sleep();

  //Read the states of the other pins
  feederIsEmpty = digitalRead(FEEDER_PIN); 
  waterLevel = analogRead(WATER_SENSOR_PIN);
  lightLevel = analogRead(LDR_PIN);
  co2Level = analogRead(CO2_SENSOR_PIN);

  // Control the heater automatically. If the temp goes over the specified value,
  // turn off the heater to avoid boiling the fish alive.
  // If the temp hits 0 (sensor failed) turn on and off the heater every few minutes
  // and let cloud4rpi handle an email alert.

  if(temperature >= tempThreshold){
    digitalWrite(HEATER_STATUS_PIN, LOW);
    heaterStatus = digitalRead(HEATER_STATUS_PIN); //set this to 0
  }
  else{
    digitalWrite(HEATER_STATUS_PIN, HIGH);
    heaterStatus = digitalRead(HEATER_STATUS_PIN);  // set this to 1
  }

  printToSerial(temperature, feederIsEmpty, waterLevel, lightLevel, co2Level, heaterStatus, lightStatus);


}



  void printToSerial(int temperature, boolean feederIsEmpty, int waterLevel, int lightLevel, int co2Level, boolean heaterStatus, boolean lightStatus){
  /*
   * OUTPUT FORMAT = temp,lightLevel,feederEmpty,waterLevel, co2Level, heaterStatus, lightStatus
   */
  Serial.print(temperature);

  Serial.print(",");
  Serial.print(lightLevel/10);

  Serial.print(",");
  Serial.print(feederIsEmpty);
  
  Serial.print(",");
  Serial.print(waterLevel/100);

  Serial.print(",");
  Serial.print(co2Level/100);

  Serial.print(",");
  Serial.print(heaterStatus);

  Serial.print(",");
  Serial.println(lightStatus);
  delay(2000);  // Wait 2 seconds

}

