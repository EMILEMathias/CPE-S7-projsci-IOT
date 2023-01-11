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