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

private:
    std::thread* _pCommandLoopThread = NULL;
    bool _quitting = false;
    int _secondsUpdate = 2;
    IChopperMessages& _msgSink;

    std::random_device _rd;
    std::mt19937 _gen;//(rd());
};

#endif // SIMULATEDCHOPPER_H
