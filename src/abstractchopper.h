#ifndef ABSTRACTCHOPPER_H
#define ABSTRACTCHOPPER_H

class IChopperMessages
{
public:
    virtual void OnMessage(const char* data)=0;
    virtual void OnDebug(const char* data)=0;
    virtual void OnVoltageChange(float newVoltage)=0;
    virtual void OnPing(float latency)=0;
    virtual void Sent(const char* szMsg)=0;
};

class AbstractChopper
{
private:

protected:
    AbstractChopper();
    int _lastPingNum;
    clock_t _sentPingClock;

public:
    virtual void SendPing();
    virtual void SendSimpleCommand(const char* szCommand, int value);
    virtual void SendCommand(const char* szCommand)=0;
    virtual void SendCommand(const char* szCommand, bool toggle);
    virtual bool ProcessData()=0;
    virtual ~AbstractChopper();
};

#endif // ABSTRACTCHOPPER_H
