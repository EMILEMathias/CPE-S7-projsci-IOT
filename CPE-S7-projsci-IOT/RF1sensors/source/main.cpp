#include "MicroBit.h"
#include <string.h> 
#include <bitset>

MicroBit uBit;
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
    uwuMsg[246] = uwuStatus ? '1' : '0';
    uwuMsg[247] = '\0';

    strcpy(strInput,uwuMsg);
    return 1;
} 

int main()
{
    //jeu de donnée
    char mimicData[maxSizeInputData] = "(0,0,2)(1,0,0)(2,0,0)(3,0,0)(4,0,0)(5,0,0)(6,0,0)(7,0,0)(8,0,0)(9,0,0)(0,1,0)(1,1,0)(2,1,0)(3,1,5)(4,1,0)(5,1,0)(6,1,0)(7,1,0)(8,1,0)(9,1,0)(0,2,0)(1,2,0)(2,2,0)(3,2,0)(4,2,3)(5,2,0)(6,2,0)(7,2,0)(8,2,0)(9,2,0)(0,3,0)(1,3,0)(2,3,0)(3,3,0)(4,3,0)(5,3,3)(6,3,0)(7,3,0)(8,3,0)(9,3,0)(0,4,0)(1,4,0)(2,4,4)(3,4,0)(4,4,0)(5,4,0)(6,4,0)(7,4,0)(8,4,0)(9,4,0)(0,5,0)(1,5,0)(2,5,0)(3,5,0)(4,5,0)(5,5,4)(6,5,0)(7,5,0)(8,5,0)(9,5,0)";
    
    //on divise les donnees
    for(int i = 0; i < maxUwuBodySize ; i++ )
        uwuSplitData[0][i] = mimicData[i];
    for(int i = 0 ; i + maxUwuBodySize < maxSizeInputData ; i++ )
        uwuSplitData[1][i] = mimicData[i + maxUwuBodySize];

    toUwuStringFormat(uwuSplitData[0],1);
    toUwuStringFormat(uwuSplitData[1],0);

    uBit.init();
    uBit.radio.setGroup(7);
    uBit.radio.enable();

    while(1)
    {
        uBit.radio.datagram.send(uwuSplitData[0]);
        uBit.radio.datagram.send(uwuSplitData[1]);
        uBit.sleep(10000);
    }
}