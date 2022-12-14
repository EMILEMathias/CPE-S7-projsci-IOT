# Program to control passerelle between Android application
# and micro-controller through USB tty
import sys
import serial
import glob
from datetime import datetime
import os
import paho.mqtt.client as mqtt

SERIALPORT = ""
BAUDRATE = 115200
ser = serial.Serial()

# search and assign port 
def findSerialPort():
   SerialPort = ""
   if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
   elif sys.platform.startswith('lin'):
        ports = glob.glob('/dev/tty[Aa]*')
   else:
        ports = ''
   for port in ports:
        try:
            SerialPort = "/dev/ttyACM0"
            # SerialPort = port 
        except (OSError, serial.SerialException):
            pass
   return SerialPort

def initUART():
    # ser = serial.Serial(SERIALPORT, BAUDRATE)
    SERIALPORT = findSerialPort()
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
    print("Starting Up Serial Monitor")
    try:
        ser.open()
        print("Serial Monitor is started on port {}".format(SERIALPORT))
    except serial.SerialException as aled:
        print("No Serial Port is available {}".format(aled))
        exit()


# Main program logic follows:
if __name__ == '__main__':
    initUART()
    client = mqtt.Client("RF2Controller")
    client.username_pw_set("emergency", "pimpon")
    client.connect("localhost",1883)
    
    print ('Press Ctrl-C to quit.')
    try:
        while ser.isOpen() :
            if (ser.inWaiting() > 0): # if incoming bytes are waiting
                # wait for a $ in serial bus 
                dataStr = str(ser.read_until(b'\x24')).replace("b","").replace("'","").replace("uwu","").replace("$","")
                os.system('clear')
                print("Serial Port : "+ ser.port + "\nReceived data :\n" + dataStr )
                client.publish("emergencyAccidents",payload=dataStr)
                client.publish("grafanaAccidents",payload=dataStr)

    except (KeyboardInterrupt, SystemExit):
        ser.close()
        exit()