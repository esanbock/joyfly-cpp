#include "choppercontrol.h"
#include "simulatedchopper.h"

CSimulatedChopper::CSimulatedChopper( int secondsUpdate )
{
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
    sleep(1);
}
