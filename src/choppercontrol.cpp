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

#include "SerialStream.h"
#include "SerialPort.h"
#include "choppercontrol.h"

using namespace std;

ChopperControl::ChopperControl(SerialPort& serialPort, int secondsUpdate, IChopperMessages& msgSink)
    :_serialPort(serialPort), _msgSink(msgSink)
{
    _lastPingNum = 3;
    _sentPingClock = clock();
    _secondsUpdate = secondsUpdate;
	_lastTime = clock();
}


ChopperControl::~ChopperControl()
{
    _serialPort.Close();
}

void ChopperControl::SendPing()
{
    _sentPingClock = clock();
    _lastPingNum++;
    if( _lastPingNum > 999 )
        _lastPingNum = 1;
    
    stringstream sstream;
    sstream << ":E" << setfill('0') << setw(3) << _lastPingNum;
    string command = sstream.str();
    _msgSink.Sent(command.c_str());
    _serialPort.Write( command );
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
        _msgSink.OnVoltageChange(100 * (1023 / stoi(line.substr(2,4))));
    }

    // otherwise, NAK
    //cout << "unrecognized command from chopper" << endl;
    _msgSink.OnMessage(line.c_str());
}

void ChopperControl::SendSimpleCommand(const char* szCommand, int value)
{
    stringstream sstream;
    sstream << szCommand << setfill('0') << setw(3) << value;
    string command = sstream.str();
    _msgSink.Sent(command.c_str());
    _serialPort.Write( command );
}

void ChopperControl::SendCommand(const char* szCommand)
{
    stringstream sstream;
    sstream << szCommand;
    string command = sstream.str();
    _msgSink.Sent(command.c_str());
    _serialPort.Write( command );
}

void ChopperControl::SendCommand(const char* szCommand, bool toggle)
{
    stringstream sstream;
    sstream << szCommand << setfill('0') << setw(3) << toggle;
    string command = sstream.str();
    _msgSink.Sent(command.c_str());
    _serialPort.Write( command );

}

bool ChopperControl::ProcessData()
{
    bool haveData = false;
    if( _serialPort.IsDataAvailable() )
    {
        try
        {
            string line = _serialPort.ReadLine( 200 );
            
            if( line[0] == ':' )
                ProcessCommandResponse(line);

            haveData = true;
        }
        catch( SerialPort::ReadTimeout& timeout )
        {
            _msgSink.OnDebug("I got nothing");
        }
    }
    
    clock_t now = clock();
    
    if( (now - _lastTime) >= (CLOCKS_PER_SEC * _secondsUpdate) )
    {
        _lastTime = clock();
        SendPing();
    }

    
    return haveData;
}
