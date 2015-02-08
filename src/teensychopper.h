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
class TeensyChopper : public AbstractChopper
{
private:

    const float IMU_MAXXY = 775;
    const float IMU_MINXY = 700;

    const int IMU_MINZ = 470;
    const int IMU_MAXZ = 550;

    SerialStream& _serialPort;
    int _secondsUpdate;
    IChopperMessages& _msgSink;
    thread* _pCommandLoopThread = NULL;
public:
    TeensyChopper(SerialStream& serialPort, int secondsUpdate, IChopperMessages& msgSink);
    virtual ~TeensyChopper();
     virtual void SendCommand(const char* szCommand);
    virtual void Start();
protected:
    void ProcessPingResponse( string& line );
    void ProcessCommandResponse( string& line );
    void ProcessData();
    void ProcessStatusResponse( string& line );
    float IMUVoltageToAngleXY(int volts);
    float IMUVoltageToAngleZ(int volts);
};

#endif
