#include <unistd.h>
#include <sstream>
#include <thread>
#include "abstractchopper.h"
#include "simulatedchopper.h"

using namespace std;

CSimulatedChopper::CSimulatedChopper(int secondsUpdate, IChopperMessages &msgSink )
    :_msgSink(msgSink), AbstractChopper(secondsUpdate)
{
    _secondsUpdate = secondsUpdate;
}

CSimulatedChopper::~CSimulatedChopper()
{

}

void CSimulatedChopper::SendCommand(const char* szCommand)
{
    _msgSink.Sent(szCommand);
}

void CSimulatedChopper::Start()
{
    _pCommandLoopThread = new std::thread([this]() {ProcessData();});

}


void CSimulatedChopper::ProcessData()
{
    while (!_quitting)
    {
        std::uniform_real_distribution<> dis(0, 100);
        std::uniform_int_distribution<> imuRand(0,1023);
        sleep(_secondsUpdate);
        SendPing();

        _msgSink.OnVoltageChange( dis(_gen) );
        _msgSink.OnPing(dis(_gen));
        _msgSink.OnMessage("hi");
        _msgSink.OnIMUChanged(imuRand(_gen), imuRand(_gen), imuRand(_gen));
    }
}
