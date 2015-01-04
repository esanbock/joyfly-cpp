//
//  choppercontrol.h
//  joyfly-cpp
//
//  Created by esanbockd on 1/3/15.
//  Copyright (c) 2015 Very Unorganized. All rights reserved.
//

#ifndef joyfly_cpp_choppercontrol_h
#define joyfly_cpp_choppercontrol_h

using namespace std;

class ChopperControl
{
private:
    SerialPort& _serialPort;
    int _lastPingNum;
    clock_t _sentPingClock;
public:
    ChopperControl(SerialPort& serialPort);
    void ProcessPingResponse( string& line );
    void SendPing();
    void ProcessCommandResponse( string& line );
    void SendSimpleCommand(const char* szCommand, int value);
    void SendCommand(const char* szCommand);
    void SendCommand(const char* szCommand, bool toggle);
};

#endif
