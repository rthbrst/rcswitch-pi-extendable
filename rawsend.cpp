/*
 Usage: ./rawsend timing1 timing2 ...
 */

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
  // This pin is not the first pin on the RPi GPIO header!
  // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
  // for more information.
  int PIN = 0;

  // Parse the first tow parameters to this command as integers
  int count = argc - 1;
  int timings[count];
  for(int i = 0; i < count; i++) {
    timings[i] = atoi(argv[i + 1]);
  }
  
  if (wiringPiSetup () == -1) return 1;

  RCSwitch mySwitch = RCSwitch();
  mySwitch.enableTransmit(PIN);

  mySwitch.sendRaw(count, timings);

  return 0;
}
