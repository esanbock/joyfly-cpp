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
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

#include "abstractchopper.h"
#include "choppercontrol.h"

using namespace std;

ChopperControl::ChopperControl(boost::asio::serial_port &serialPort, int secondsUpdate, IChopperMessages& msgSink)
    :_serialPort(serialPort), _msgSink(msgSink)
{
    _secondsUpdate = secondsUpdate;
	_lastTime = clock();
}


ChopperControl::~ChopperControl()
{
    _serialPort.close();
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
    _msgSink.Sent(szCommand);
    boost::asio::write(_serialPort,szCommand);
    //_serialPort.Write( szCommand );
}



bool ChopperControl::ProcessData()
{
    bool haveData = false;

    if( _serialPort.IsDataAvailable() )
    {
        try
        {
            string line = _serialPort.ReadLine( 200 );

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
