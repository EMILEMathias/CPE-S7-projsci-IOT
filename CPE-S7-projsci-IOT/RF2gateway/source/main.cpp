#include "MicroBit.h"
MicroBit uBit;

int key[2][2] = {
    {51,154},
    {103,51}
};


//decrypte les données
ManagedString decrypt(ManagedString s, int inv[][2])
{
    ManagedString d("");
    int k = 0;
    int input[2];
    while (k < s.length())
    {
        input[0] = s.charAt(k);
        k++;
        input[1] = s.charAt(k);
        k++;
        for (int i = 0; i < 2; i++)
        {
            int decipher = 0;
            for (int j = 0; j < 2; j++) {
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
    uBit.serial.send(decrypt(ManagedString(radioData),key));
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