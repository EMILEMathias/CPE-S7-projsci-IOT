from tkinter import *

import serial
import time

# Graphic interface for the send program
master = Tk()

# send serial message 
# Don't forget to establish the right serial port ******** ATTENTION
SERIALPORT = "/dev/ttyACM1"
BAUDRATE = 115200
ser = serial.Serial()
import paho.mqtt.client as mqtt

def initUART():     
        ser.port=SERIALPORT
        ser.baudrate=BAUDRATE
        ser.bytesize = serial.EIGHTBITS #number of bits per bytes
        ser.parity = serial.PARITY_NONE #set parity check: no parity
        ser.stopbits = serial.STOPBITS_ONE #number of stop bits
        ser.timeout = None          #block read

        # ser.timeout = 0             #non-block read
        # ser.timeout = 2              #timeout block read
        ser.xonxoff = False     #disable software flow control
        ser.rtscts = False     #disable hardware (RTS/CTS) flow control
        ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
        #ser.writeTimeout = 0     #timeout for write
        print ("Starting Up Serial Monitor")
        try:
                ser.open()
        except serial.SerialException:
                print("Serial {} port not available".format(SERIALPORT))
                exit()

def sendUARTMessage(msg):
    ser.write(msg.encode())
    print("Message <" + msg + "> sent to micro-controller." )


def sendData(data):
        mockUpDataSPlit = [data[i:i+255] for i in range(0, len(data), 255)]
        for splitData in mockUpDataSPlit:
                if(splitData == mockUpDataSPlit[-1]):
                        sendUARTMessage(splitData)
                        sendUARTMessage("$*")
                        time.sleep(6)
                else:
                        sendUARTMessage(splitData)
                        sendUARTMessage("*")
                        time.sleep(6)

if __name__ == '__main__':
    initUART()
    sendData("(45.43204,40.12304,9)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,1)")
#     client = mqtt.Client("RF2Controller")
#     client.connect("")
#     client.publish("house/light","ON")
    print ('Press Ctrl-C to quit.')
    sendData()