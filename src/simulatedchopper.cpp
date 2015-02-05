#include <unistd.h>
#include <sstream>
#include "abstractchopper.h"
#include "simulatedchopper.h"

using namespace std;

CSimulatedChopper::CSimulatedChopper(int secondsUpdate, IChopperMessages &msgSink )
    :_msgSink(msgSink)
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

bool CSimulatedChopper::ProcessData()
{
    std::uniform_real_distribution<> dis(0, 100);
    std::uniform_int_distribution<> imuRand(0,1023);
    sleep(_secondsUpdate);
    SendPing();

    _msgSink.OnVoltageChange( dis(_gen) );
    _msgSink.OnPing(dis(_gen));
    _msgSink.OnMessage("hi");
    _msgSink.OnIMUChanged(imuRand(_gen), imuRand(_gen), imuRand(_gen));
    return true;
}
