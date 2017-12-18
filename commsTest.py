import time
import io
import serial
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


def main():
    while 1:
        readFromSerial()
        time.sleep (1)
        printToSerial()
    ser.close() # Only executes once the loop exits    
     


def printToSerial():
     print("Temp = %s , Light Level = %s, Feeder Status = %s, Water Level = %s, CO2 Level = %s, Heater Status = %s, Light Status = %s" % (serialtemp,       seriallightLevel, serialfeederEmpty, serialwaterLevel, serialco2Level, serialheaterStatus, seriallightStatus))





if __name__ == "__main__":
    main()


