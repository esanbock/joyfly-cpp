#ifndef CONTROLLERINPUTER
#define CONTROLLERINPUTER

#include "timeseries.h"
#include "abstractchopper.h"

class IJoyflyController
{
public:
    virtual void DebugMessage( const char* szMsg )=0;
    virtual void ToggleAutoPilot()=0;
    virtual void SetAutoPilot( bool isOn )=0;
    virtual void SetThrottle(int val)=0;
    virtual int Connect(const std::string serialDevice)=0;
    virtual int Connect(const int secondsUpdate)=0;
    virtual void AddJoyStick(int joyNum)=0;
    virtual TimeSeries<double,double>* GetVoltageHistory()=0;
    virtual void GetHeading( int&x, int& y, int &z)=0;
    virtual TimeSeries<double,double>* GetIMUHistory()=0;
    virtual TimeSeries<double,double>* GetHeadingHistory()=0;
    virtual TimeSeries<double,double>* GetMotorHistory()=0;
    virtual AbstractChopper& GetChopper()=0;
};

#endif // CONTROLLERINPUTER

