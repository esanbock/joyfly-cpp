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

// this is the model class
class ChopperControl
{
private:
    SerialPort& _serialPort;
    int _lastPingNum;
    clock_t _sentPingClock;
    clock_t _lastTime;
    int _secondsUpdate;
public:
    ChopperControl(SerialPort& serialPort, int secondsUpdate);
    void ProcessPingResponse( string& line );
    void SendPing();
    void SendSimpleCommand(const char* szCommand, int value);
    void SendCommand(const char* szCommand);
    void SendCommand(const char* szCommand, bool toggle);
    bool ProcessData();
protected:
    void ProcessCommandResponse( string& line );
};

#endif
