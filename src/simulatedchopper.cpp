#include <sstream>
#include "choppercontrol.h"
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


void CSimulatedChopper::SendPing()
{

}

void CSimulatedChopper::SendSimpleCommand(const char* szCommand, int value)
{

}

void CSimulatedChopper::SendCommand(const char* szCommand)
{

}

void CSimulatedChopper::SendCommand(const char* szCommand, bool toggle)
{

}

bool CSimulatedChopper::ProcessData()
{
    std::uniform_real_distribution<> dis(0, 100);

    sleep(_secondsUpdate);
    stringstream sstream;
    sstream << "test data " << dis(_gen);
    _msgSink.OnMessage( sstream.str().c_str() );

    _msgSink.OnVoltageChange( dis(_gen) );
    _msgSink.OnPing(dis(_gen));
    return true;
}
