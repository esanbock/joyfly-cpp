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

// this is the model class
class ChopperControl : public AbstractChopper
{
private:
    SerialPort& _serialPort;
    clock_t _lastTime;
    int _secondsUpdate;
    IChopperMessages& _msgSink;
public:
    ChopperControl(SerialPort& serialPort, int secondsUpdate, IChopperMessages& msgSink);
    virtual ~ChopperControl();


    virtual void SendSimpleCommand(const char* szCommand, int value);
    virtual void SendCommand(const char* szCommand);
    virtual bool ProcessData();
protected:
    void ProcessPingResponse( string& line );
    void ProcessCommandResponse( string& line );
};

#endif
