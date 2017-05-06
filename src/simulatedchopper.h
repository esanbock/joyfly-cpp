#ifndef SIMULATEDCHOPPER_H
#define SIMULATEDCHOPPER_H

#include <random>

class CSimulatedChopper : public AbstractChopper
{
public:
    CSimulatedChopper( int secondsUpdate, IChopperMessages& msgSink );
    virtual ~CSimulatedChopper();

    virtual void SendCommand(const char* szCommand);
    virtual void ProcessData();
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

private:
    std::thread* _pCommandLoopThread = NULL;
    bool _quitting = false;
    int _secondsUpdate = 2;
    IChopperMessages& _msgSink;

    std::random_device _rd;
    std::mt19937 _gen;//(rd());
};

#endif // SIMULATEDCHOPPER_H
