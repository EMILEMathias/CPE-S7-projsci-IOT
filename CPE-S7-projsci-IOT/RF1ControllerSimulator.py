from tkinter import *

import serial
import time

# Graphic interface for the send program
master = Tk()

# send serial message 
# Don't forget to establish the right serial port ******** ATTENTION
SERIALPORT = "/dev/ttyACM2"
BAUDRATE = 115200
ser = serial.Serial()

def initUART():     
        if serialButton['text'] == "Open Serial":   
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
                serialButton['text'] = "Close Serial"
                b['state'] = 'normal'
        else:
                ser.close()
                serialButton['text'] = "Open Serial"
                b['state'] = 'disabled'


def sendUARTMessage(msg):
    ser.write(msg.encode())
    print("Message <" + msg + "> sent to micro-controller." )


def send_scales():
        mockUpData = "(45.43204,40.12304,9)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,0)(45.43204,40.12304,1)"
        mockUpDataSPlit = [mockUpData[i:i+255] for i in range(0, len(mockUpData), 255)]
        for splitData in mockUpDataSPlit:
                if(splitData == mockUpDataSPlit[-1]):
                        sendUARTMessage(splitData)
                        sendUARTMessage("$*")
                        time.sleep(6)
                else:
                        sendUARTMessage(splitData)
                        sendUARTMessage("*")
                        time.sleep(6)
        
b=Button(master,text="Send Values",highlightcolor="blue",command=send_scales, state="disabled") # button to read values
serialButton=Button(master,text="Open Serial",highlightcolor="blue",command=initUART) # button to read values
b.grid(row=6,column=7,columnspan = 3)
serialButton.grid(row=6, column=0, columnspan = 3)

mainloop()