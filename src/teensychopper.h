//
//  choppercontrol.h
//  joyfly-cpp
//
//  Created by esanbockd on 1/3/15.
//  Copyright (c) 2015 Very Unorganized. All rights reserved.
//

#ifndef joyfly_cpp_choppercontrol_h
#define joyfly_cpp_choppercontrol_h
#include <mutex>

using namespace std;

// this is the model class
class TeensyChopper : public AbstractChopper
{
private:
    const float IMU_MINXY = 300;
    const float IMU_MAXXY = 700;
    const int IMU_MINZ = IMU_MINXY;
    const int IMU_MAXZ = IMU_MAXXY;
    int _seenMinXYVolts = 400;
    int _seenMaxXYVolts = 500;
    int _seenMinZVolts = 400;
    int _seenMaxZVolts = 500;

    SerialStream& _serialPort;
    int _secondsUpdate;
    IChopperMessages& _msgSink;
    thread* _pCommandLoopThread = NULL;
    mutex _mtxComm;

public:
    TeensyChopper(SerialStream& serialPort, int secondsUpdate, IChopperMessages& msgSink);
    virtual ~TeensyChopper();
     virtual void SendCommand(const char* szCommand);
    virtual void Start();
protected:
    void ProcessPingResponse( string& line );
    void ProcessCommandResponse( string& line );
    void ProcessCollective( string& line);
    void ProcessData();
    void ProcessStatusResponse( string& line );
    float IMUVoltageToAngleXY(int volts);
    float IMUVoltageToAngleZ(int volts);
};

#endif
