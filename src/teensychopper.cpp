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
#include "teensychopper.h"

using namespace std;
using namespace std::chrono;

TeensyChopper::TeensyChopper(SerialStream &serialPort, int secondsUpdate, IChopperMessages& msgSink)
    :_serialPort(serialPort), _msgSink(msgSink), AbstractChopper(secondsUpdate)
{
    _secondsUpdate = secondsUpdate;
}


TeensyChopper::~TeensyChopper()
{
    _quitting = true;
    _serialPort.close();
}

void TeensyChopper::Start()
{
    if( _pCommandLoopThread != NULL )
        throw logic_error("start already called");
    AbstractChopper::Start();
    _pCommandLoopThread = new std::thread([this]() {ProcessData();});

}

void TeensyChopper::ProcessPingResponse( string& line )
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


void TeensyChopper::ProcessCommandResponse( string& line )
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

    if( line.compare(0,3, ":S ") == 0)
    {
        ProcessStatusResponse(line);
    }

    // otherwise, NAK
    //cout << "unrecognized command from chopper" << endl;
    _msgSink.OnMessage(line.c_str());
}

void TeensyChopper::ProcessStatusResponse( string& line)
{
    int x=0;
    int y=0;
    int z=0;

    stringstream ss(line);

    ss.ignore(3);

    while( ss.gcount() > 0 )
    {
        char field[3];
        ss.get(field, 2);
        field[2] = 0;
        switch(field[0])
        {
        case 'x':
            ss.ignore(1);
            ss >> x >> skipws;
            break;
        case 'y':
            ss.ignore(1);
            ss >> y >> skipws;
            break;
        case 'z':
            ss.ignore(1);
            ss >> z >> skipws;
            break;
        }
    }
    if( x < IMU_XMIN || x > IMU_XMAX ||
            y < IMU_YMIN || y > IMU_YMAX ||
            z < IMU_ZMIN || z > IMU_ZMAX )
    {
        _msgSink.OnDebug("Parsing error");
    }
    else
    {
        _msgSink.OnIMUChanged(x, y , z);
    }
}



static mutex g_write_mutex;
void TeensyChopper::SendCommand(const char* szCommand)
{
    lock_guard<std::mutex> lock(g_write_mutex); // unlocks when out of scope
    _serialPort << szCommand << endl;
    _msgSink.Sent(szCommand);
}



void TeensyChopper::ProcessData()
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


