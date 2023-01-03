#include "MicroBit.h"
#include <string.h> 
#include <bitset>

MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX); 
int uwuID = 125;

// transforme un message de max 242 char au format uwu
ManagedString toUwuStringFormat( ManagedString strInput,int uwuStatus){
    if(strInput.length() > 242)
    {
        uBit.display.scroll("err");
        return 0;
    }
    ManagedString uwuBody(strInput);
    ManagedString uwuName("uwu");
    ManagedString uwuPID((char)uwuID);
    ManagedString uwuPStatus(uwuStatus ? '*' : '$');
    ManagedString uwuEndMsg("\0");

    return ManagedString(uwuName+uwuBody+uwuPID+uwuPStatus+uwuEndMsg);
}

void onSerial(MicroBitEvent e)
{
    ManagedString serialData = serial.readUntil('$');
    ManagedString firstHalfMsg = serialData.substring(0,243);
    ManagedString secondHalfMsg = serialData.substring(243,446);
    uBit.display.scroll("A1");
    uBit.display.scroll(serialData);


    uBit.radio.datagram.send(toUwuStringFormat(firstHalfMsg,1));
    uBit.sleep(2000);
    uBit.radio.datagram.send(toUwuStringFormat(secondHalfMsg,0));
    uBit.sleep(2000);
}

int main()
{
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH, onSerial);
    serial.eventOn( "$", ASYNC);
    serial.read(1,ASYNC);

    uBit.radio.setGroup(7);
    uBit.radio.enable();

    while(1){
        uBit.sleep(1000);
    }
}