/*
  based on RF_Sniffer hacked from http://code.google.com/p/rc-switch/ by @justy
*/

#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  int max = 200;
  int sleep = 1000000;

  if(argc > 1)
    max = atoi(argv[1]);
  if(argc > 2)
    sleep = atoi(argv[2]);

  printf("max changes: %d\n\n", max);
  // This pin is not the first pin on the RPi GPIO header!
  // Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
  // for more information.
  int PIN = 2;

  if(wiringPiSetup() == -1)
    return 0;

  RCSwitch mySwitch = RCSwitch((unsigned int)max);
  mySwitch.enableReceive(PIN);  // Receiver on inerrupt 0 => that is pin #2
     
  unsigned int *timings;
  unsigned int last = 0;
    
  while(1) {
    timings = mySwitch.getReceivedRawdata();

    if(timings[1] != last) {
      last = timings[1];
      for(int i = 1; i < max; i++)
        printf("%i ", timings[i]);
      printf("\n\n");
    }

    usleep(sleep);
  }

  return 0;
}
