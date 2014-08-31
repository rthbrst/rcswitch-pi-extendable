#ifndef _RCSwitchVoltomat_h
#define _RCSwitchVoltomat_h

#include "RCSwitch.h"

class RCSwitchVoltomat : public RCSwitch {
  public:
    RCSwitchVoltomat();

    void setProtocol(int nProtocol);

  protected:
    static bool receiveMyProtocol(unsigned int changeCount);
    void send0();
    void send1();
    void sendSync();
    void sendPreSync();
};

#endif
