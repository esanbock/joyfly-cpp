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
    float latency = (float)(clock() - _sentPingClock ) / CLOCKS_PER_SEC ;
    _msgSink.OnPing(latency);
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



void ChopperControl::SendCommand(const char* szCommand)
{
    _serialPort << szCommand << endl;
    _msgSink.Sent(szCommand);
}



void ChopperControl::ProcessData()
{
    while(!_quitting)
    {
        string line;
        _serialPort >> line;

        ProcessCommandResponse(line);
    }
}


