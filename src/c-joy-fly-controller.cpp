/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * c-joy-fly-controller.cpp
 * Copyright (C) 2015 Douglas Esanbock <douglas@horus>
 *
 * joyfly-cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * joyfly-cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <vector>
#include <thread>
#include "SerialStream.h"
#include "SerialPort.h"
#include "choppercontrol.h"

#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "c-joy-fly-controller.h"

CJoyFlyController::CJoyFlyController()
{
}

void CJoyFlyController::AddView(CJoyFlyView* pView)
{
	_views.push_back( pView );
}

CJoyFlyController::~CJoyFlyController()
{
    delete _pChopperControl;
}

void CJoyFlyController::OnChopperMessage( const char* szMsg )
{
	for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
	{
		(*it)->OnChopperMessage(szMsg);
	}
}

void CJoyFlyController::DebugMessage( const char* szMsg )
{
	for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
	{
		(*it)->OnChopperMessage(szMsg);
	}
}

ChopperControl& CJoyFlyController::ConnectToChopper( const string serialDevice, int secondsUpdate )
{
    DebugMessage( (string("Opening serial port ") + serialDevice).c_str() );
    SerialPort comPort( serialDevice.c_str() );
    comPort.Open( SerialPort::BAUD_9600, SerialPort::CHAR_SIZE_8, SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1) ;
    _pChopperControl = new ChopperControl(comPort, secondsUpdate);
    return *_pChopperControl;
}


void CJoyFlyController::DoCommandLoop()
{
    do
    {
        _pChopperControl->ProcessData();
        
    } while( !_quitting );
}

int CJoyFlyController::Start(string& serialDevice, int secondsUpdate)
{
    ConnectToChopper( serialDevice.c_str(), secondsUpdate );

    pCommandLoopThread = new std::thread([this]() {DoCommandLoop();});

    return 0;
}

void CJoyFlyController::AddJoyStick(int joyNum)
{
    _pJoystickInputer = new CJoystickInputer(joyNum, *this);
    _pJoystickInputer->Start();
}

void CJoyFlyController::RunJoystickTests()
{
    CJoyTest t;
    t.RunTests();
}
