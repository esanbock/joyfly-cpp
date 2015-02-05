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
class ChopperControl : public AbstractChopper
{
private:
    SerialStream& _serialPort;
    int _secondsUpdate;
    IChopperMessages& _msgSink;
    thread* _pCommandLoopThread = NULL;
public:
    ChopperControl(SerialStream& serialPort, int secondsUpdate, IChopperMessages& msgSink);
    virtual ~ChopperControl();
     virtual void SendCommand(const char* szCommand);
    virtual void Start();
protected:
    void ProcessPingResponse( string& line );
    void ProcessCommandResponse( string& line );
    void ProcessData();
};

#endif
