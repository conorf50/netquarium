# -*- coding: utf-8 -*-
#
# Cloud4RPi Based Aquarium Monitor
# ==================================
#
# This code is based on the examples found here: https://github.com/cloud4rpi/cloud4rpi-raspberrypi-python
#
# - Reads in values from the serial
# - Converts these values into variables that we can use
# - Sends these variables to the cloud


from os import uname
from socket import gethostname
import sys
import time
import random

import cloud4rpi
import rpi

import io
import serial

# Put your device token here. To get the token,
# sign up at https://cloud4rpi.io and create a device.
# Please use an environmenr variable here instead of a hard coded variable

DEVICE_TOKEN = '9zkCiNZzbMhs6eCiAT2jW8Fpm'

# Constants
DATA_SENDING_INTERVAL = 2.5  # secs
DIAG_SENDING_INTERVAL = 10  # secs
POLL_INTERVAL = 1  # 1000ms




ser = serial.Serial('/dev/ttyACM0', timeout=None, baudrate=9600)


def readFromSerial():
    serial_line = ser.readline().strip()
    serialBytes = serial_line.decode('UTF-8')
    serialString = serialBytes.split(',')
    global serialtemp
    serialtemp = (serialString[0])
        
    global seriallightLevel
    seriallightLevel = (serialString[1])
        
    global serialfeederEmpty
    serialfeederEmpty = (serialString[2])
        
    global serialwaterLevel
    serialwaterLevel = (serialString[3])
        
    global serialco2Level
    serialco2Level = (serialString[4])
      
    global serialheaterStatus
    serialheaterStatus = (serialString[5])
        
    global seriallightStatus
    seriallightStatus = (serialString[6])


# define functions that cloud4rp1 uses to send data to the cloud

def read_serial_temp():
    waterTemp = serialtemp
    return waterTemp


def read_serial_waterLevel():
    waterLevel = serialwaterLevel
    return waterLevel

def read_serial_co2Level():
    co2Level = serialco2Level
    return co2Level

def read_serial_lightLevel():
    lightLevel = seriallightLevel
    return lightLevel

def read_serial_feederStatus():
    FEEDER_EMPTY_STATUS = serialfeederEmpty
    return FEEDER_EMPTY_STATUS


def read_serial_heaterStatus():
    heaterStatus = serialheaterStatus
    return heaterStatus

def read_serial_lightStatus():
    lightStatus = seriallightStatus
    return lightStatus
    


def main():


    # Put variable declarations here
    # This array binds to the functions
    variables = {
       
        'CPU Temp': {
            'type': 'numeric',
            'bind': rpi.cpu_temp
        },
        'Feeder Empty': {
            'type': 'numeric',
            'bind': read_serial_feederStatus
        },
        'CO2 Level': {
            'type': 'numeric',
            'bind': read_serial_co2Level
        },
 	'Light Level': {
            'type': 'numeric',
            'bind': read_serial_lightLevel
        },
 	'Light Status': {
 	    'type': 'numeric',
            'bind': read_serial_lightStatus
        },
        'Water Level': {
            'type': 'numeric',
            'bind': read_serial_waterLevel
        },
        'Water Temp': {
            'type': 'numeric',
            'bind': read_serial_temp
        },
        'Heater Status': {
            'type': 'numeric',
            'bind': read_serial_heaterStatus
        },

    }

    diagnostics = {
        'CPU Temp': rpi.cpu_temp,
        'IP Address': rpi.ip_address,
        'Host': gethostname(),
        'Operating System': " ".join(uname())
    }

    device = cloud4rpi.connect(DEVICE_TOKEN)
    device.declare(variables)
    device.declare_diag(diagnostics)

    device.publish_config()

    # Adds a 1 second delay to ensure device variables are created
    time.sleep(1)

    try:
        data_timer = 0
        diag_timer = 0
        while True:
            readFromSerial()
            if data_timer <= 0:
                device.publish_data()
                data_timer = DATA_SENDING_INTERVAL

            if diag_timer <= 0:
                device.publish_diag()
                diag_timer = DIAG_SENDING_INTERVAL

            time.sleep(POLL_INTERVAL)
            diag_timer -= POLL_INTERVAL
            data_timer -= POLL_INTERVAL

    except KeyboardInterrupt:
        cloud4rpi.log.info('Keyboard interrupt received. Stopping...')

    except Exception as e:
        error = cloud4rpi.get_error_message(e)
        cloud4rpi.log.error("ERROR! %s %s", error, sys.exc_info()[0])

    finally:
        sys.exit(0)


if __name__ == '__main__':
    main()
