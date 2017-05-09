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
#include <algorithm>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include "abstractchopper.h"
#include "teensychopper.h"

using namespace std;
using namespace std::chrono;

TeensyChopper::TeensyChopper(iostream &serialPort, int secondsUpdate, IChopperMessages& msgSink) :
    AbstractChopper(secondsUpdate),
    _serialPort(serialPort),
    _msgSink(msgSink)
{
    _secondsUpdate = secondsUpdate;
}


TeensyChopper::~TeensyChopper()
{
    _quitting = true;
}

void TeensyChopper::Start()
{
    if( _pCommandLoopThread != NULL )
        throw logic_error("start already called");
    AbstractChopper::Start();
    _pCommandLoopThread = new std::thread([this]() {ProcessData();});

}

void TeensyChopper::ProcessPingResponse( const string line )
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


void TeensyChopper::ProcessCommandResponse( const string line )
{
    if( line.compare(0,3,":ER") == 0 )
    {
        ProcessPingResponse( line );
        return;
    }
    
    if( line.compare(0,2,"V=") == 0 )
    {
        try
        {
            int rawVoltage = stoi(line.substr(2,4));
            if( rawVoltage < 0 || rawVoltage > 1023 )
            {
                _msgSink.OnUnparsable(line.c_str());
                return;
            }
            float pctVoltage = (float)rawVoltage / 1023.0;
            _msgSink.OnVoltageChange(pctVoltage * 100);
        }
        catch( invalid_argument& invalid_arg )
        {
            _msgSink.OnMessage( "unable to parse voltage" );
        }
        return;
    }

    if( line.compare(0,3, ":S ") == 0)
    {
        ProcessStatusResponse(line);
        return;
    }
    if( line.compare(0,2, ":C") == 0)
    {
        ProcessCollective(line);
        return;
    }

    if( line.compare(0,3, ":NH") == 0)
    {
        ProcessNewHeading(line);
        return;
    }

    if( line.compare(0,3, ":M ") == 0)
    {
        ProcessMotorChange(line);
        return;
    }

    // otherwise, NAK
    //cout << "unrecognized command from chopper" << endl;
    _msgSink.OnUnparsable( line.c_str() );

}

void TeensyChopper::ProcessCollective( const string line)
{
    try
    {
        double collective = stod(line.substr(2));
        _msgSink.OnCollective(collective);
    }
    catch( invalid_argument& err )
    {
        _msgSink.OnUnparsable(line.c_str());
    }
}



void TeensyChopper::ProcessStatusResponse( const string line)
{
    if( line.length() < 9 )
    {
        _msgSink.OnUnparsable(line.c_str());
        return;
    }

    int x=0;
    int y=0;
    int z=0;

    if( ExtractXYZ(line.substr(3), x, y, z) )
    {
         _msgSink.OnIMUChanged(x, y , z);
    }
}

void TeensyChopper::ProcessNewHeading( const string line )
{
    if( line.length() < 9 )
    {
        _msgSink.OnUnparsable(line.c_str());
        return;
    }

    int x=0;
    int y=0;
    int z=0;
    if( ExtractXYZ(line.substr(4), x, y, z) )
    {
        if( x < IMU_MINXY || x > IMU_MAXXY ||
                y < IMU_MINXY || y > IMU_MAXXY ||
                z < IMU_MINZ || z > IMU_MAXZ )
        {
            _msgSink.OnDebug("Parsing error");
            return;
        }

        _msgSink.OnNewHeading(x,y,z);
    }
}

void TeensyChopper::ProcessMotorChange( const string line )
{
    if( line.length() < 8 )
    {
        _msgSink.OnUnparsable(line.c_str());
        return;
    }

    int x=0;
    int y=0;
    int z=0;
    if( ExtractXYZ(line.substr(3), x, y, z) )
    {

        _msgSink.OnNewMotors(x,y,z);
    }

}


bool TeensyChopper::ExtractXYZ( const string line, int& x, int& y, int& z)
{
    vector<string> result;
    boost::split( result, line, boost::is_any_of(string(",")) );

    if( result.size() < 3 )
    {
        string err = "Parsing error.  Not enough vals in XYZ.  Received:  ";
        _msgSink.OnDebug( (err + line).c_str() );
        return false;
    }

    try
    {
        x = stoi(result[0]);
        y = stoi(result[1]);
        z = stoi(result[2]);
    }
    catch( std::invalid_argument& err )
    {
        // mucho error
        _msgSink.OnDebug("Parsing error. Bad data in XYZ");
        return false;
    }

    return true;
}

float TeensyChopper::IMUVoltageToAngleXY(const int volts)
{
    _seenMinXYVolts = min(_seenMinXYVolts, volts);
    _seenMaxXYVolts = max(_seenMaxXYVolts, volts);
    float fAngle = ((float)volts - _seenMinXYVolts ) * (180/(_seenMaxXYVolts-_seenMinXYVolts));
    return 180 - fAngle;
}

float TeensyChopper::IMUVoltageToAngleZ(const int volts)
{
    _seenMinZVolts = min(_seenMinZVolts, volts);
    _seenMaxZVolts = max(_seenMaxZVolts, volts);
    float fAngle = ((float)volts - _seenMinZVolts) * (360/(_seenMaxZVolts-_seenMinZVolts));
    return fAngle;
}


void TeensyChopper::SendCommand(const char* szCommand)
{
    _mtxComm.lock();

    _serialPort << szCommand << endl;
    _serialPort.flush();
    cout << szCommand << endl;

    _msgSink.Sent(szCommand);

    _mtxComm.unlock();
}


void TeensyChopper::ProcessData()
{
    while(!_quitting)
    {
        string line;
        try
        {
            std::getline( _serialPort, line );
            //cout << line;
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

void TeensyChopper::SetHome()
{
    SendCommand(":H");
}

void TeensyChopper::Roll(int val)
{
    SendSimpleCommand(":B",  val);
}

void TeensyChopper::EnableAutopilot(bool enabled)
{
    AbstractChopper::SendCommand(":N", enabled);
}

void TeensyChopper::GetStatus()
{
    SendCommand(":S");
}
void TeensyChopper::GetVoltage()
{
    SendCommand(":V");
}

void TeensyChopper::Pitch(int val)
{
    SendSimpleCommand(":P",  val);
}

void TeensyChopper::Yaw(int val)
{
    SendSimpleCommand(":Y",  val);
}


void TeensyChopper::Lift(int val)
{
    SendSimpleCommand(":L",  val);
}

void TeensyChopper::SetThrottle(int val)
{
    SendSimpleCommand(":T",  val);
}

void TeensyChopper::ChangePid( int pidNum, float kP, float kI, float kD )
{
    stringstream sstream;
    sstream << ":Q" << pidNum << kP << "," << kI << "," << kD;

    SendCommand(sstream.str().c_str());
}
