#include "MicroBit.h"

MicroBit uBit;

//receptionne les données RF et envoie en serial
void onData(MicroBitEvent e)
{
    PacketBuffer radioData = uBit.radio.datagram.recv();
    uBit.display.scroll("B2",75);
    // uBit.display.scroll(ManagedString(radioData),75);
    uBit.serial.send(radioData);
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