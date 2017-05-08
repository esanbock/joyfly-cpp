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

    // otherwise, NAK
    //cout << "unrecognized command from chopper" << endl;
    _msgSink.OnUnparsable( line.c_str() );

}

void TeensyChopper::ProcessCollective( const string line)
{
    double collective = stod(line.substr(2));
    _msgSink.OnCollective(collective);
}



void TeensyChopper::ProcessStatusResponse( const string line)
{
    int x=0;
    int y=0;
    int z=0;

    if( ExtractXYZ(line.substr(3), x, y, z) )
    {
         _msgSink.OnIMUChanged(IMUVoltageToAngleXY(x), IMUVoltageToAngleXY(y) , IMUVoltageToAngleZ(z));
    }
}

void TeensyChopper::ProcessNewHeading( const string line )
{
    int x=0;
    int y=0;
    int z=0;
    if( ExtractXYZ(line.substr(4), x, y, z) )
    {
        _msgSink.OnNewHeading(x,y,z);
    }
}


bool TeensyChopper::ExtractXYZ( const string line, int& x, int& y, int& z)
{
    stringstream ss(line);

    while( !ss.eof() )
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
        case 't':
            ss.ignore(4);
            break;
        default:
             _msgSink.OnDebug("error parsing xyz");
             return false;
        }
    }
    if( x < IMU_MINXY || x > IMU_MAXXY ||
            y < IMU_MINXY || y > IMU_MAXXY ||
            z < IMU_MINZ || z > IMU_MAXZ )
    {
        _msgSink.OnDebug("Parsing error");
        return false;
    }
    else
    {
        return true;
    }
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

void TeensyChopper::Bank(int val)
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
