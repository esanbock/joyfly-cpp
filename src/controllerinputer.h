#ifndef CONTROLLERINPUTER
#define CONTROLLERINPUTER

class IControllerInputer
{
public:
    virtual void DebugMessage( const char* szMsg )=0;
    virtual void SetAutoPilot(bool onOff)=0;
    virtual void SetHome()=0;
    virtual void GetStatus()=0;
    virtual void GetVoltage()=0;
};

#endif // CONTROLLERINPUTER

