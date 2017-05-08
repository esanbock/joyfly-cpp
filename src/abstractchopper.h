#ifndef ABSTRACTCHOPPER_H
#define ABSTRACTCHOPPER_H

#include <chrono>
#include <thread>

class IChopperMessages
{
public:
    virtual void OnMessage(const char* data)=0;
    virtual void OnDebug(const char* data)=0;
    virtual void OnVoltageChange(float newVoltage)=0;
    virtual void OnPing(float latency)=0;
    virtual void Sent(const char* szMsg)=0;
    virtual void OnIMUChanged( const int x, const int y, const int z )=0;
    virtual void OnUnparsable(const char* data)=0;
    virtual void OnCollective(double collective)=0;
    virtual void OnNewHeading( const int x, const int y, const int z)=0;
    virtual void OnNewMotors( const int x, const int y, const int z)=0;
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
    virtual void SendSimpleCommand(const char* szCommand, int value);
    virtual void SendCommand(const char* szCommand)=0;
    virtual void SendCommand(const char* szCommand, bool toggle);

public:
    virtual ~AbstractChopper();
    virtual void Start();
    virtual void SendPing();

    virtual void SetHome()=0;
    virtual void Roll(int val)=0;
    virtual void EnableAutopilot(bool enable)=0;
    virtual void GetStatus()=0;
    virtual void GetVoltage()=0;
    virtual void SetThrottle(int val)=0;
    virtual void Lift(int val)=0;
    virtual void Yaw(int val)=0;
    virtual void Pitch(int val)=0;
    virtual void ChangePid( double kP, double kI, double kD)=0;

};

#endif // ABSTRACTCHOPPER_H
