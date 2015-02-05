#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <thread>

#include "abstractchopper.h"

using namespace std;
using namespace std::chrono;

AbstractChopper::AbstractChopper(int pingerSeconds)
{
    _pingerSeconds = chrono::seconds(pingerSeconds);
    _lastPingNum = 3;
    _lastTime = std::chrono::system_clock::now();

}

AbstractChopper::~AbstractChopper()
{
    _quitting = true;
}

void AbstractChopper::Start()
{
    _pPingThread = new std::thread([this]() {PingThread();});

}

void AbstractChopper::PingThread()
{
    while(!_quitting)
    {
        system_clock::time_point now = system_clock::now();
        auto timeLeft = now - _lastTime;

        if( timeLeft >= chrono::seconds(_pingerSeconds) )
        {
            _lastTime = system_clock::now();
            SendPing();
        }
        else
        {
            this_thread::sleep_for(timeLeft);
        }
    }
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
