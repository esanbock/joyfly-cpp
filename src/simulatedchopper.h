#ifndef SIMULATEDCHOPPER_H
#define SIMULATEDCHOPPER_H

#include <random>

class CSimulatedChopper : public AbstractChopper
{
public:
    CSimulatedChopper( int secondsUpdate, IChopperMessages& msgSink );
    virtual ~CSimulatedChopper();

    virtual void SendCommand(const char* szCommand);
    virtual bool ProcessData();
private:
    int _secondsUpdate = 2;
    IChopperMessages& _msgSink;

    std::random_device _rd;
    std::mt19937 _gen;//(rd());
};

#endif // SIMULATEDCHOPPER_H
