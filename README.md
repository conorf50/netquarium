# netquarium
## 2019 README.md Update - This is only a proof of concept and will not interface with anything other than the TMP102 temperature sensor. Feel free to fork this project and use it for your own needs. I do NOT accept responsibilty for any damages caused by the use of this software, use at your own risk!



### Introduction:
A "smart" aquarium monitoring system based on commodity I.O.T hardware and the cloud4Rpi cloud platform. This project allows a fishkeeper to use a simple web interface to remotely monitor an aquarium and control the heater and lights. The web dashboard displays the data from the aquarium in a easy to understand manner. 


### Install Guide
This is an ongoing project so instructions are likely to change. The current instructions are as follows
1. Gather the required hardware. The list is as follows
  * Arduino
  * Raspberry Pi
  * Breadboard
  * Connecting wires and hardware
  * TMP 102 temperature sensor
  * USB cable to connect the Arduino to the Raspberry Pi
  * LDR
  * Resistors and potentionometers
2. Sign up to [cloud4Rpi](https://cloud4rpi.io/register) and select 'New Device' button. Follow the steps to add a Raspberry Pi to the platform at [this link](http://docs.cloud4rpi.io/start/rpi/)
3. Connect the Arduino to a computer with a USB cable and program it with the supplied sketch
4. Connect the sensors to the Arduino using the schematic provided in the sketch
5. Connect the Arduino to the Pi and connect the Pi to the internet.
6. cd into the project directory and run the script with Python 3.

Make sure that the device token is changed first!

7. Go to the [cloud4rpi dashboard](https://cloud4rpi.io/control-panels), click on 'New Control Panel' and add widgets and graphs to the dashboard. The widgets get data from the sources defined in the python file.


