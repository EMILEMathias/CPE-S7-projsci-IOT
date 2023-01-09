#include "MicroBit.h"
MicroBit uBit;

int key[5][5] = {
    {39,118,118,158,20},
    {237,197,197,177,118},
    {136,158,157,40,198},
    {3,0,1,255,254},
    {57,177,177,237,159}
};

//decrypte les données
ManagedString decrypt(ManagedString s, int inv[][5])
{
    ManagedString d("");
    int k = 0;
    int input[5];
    while (k < s.length())
    {
        input[0] = s.charAt(k);
        k++;
        input[1] = s.charAt(k);
        k++;
        input[2] = s.charAt(k);
        k++;
        input[3] = s.charAt(k);
        k++;
        input[4] = s.charAt(k);
        k++;
        for (int i = 0; i < 5; i++)
        {
            int decipher = 0;
            for (int j = 0; j < 5; j++) {
                decipher += inv[i][j] * input[j];
            }
            d = ManagedString(d+(char)((decipher % 256)));
        }
    }
    return d;
}

//receptionne les données RF et envoie en serial
void onData(MicroBitEvent e)
{
    PacketBuffer radioData = uBit.radio.datagram.recv();
    uBit.display.scroll("B2",75);
    uBit.serial.send(decrypt(ManagedString(radioData).substring(0,ManagedString(radioData).length()-1),key) + ManagedString(radioData).charAt(ManagedString(radioData).length()-1));
    // uBit.serial.send(ManagedString(radioData));
}

//setup RF and serial
int main()
{
    uBit.init();
    uBit.radio.setGroup(7);
    uBit.radio.enable();
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);

    while(1){
        uBit.sleep(1000);
    }
}