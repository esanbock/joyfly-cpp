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
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include "serialstream.h"

#include "abstractchopper.h"
#include "teensychopper.h"
#include "simulatedchopper.h"

#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "controllerinputer.h"
#include "c-joy-fly-view.h"
#include "c-joy-fly-controller.h"

using namespace boost;

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

    if( _pComPort != NULL )
    {
        _pComPort->close();
        delete _pComPort;
    }
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

    SerialOptions options;
    options.setDevice(serialDevice);
    options.setFlowControl(SerialOptions::FlowControl::noflow);
    options.setParity(SerialOptions::Parity::noparity);
    options.setStopBits(SerialOptions::StopBits::one);
    options.setBaudrate(9600);
    _pComPort = new SerialStream(options);
    _pComPort->exceptions(ios::badbit | ios::failbit);

    AbstractChopper* pTeensyChopper = new TeensyChopper(*_pComPort, secondsUpdate, *this);
    pTeensyChopper->Start();
    return pTeensyChopper;
}

AbstractChopper* CJoyFlyController::ConnectToSimulator( int secondsUpdate )
{
    DebugMessage ("simulator");
    AbstractChopper* pSimulatedChopper = new CSimulatedChopper(secondsUpdate, *this);
    pSimulatedChopper->Start();
    return pSimulatedChopper;
}

void CJoyFlyController::Quit()
{
    // stop all updates
    _views.clear();

    if( _pJoystickInputer != NULL )
    {
        _pJoystickInputer->Stop();
        delete _pJoystickInputer;
        _pJoystickInputer = NULL;
    }
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

void CJoyFlyController::OnAutoNav(bool isOn)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        (*it)->OnAutoNav(isOn);
    }
}

void CJoyFlyController::OnPing(float latency)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        (*it)->OnPing(latency);
    }
}

int CJoyFlyController::Connect(const string serialDevice, int secondsUpdate)
{
    if( _pChopperControl != NULL )
        throw domain_error("controller already started");

    if( serialDevice[0] != '\/' )
        _pChopperControl = ConnectToSimulator(secondsUpdate);
    else
        _pChopperControl = ConnectToChopper( serialDevice.c_str(), secondsUpdate );

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

void CJoyFlyController::ToggleAutoPilot()
{
    _autoNav = !_autoNav;
    _pChopperControl->SendCommand(":N", _autoNav ? 1 : 0);
    this->OnAutoNav( _autoNav );
}

void CJoyFlyController::SetAutoPilot( bool isOn )
{
    _autoNav = isOn;
    _pChopperControl->SendCommand(":N", _autoNav ? 1 : 0);
    this->OnAutoNav( _autoNav );
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
    const float MAXX = 771;
    const float MINX = 700;
    //771-700
    float fAngle = 90 + ((float)x -700) * 2.53521126761;

    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        (*it)->OnBank(fAngle);
        cout << "fAngle = " << fAngle << endl;
        //(*it)->OnPitch(((float)y / 50.0) * 2 - 1.0);
    }

}

