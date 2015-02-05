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
    virtual void OnIMUChanged( int x, int y, int z )=0;
};

class AbstractChopper
{
private:
    std::chrono::system_clock::time_point _lastTime;
    std::chrono::seconds _pingerSeconds;

protected:
    AbstractChopper( int pingerSeconds );
    int _lastPingNum;
    clock_t _sentPingClock;
    std::thread* _pPingThread = NULL;
    virtual void PingThread();
    bool _quitting = false;


public:
    virtual ~AbstractChopper();
    virtual void Start();
    virtual void SendPing();
    virtual void SendSimpleCommand(const char* szCommand, int value);
    virtual void SendCommand(const char* szCommand)=0;
    virtual void SendCommand(const char* szCommand, bool toggle);
};

#endif // ABSTRACTCHOPPER_H
