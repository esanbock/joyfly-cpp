#ifndef CONTROLLERINPUTER
#define CONTROLLERINPUTER

#include "timeseries.h"

class IJoyflyController
{
public:
    virtual void DebugMessage( const char* szMsg )=0;
    virtual void ToggleAutoPilot()=0;
    virtual void SetAutoPilot( bool isOn )=0;
    virtual void SetHome()=0;
    virtual void GetStatus()=0;
    virtual void GetVoltage()=0;
    virtual void Bank(int val)=0;
    virtual void Pitch(int val)=0;
    virtual void Yaw(int val)=0;
    virtual void SetThrottle(int val)=0;
    virtual void Lift(int val)=0;
    virtual int Connect(const std::string serialDevice, int secondsUpdate)=0;
    virtual void AddJoyStick(int joyNum)=0;
    virtual TimeSeries<double,double>* GetVoltageHistory()=0;
};

#endif // CONTROLLERINPUTER

