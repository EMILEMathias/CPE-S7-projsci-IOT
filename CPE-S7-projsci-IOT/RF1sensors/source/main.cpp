#include "MicroBit.h"
#include <string.h> 
#include <bitset>

MicroBit uBit;
MicroBitSerial serial(USBTX, USBRX); 
int uwuID = 125;
int maxSizeInputData = 421;
int maxUwuBodySize = 242;
char uwuSplitData[3][248];

// transforme un message de max 242 char au format uwu
int toUwuStringFormat( char* strInput,int uwuStatus){
    if(strlen(strInput) > 242)
        {
            uBit.display.scroll("err");
            return 0;
        }
    
    char uwuMsg[248];
    char uwuBody[244];
    char uwuName[4] = "uwu";

    sprintf (uwuBody, "%s",strInput);
    sprintf(uwuMsg,"%s%s",uwuName,uwuBody);

    for(int i = 0; i < 248; i++ )
        if( uwuMsg[i] == '\0') uwuMsg[i] = ' ';

    uwuMsg[245] = (char)uwuID;
    uwuMsg[246] = uwuStatus ? '*' : '$';
    uwuMsg[247] = '\0';

    strcpy(strInput,uwuMsg);
    return 1;
}

void onSerial(MicroBitEvent e)
{
    char serialData[maxSizeInputData] = {0};
    //wip
    serialData = serial.readUntil('$').toCharArray();
    //on divise les donnees
    for(int i = 0; i < maxUwuBodySize ; i++ )
        uwuSplitData[0][i] = serialData[i];
    for(int i = 0 ; i + maxUwuBodySize < maxSizeInputData ; i++ )
    {
        uwuSplitData[1][i] = serialData[i + maxUwuBodySize];
    }

    toUwuStringFormat(uwuSplitData[0],1);
    toUwuStringFormat(uwuSplitData[1],0);

    uBit.radio.datagram.send(uwuSplitData[0]);
    uBit.sleep(2000);
    uBit.radio.datagram.send(uwuSplitData[1]);
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