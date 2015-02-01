#ifndef SIMULATEDCHOPPER_H
#define SIMULATEDCHOPPER_H

#include <random>

class CSimulatedChopper : public AbstractChopper
{
public:
    CSimulatedChopper( int secondsUpdate, IChopperMessages& msgSink );
    virtual ~CSimulatedChopper();

    virtual void SendPing();
    virtual void SendSimpleCommand(const char* szCommand, int value);
    virtual void SendCommand(const char* szCommand);
    virtual void SendCommand(const char* szCommand, bool toggle);
    virtual bool ProcessData();
private:
    int _secondsUpdate = 2;
    IChopperMessages& _msgSink;

    random_device _rd;
    mt19937 _gen;//(rd());
};

#endif // SIMULATEDCHOPPER_H
