#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include "abstractchopper.h"

using namespace std;

AbstractChopper::AbstractChopper()
{
    _lastPingNum = 3;
    _sentPingClock = clock();

}

AbstractChopper::~AbstractChopper()
{

}

void AbstractChopper::SendPing()
{
    _sentPingClock = clock();
    _lastPingNum++;
    if( _lastPingNum > 999 )
        _lastPingNum = 1;

    stringstream sstream;
    sstream << ":E" << setfill('0') << setw(3) << _lastPingNum;
    string command = sstream.str();
    SendCommand(command.c_str());
}

void AbstractChopper::SendCommand(const char* szCommand, bool toggle)
{
    stringstream sstream;
    sstream << szCommand << setfill('0') << setw(3) << toggle;
    string command = sstream.str();
    SendCommand(command.c_str());
}

void AbstractChopper::SendSimpleCommand(const char* szCommand, int value)
{
    stringstream sstream;
    sstream << szCommand << setfill('0') << setw(3) << value;
    string command = sstream.str();
    SendCommand( command.c_str() );
}
