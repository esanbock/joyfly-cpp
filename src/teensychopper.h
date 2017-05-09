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
    int _secondsUpdate;
    thread* _pCommandLoopThread = NULL;
    mutex _mtxComm;
    const float IMU_MINXY = 300;
    const float IMU_MAXXY = 700;
    const int IMU_MINZ = IMU_MINXY;
    const int IMU_MAXZ = IMU_MAXXY;
    int _seenMinXYVolts = 400;
    int _seenMaxXYVolts = 500;
    int _seenMinZVolts = 400;
    int _seenMaxZVolts = 500;
    iostream& _serialPort;
    IChopperMessages& _msgSink;

public:
    TeensyChopper(iostream& serialPort, int secondsUpdate, IChopperMessages& msgSink);
    virtual ~TeensyChopper();
    virtual void Start();
    virtual void SetHome();
    virtual void Roll(int val);
    virtual void EnableAutopilot(bool enable);
    virtual void GetStatus();
    virtual void GetVoltage();
    virtual void SetThrottle(int val);
    virtual void Lift(int val);
    virtual void Yaw(int val);
    virtual void Pitch(int val);
    virtual void ChangePid( int pidNum, float kP, float kI, float kD );
    float IMUVoltageToAngleXY(const int volts);
    float IMUVoltageToAngleZ(const int volts);

protected:
    virtual void SendCommand(const char* szCommand);
    void ProcessPingResponse( const string line );
    void ProcessCommandResponse( const string line );
    void ProcessCollective( const string line);
    void ProcessData();
    void ProcessStatusResponse( const string line );
    void ProcessNewHeading( const string line );
    bool ExtractXYZ( const string line, int& x, int& y, int& z);
    void ProcessMotorChange( const string line );
};

#endif
