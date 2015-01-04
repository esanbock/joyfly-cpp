//
//  choppercontrol.cpp
//  joyfly-cpp
//
//  Created by esanbockd on 1/3/15.
//  Copyright (c) 2015 Very Unorganized. All rights reserved.
//

//#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "SerialStream.h"
#include "SerialPort.h"
#include "choppercontrol.h"

using namespace std;

ChopperControl::ChopperControl(SerialPort& serialPort)
    :_serialPort(serialPort)
{
    _lastPingNum = 3;
    _sentPingClock = clock();
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
    cout << command << endl;
    _serialPort.Write( command );
}

void ChopperControl::ProcessPingResponse( string& line )
{
    int pingResponse = atoi( line.substr(3).c_str() );
    if( pingResponse != _lastPingNum )
    {
        cout << "unmatched ping " << pingResponse << ".  Expected " << _lastPingNum << endl;
        return;
    }
    float latency = (float)(clock() - _sentPingClock ) / CLOCKS_PER_SEC ;
    cout << "latency = " << latency << ": " << " " << clock() << "," << _sentPingClock << endl; ;
}


void ChopperControl::ProcessCommandResponse( string& line )
{
    if( line.compare(0,3,":ER") == 0 )
    {
        ProcessPingResponse( line );
        return;
    }
    
    // otherwise, NAK
    cout << "unrecognized command from chopper" << endl;
}

