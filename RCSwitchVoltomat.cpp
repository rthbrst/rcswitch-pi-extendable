/**
 * RCSwitch protocol implementation for cheap self-learning RC
 * switches from Voltomat (sold by Bauhaus, German DIY retailer)
 *
 * After plugging the RC switch in, it will be programmed to use a
 * specific code by sending an ON signal within a short
 * period of time. Signals from original remote controls can be
 * recorded using 'sniffer' command (which does not distinguish
 * between ON and OFF but only displays a numerical representation
 * of the signal).
 *
 * The signal is a 32-bit code:
 *
 * iiiiiiiiiiiiiiiiiiiiiiiiiigsxxdd
 *
 * i: remote control identification
 * x: ? (might also be used for identification)
 * d: switch number (1: 11, 2: 10, 3: 01)
 * g: group (switch all): 0 no, 1 yes (dd is
 *      11 when operating group)
 * s: command (1: ON, 0: OFF)
 *
 *
 * Credits: Created by Leif Rothbrust
 *          post(at)rothbrust(dot)com
 *          https://github.com/rthbrst
 */

#include "RCSwitchVoltomat.h"

RCSwitchVoltomat::RCSwitchVoltomat() : RCSwitch(200) {
  RCSwitch::fReceiveExtendedProtocol = &receiveMyProtocol;
}

void RCSwitchVoltomat::setProtocol(int nProtocol) {
  RCSwitch::setProtocol(nProtocol);
  if (nProtocol == PROTO_NUMBER) {
    this->setPulseLength(260);
  }
}

bool RCSwitchVoltomat::receiveMyProtocol(unsigned int changeCount) {
      unsigned long code = 0;
      unsigned long delay = RCSwitch::timings[0] / 38;
      unsigned long delayTolerance = delay * RCSwitch::nReceiveTolerance * 0.01;  

      for (int i = 3; i<changeCount ; i=i+4) {
        if (RCSwitch::timings[i] > delay-delayTolerance && RCSwitch::timings[i] < delay+delayTolerance &&
            RCSwitch::timings[i+1] > delay-delayTolerance && RCSwitch::timings[i+1] < delay+delayTolerance &&
            RCSwitch::timings[i+2] > delay-delayTolerance && RCSwitch::timings[i+2] < delay+delayTolerance &&
            RCSwitch::timings[i+3] > delay*5-delayTolerance && RCSwitch::timings[i+3] < delay*5+delayTolerance) {
          code = code << 1;
        } else if (RCSwitch::timings[i] > delay-delayTolerance && RCSwitch::timings[i] < delay+delayTolerance &&
            RCSwitch::timings[i+1] > delay*5-delayTolerance && RCSwitch::timings[i+1] < delay*5+delayTolerance &&
            RCSwitch::timings[i+2] > delay-delayTolerance && RCSwitch::timings[i+2] < delay+delayTolerance &&
            RCSwitch::timings[i+3] > delay-delayTolerance && RCSwitch::timings[i+3] < delay+delayTolerance) {
          code+=1;
          code = code << 1;
        } else {
          // Failed
          i = changeCount;
          code = 0;
        }
      }
      code = code >> 1;
    if (changeCount > 6) {    // ignore < 4bit values as there are no devices sending 4bit values => noise
      RCSwitch::nReceivedValue = code;
      RCSwitch::nReceivedBitlength = changeCount / 4;
      RCSwitch::nReceivedDelay = delay;
      RCSwitch::nReceivedProtocol = PROTO_NUMBER;
    }

  if (code == 0){
    return false;
  } else if (code != 0) {
    return true;
  }
}

/**
 * |_|_____
 */
void RCSwitchVoltomat::send0() {
  if(RCSwitch::nProtocol == PROTO_NUMBER) {
    this->transmit(1,1);
    this->transmit(1,5);
  } else {
    RCSwitch::send0();
  }
}

/**
 * |_____|_
 */
void RCSwitchVoltomat::send1() {
  if(RCSwitch::nProtocol == PROTO_NUMBER) {
    this->transmit(1,5);
    this->transmit(1,1);
  } else {
    RCSwitch::send1();
  }
}

/**
 * |__________
 */
void RCSwitchVoltomat::sendSync() {
  if(RCSwitch::nProtocol == PROTO_NUMBER) {
    this->transmit(1,10);
  } else {
    RCSwitch::sendSync();
  }
}
