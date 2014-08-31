all: send rawsend rawsniffer sniffer protosend

send: RCSwitch.o send.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi

rawsend: RCSwitch.o rawsend.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi

rawsniffer: RCSwitch.o rawsniffer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi

sniffer: RCSwitch.o RCSwitchVoltomat.o sniffer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi

protosend: RCSwitch.o RCSwitchVoltomat.o protosend.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi

clean:
	$(RM) *.o send rawsend rawsniffer sniffer protosend
