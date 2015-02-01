//
//  choppercontrol.h
//  joyfly-cpp
//
//  Created by esanbockd on 1/3/15.
//  Copyright (c) 2015 Very Unorganized. All rights reserved.
//

#ifndef joyfly_cpp_choppercontrol_h
#define joyfly_cpp_choppercontrol_h

#include "SerialStream.h"
#include "SerialPort.h"

using namespace std;

class AbstractChopper
{
public:
    virtual void SendPing()=0;
    virtual void SendSimpleCommand(const char* szCommand, int value)=0;
    virtual void SendCommand(const char* szCommand)=0;
    virtual void SendCommand(const char* szCommand, bool toggle)=0;
    virtual bool ProcessData()=0;
    virtual ~AbstractChopper(){}
};

// this is the model class
class ChopperControl : public AbstractChopper
{
private:
    SerialPort& _serialPort;
    int _lastPingNum;
    clock_t _sentPingClock;
    clock_t _lastTime;
    int _secondsUpdate;
public:
    ChopperControl(SerialPort& serialPort, int secondsUpdate);
    virtual ~ChopperControl();

    virtual void SendPing();
    virtual void SendSimpleCommand(const char* szCommand, int value);
    virtual void SendCommand(const char* szCommand);
    virtual void SendCommand(const char* szCommand, bool toggle);
    virtual bool ProcessData();
protected:
    void ProcessPingResponse( string& line );
    void ProcessCommandResponse( string& line );
};

#endif
