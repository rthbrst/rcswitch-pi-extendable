/*
  based on RF_Sniffer by @justy to provide a handy RF code sniffer
  edited by @rthbrst to be used with extended version of RCSwitch

  This tool records and displays received RC switch commands.

  As some protocols use similar binary representations, the signals
  will only be shown as a numerical representation (plus protocol
  number of course). This means, this tool will also not distinguish
  between ON and OFF or specific vendors of RC switches.
  'protosend' command can be used to transmit these signals again by
  specifying protocol number and decimal signal representation (see
  protosend.cpp for details).
*/

#include "RCSwitchVoltomat.h"
//#include "RCSwitch.h"  // might also be used with RCSwitch w/o extended protocols

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <bitset>


RCSwitchVoltomat mySwitch;
//RCSwitch mySwitch;

int main(int argc, char *argv[]) {  
  // This pin is not the first pin on the RPi GPIO header!
  // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
  // for more information.
  int PIN = 2;

  if(wiringPiSetup() == -1)
    return 0;

  mySwitch = RCSwitchVoltomat();
  //mySwitch = RCSwitch();

  mySwitch.enableReceive(PIN);  // Receiver on inerrupt 0 => that is pin #2

  while(1) {
    if (mySwitch.available()) {
      int value = mySwitch.getReceivedValue();
      if (value == 0) {
        printf("Received signal: unknown encoding.\n\n");
      } else {
        printf("Received signal:\n  protocol %d (bitlength %d)\n", mySwitch.getReceivedProtocol(), mySwitch.getReceivedBitlength());
        printf("  binary:  %s\n", RCSwitch::dec2binWzerofill(value, mySwitch.getReceivedBitlength()));
        printf("  decimal: %d\n", value);
        printf("  hex:     %x\n\n", value);
      }

      mySwitch.resetAvailable();
    }
  }

  return 0;
}
