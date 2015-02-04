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
    boost::asio::serial_port& _serialPort;
    clock_t _lastTime;
    int _secondsUpdate;
    IChopperMessages& _msgSink;
public:
    ChopperControl(boost::asio::serial_port& serialPort, int secondsUpdate, IChopperMessages& msgSink);
    virtual ~ChopperControl();


     virtual void SendCommand(const char* szCommand);
    virtual bool ProcessData();
protected:
    void ProcessPingResponse( string& line );
    void ProcessCommandResponse( string& line );
};

#endif
