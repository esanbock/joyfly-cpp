//
//  choppercontrol.cpp
//  joyfly-cpp
//
//  Created by esanbockd on 1/3/15.
//  Copyright (c) 2015 Very Unorganized. All rights reserved.
//

#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>

#include "serialstream.h"
#include "abstractchopper.h"
#include "choppercontrol.h"

using namespace std;
using namespace std::chrono;

ChopperControl::ChopperControl(SerialStream &serialPort, int secondsUpdate, IChopperMessages& msgSink)
    :_serialPort(serialPort), _msgSink(msgSink), AbstractChopper(secondsUpdate)
{
    _secondsUpdate = secondsUpdate;
}


ChopperControl::~ChopperControl()
{
    _quitting = true;
    _serialPort.close();
}

void ChopperControl::Start()
{
    if( _pCommandLoopThread != NULL )
        throw logic_error("start already called");
    AbstractChopper::Start();
    _pCommandLoopThread = new std::thread([this]() {ProcessData();});

}

void ChopperControl::ProcessPingResponse( string& line )
{
    int pingResponse = atoi( line.substr(3).c_str() );
    if( pingResponse != _lastPingNum )
    {
        stringstream sstream;
        sstream << "unmatched ping " << pingResponse << ".  Expected " << _lastPingNum << endl;
        _msgSink.OnDebug(sstream.str().c_str());
        return;
    }
    system_clock::time_point now = system_clock::now();
    auto latency = now - _sentPingClock;

    _msgSink.OnPing(chrono::duration_cast<milliseconds>(latency).count());
}


void ChopperControl::ProcessCommandResponse( string& line )
{
    if( line.compare(0,3,":ER") == 0 )
    {
        ProcessPingResponse( line );
        return;
    }
    
    if( line.compare(0,2,"V=") == 0 )
    {
        int rawVoltage = stoi(line.substr(2,4));
        float pctVoltage = (float)rawVoltage / 1023.0;
        _msgSink.OnVoltageChange(pctVoltage * 100);
    }

    // otherwise, NAK
    //cout << "unrecognized command from chopper" << endl;
    _msgSink.OnMessage(line.c_str());
}


static mutex g_write_mutex;
void ChopperControl::SendCommand(const char* szCommand)
{
    lock_guard<std::mutex> lock(g_write_mutex); // unlocks when out of scope
    _serialPort << szCommand << endl;
    _msgSink.Sent(szCommand);
}



void ChopperControl::ProcessData()
{
    while(!_quitting)
    {
        string line;
        try
        {
            std::getline( _serialPort, line );
            //_serialPort >> line;
        }
        catch(exception& err)
        {
            _msgSink.OnDebug(err.what());
            _quitting = true;
            return;
        }

        ProcessCommandResponse(line);
    }
}


