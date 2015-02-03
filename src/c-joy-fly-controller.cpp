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
#include <stdexcept>
#include "SerialStream.h"
#include "SerialPort.h"
#include "abstractchopper.h"
#include "choppercontrol.h"
#include "simulatedchopper.h"

#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "controllerinputer.h"
#include "c-joy-fly-view.h"
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
    delete _comPort;
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
        (*it)->OnDebugMessage(szMsg);
	}
}

AbstractChopper* CJoyFlyController::ConnectToChopper( const string serialDevice, int secondsUpdate )
{
    DebugMessage( (string("Opening serial port ") + serialDevice).c_str() );
    _comPort = new SerialPort( serialDevice.c_str() );
    _comPort->Open( SerialPort::BAUD_9600, SerialPort::CHAR_SIZE_8, SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1) ;
    return new ChopperControl(*_comPort, secondsUpdate, *this);
}

AbstractChopper* CJoyFlyController::ConnectToSimulator( int secondsUpdate )
{
    DebugMessage ("simulator");
    return new CSimulatedChopper(secondsUpdate, *this);
}

void CJoyFlyController::Quit()
{
    _quitting = true;

    // stop all updates
    _views.clear();

    if( _pJoystickInputer != NULL )
    {
        _pJoystickInputer->Stop();
        delete _pJoystickInputer;
        _pJoystickInputer = NULL;
    }
}

void CJoyFlyController::DoCommandLoop()
{
    do
    {
        _pChopperControl->ProcessData();

    } while( !_quitting );
}

void CJoyFlyController::OnMessage(const char *data)
{
    OnChopperMessage(data);
}

void CJoyFlyController::OnDebug(const char *data)
{
    DebugMessage(data);
}

void CJoyFlyController::Sent(const char *data)
{
    DebugMessage(data);
}

void CJoyFlyController::OnVoltageChange(float newVoltage)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        (*it)->OnVoltageChange(newVoltage);
    }
}

void CJoyFlyController::OnThrottleChange(int newThrottle)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        (*it)->OnThrottleChange(newThrottle);
    }
}

void CJoyFlyController::OnPing(float latency)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        (*it)->OnPing(latency);
    }
}

int CJoyFlyController::Start(const string serialDevice, int secondsUpdate)
{
    if( _pCommandLoopThread != NULL )
        throw domain_error("controller already started");

    if( serialDevice == "/simulator" )
        _pChopperControl = ConnectToSimulator(secondsUpdate);
    else
        _pChopperControl = ConnectToChopper( serialDevice.c_str(), secondsUpdate );

    _pCommandLoopThread = new std::thread([this]() {DoCommandLoop();});

    return 0;
}

void CJoyFlyController::AddJoyStick(int joyNum)
{
    _pJoystickInputer = new CJoystickInputer(joyNum, *this);
    _pJoystickInputer->Start();
}

void CJoyFlyController::RunJoystickTests()
{
    CSdlJoystick::RunTests();
}

void CJoyFlyController::SetAutoPilot(bool onOff)
{
    _pChopperControl->SendCommand(":N", onOff ? 1 : 0);
}

void CJoyFlyController::SetHome()
{
    _pChopperControl->SendCommand(":H");
}

void CJoyFlyController::GetStatus()
{
    _pChopperControl->SendCommand(":S");
}

void CJoyFlyController::GetVoltage()
{
    _pChopperControl->SendCommand(":V");
}

void CJoyFlyController::Bank(int val)
{
    _pChopperControl->SendSimpleCommand(":B",  val);
}

void CJoyFlyController::Pitch(int val)
{
    _pChopperControl->SendSimpleCommand(":P",  val);
}

void CJoyFlyController::Yaw(int val)
{
    _pChopperControl->SendSimpleCommand(":Y",  val);
}

void CJoyFlyController::SetThrottle(int val)
{
    _pChopperControl->SendSimpleCommand(":T",  val);
    OnThrottleChange(val);
}

void CJoyFlyController::Lift(int val)
{
    _pChopperControl->SendSimpleCommand(":L",  val);
}

void CJoyFlyController::OnIMUChanged( int x, int y, int z )
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        float fAngle = (float)x * (360.0 / 1024.0);
        (*it)->OnBank(fAngle);
        (*it)->OnPitch(((float)y / 1024.0) * 2 - 1.0);
    }

}

