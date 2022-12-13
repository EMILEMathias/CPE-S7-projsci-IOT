#include "MicroBit.h"

MicroBit uBit;

void onData(MicroBitEvent e)
{
    PacketBuffer radioData = uBit.radio.datagram.recv();
    uBit.serial.send(radioData);
}

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