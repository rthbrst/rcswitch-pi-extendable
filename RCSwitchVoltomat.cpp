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

void RCSwitchVoltomat::sendSync() {
  RCSwitch::sendSync();
}

/**
 * |__________
 */
void RCSwitchVoltomat::sendPreSync() {
  if(RCSwitch::nProtocol == PROTO_NUMBER) {
    this->transmit(1,10);
  } else {
    RCSwitch::sendPreSync();
  }
}
