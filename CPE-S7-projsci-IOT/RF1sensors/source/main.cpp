#include "MicroBit.h"
#include <string.h> 
#include <math.h>

MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX); 
int uwuID = 125;
int maxMsgBodyLen = 244;
int key[2][2] = {
    {1,2},
    {3,1}
};

//transform key into matrix
ManagedString encrypt(ManagedString text, int key[][2])
{
    ManagedString c("");
    int k = 0;
    int input[2];
    while (k < text.length())
    {
        input[0] = text.charAt(k);
        k++;
        input[1] = text.charAt(k);
        k++;

        for (int i = 0; i < 2; i++)
        {
            int cipher = 0;
            for (int j = 0; j < 2; j++) {
                cipher += key[i][j] * input[j];
            }
            c = ManagedString(c+(char)((cipher % 256)));
        }
    }
    return c;
}
// transforme un message de max 242 char au format uwu
ManagedString toUwuStringFormat( ManagedString strInput){
    if(strInput.length() > maxMsgBodyLen)
    {
        uBit.display.scroll("err");
        return 0;
    }
    ManagedString uwuBody(strInput);
    ManagedString uwuName("uwu");
    ManagedString uwuPID((char)uwuID);

    ManagedString uwuEncryptedMsg(encrypt(uwuName+uwuBody+uwuPID, key));
    return ManagedString(uwuEncryptedMsg);
}

// récupère les données du bus, la divise en deux et la diffuse en RF
void onSerial(MicroBitEvent e)
{
    ManagedString serialData = serial.readUntil('*');
    // ManagedString serialData = serial.read(20);
    // ManagedString mockupData = ManagedString("(0,0,2)(1,0,0)(2,0,0)(3,0,0)(4,0,0)(5,0,0)(6,0,0)(7,0,0)(8,0,0)(9,0,0)(0,1,0)(1,1,0)(2,1,0)(3,1,5)(4,1,0)(5,1,0)(6,1,0)(7,1,0)(8,1,0)(9,1,0)(0,2,0)(1,2,0)(2,2,0)(3,2,0)(4,2,3)(5,2,0)(6,2,0)(7,2,0)(8,2,0)(9,2,0)(0,3,0)(1,3,0)(2,3,0)(3,3,0)(4,3,0)(5,3,3)(6,3,0)(7,3,0)(8,3,0)(9,3,0)(0,4,0)(1,4,0)(2,4,4)(3,4,0)(4,4,0)(5,4,0)(6,4,0)(7,4,0)(8,4,0)(9,4,0)(0,5,0)(1,5,0)(2,5,0)(3,5,0)(4,5,0)(5,5,4)(6,5,0)(7,5,0)(8,5,0)(9,5,0)(0,0,2)(1,0,0)(2,0,0)(3,0,0)(1,0,0)(2,0,0)(3,0,0)(1,0,0)(1,0,0)(");
    
    // boucle à travers les données
    for(int i = 0; i*maxMsgBodyLen <= serialData.length(); i++ ){
         if((i+1)*maxMsgBodyLen > serialData.length())
            uBit.radio.datagram.send(toUwuStringFormat(serialData.substring(i*maxMsgBodyLen,serialData.length())));
        else
            uBit.radio.datagram.send(toUwuStringFormat(serialData.substring(i*maxMsgBodyLen,(i+1)*maxMsgBodyLen)));
        uBit.sleep(2000);
    }
}

int main()
{
    //setup RF and serial
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_SERIAL, MICROBIT_SERIAL_EVT_DELIM_MATCH, onSerial);
    //Set serial read buffer
    serial.setRxBufferSize(254);
    serial.eventOn("*");
    serial.read(0);

    uBit.radio.setGroup(7);
    uBit.radio.enable();

    while(1){
        uBit.sleep(1000);
    }
}