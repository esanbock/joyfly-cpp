#ifndef SIMULATEDCHOPPER_H
#define SIMULATEDCHOPPER_H

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
};

#endif // SIMULATEDCHOPPER_H
