/**
 * This tool transmits signals (given by their decimal
 * representation) with the specified protocol.
 *
 * Usage: protosend protocol_number decimal_code
 *
 * Hint: Currently only protocols 1 and 100 are implemented in
 *       this tool.
 *
 * 'sniffer' command can be used to record signals from a remote
 * control. It will show protocol number and the decimal
 * representation of the signal. See sniffer.cpp for details.
 */

#include "RCSwitchVoltomat.h"
#include <stdlib.h>
#include <stdio.h>
     

int main(int argc, char *argv[]) {
  // This pin is not the first pin on the RPi GPIO header!
  // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
  // for more information.
  int PIN = 0;

  if(argc < 3) {
    printf("Usage: protosend protocol_number decimal_code\n");
    return -1;
  }
  
  // Parse the first tow parameters to this command as integers
  int protocol = atoi(argv[1]);
  int code = atoi(argv[2]);
  
  if (wiringPiSetup () == -1) return 1;

  RCSwitchVoltomat mySwitch = RCSwitchVoltomat();
  mySwitch.enableTransmit(PIN);

  if(protocol == 1) {
    printf("sending code[%i] with protocol 1\n", code);
    mySwitch.setProtocol(1);
    mySwitch.setRepeatTransmit(20);
    mySwitch.send(code, 24);
  }
  else if(protocol == 100) {
    printf("sending code[%i] with protocol 100\n", code);
    mySwitch.setProtocol(100);
    mySwitch.send(code, 32);
  }
  else {
    printf("no protocol specified!\n");
  }

	return 0;
}
