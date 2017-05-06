#include <unistd.h>
#include <sstream>
#include <thread>
#include "abstractchopper.h"
#include "simulatedchopper.h"

using namespace std;

CSimulatedChopper::CSimulatedChopper(int secondsUpdate, IChopperMessages &msgSink )
    :AbstractChopper(secondsUpdate), _msgSink(msgSink)
{
    _secondsUpdate = secondsUpdate;
}

CSimulatedChopper::~CSimulatedChopper()
{
    _quitting = true;
    if( _pCommandLoopThread != NULL )
    {
        _pCommandLoopThread->join();

        delete _pCommandLoopThread;
    }
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
        std::uniform_int_distribution<> imuRand(0,180);
        std::uniform_int_distribution<> compassRand(0,360);
        sleep(_secondsUpdate);
        SendPing();

        _msgSink.OnVoltageChange( dis(_gen) );
        _msgSink.OnPing(dis(_gen));
        _msgSink.OnMessage("hi");
        _msgSink.OnIMUChanged(imuRand(_gen), imuRand(_gen), compassRand(_gen));
    }
}

void CSimulatedChopper::SetHome()
{
    std::uniform_int_distribution<> imuRand(0,180);
    std::uniform_int_distribution<> compassRand(0,360);
    _msgSink.OnNewHeading(imuRand(_gen), imuRand(_gen), compassRand(_gen));
}
