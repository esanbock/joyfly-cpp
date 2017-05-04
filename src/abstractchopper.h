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
    virtual void OnIMUChanged( float x, float y, float z )=0;
    virtual void OnUnparsable(const char* data)=0;
    virtual void OnCollective(double collective)=0;
};

class AbstractChopper
{
private:
    std::chrono::system_clock::time_point _lastTime;
    std::chrono::seconds _pingerSeconds;

protected:
    AbstractChopper( int pingerSeconds );
    int _lastPingNum;
    std::thread* _pPingThread = NULL;
    virtual void PingThread();
    bool _quitting = false;
    std::chrono::system_clock::time_point _sentPingClock;

public:
    virtual ~AbstractChopper();
    virtual void Start();
    virtual void SendPing();
    virtual void SendSimpleCommand(const char* szCommand, int value);
    virtual void SendCommand(const char* szCommand)=0;
    virtual void SendCommand(const char* szCommand, bool toggle);
};

#endif // ABSTRACTCHOPPER_H
