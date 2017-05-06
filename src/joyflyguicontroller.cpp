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

#include <iostream>
#include <vector>
#include <thread>
#include <stdexcept>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include "serialstream.h"

#include "abstractchopper.h"
#include "teensychopper.h"
#include "simulatedchopper.h"

#include "SDL2/SDL.h"

#include "joyflycontroller.h"
#include "joyflyview.h"
#include "joystickinputer.h"
#include "joyflyguicontroller.h"

using namespace boost;
using namespace std;

CJoyFlyGuiController::CJoyFlyGuiController()
{
}

void CJoyFlyGuiController::AddView(CJoyFlyView* pView)
{
	_views.push_back( pView );
}

CJoyFlyGuiController::~CJoyFlyGuiController()
{
    delete _pChopperControl;

    if( _pComPort != NULL )
    {
        _pComPort->close();
        delete _pComPort;
    }
}

void CJoyFlyGuiController::OnChopperMessage( const char* szMsg )
{
	for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
	{
        CMainView* pMainView = dynamic_cast<CMainView*>(*it);
        if( pMainView != nullptr)
            pMainView->OnChopperMessage(szMsg);
	}
}

void CJoyFlyGuiController::DebugMessage( const char* szMsg )
{
	for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
	{
        CMainView* pMainView = dynamic_cast<CMainView*>(*it);
        if( pMainView != nullptr)
            pMainView->OnDebugMessage(szMsg);
	}
}

AbstractChopper* CJoyFlyGuiController::ConnectToChopper( const string serialDevice, int secondsUpdate )
{
    DebugMessage( (string("Opening serial port ") + serialDevice).c_str() );

    SerialOptions options;
    options.setDevice(serialDevice);
    options.setFlowControl(SerialOptions::FlowControl::noflow);
    options.setParity(SerialOptions::Parity::noparity);
    options.setStopBits(SerialOptions::StopBits::one);
    options.setBaudrate(38400);
    _pComPort = new SerialStream(options);
    _pComPort->exceptions(ios::badbit | ios::failbit);

    AbstractChopper* pTeensyChopper = new TeensyChopper(*_pComPort, secondsUpdate, *this);
    pTeensyChopper->Start();
    return pTeensyChopper;
}

AbstractChopper* CJoyFlyGuiController::ConnectToSimulator( int secondsUpdate )
{
    DebugMessage ("simulator");
    AbstractChopper* pSimulatedChopper = new CSimulatedChopper(secondsUpdate, *this);
    pSimulatedChopper->Start();
    return pSimulatedChopper;
}

void CJoyFlyGuiController::Quit()
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


void CJoyFlyGuiController::OnMessage(const char *data)
{
    OnChopperMessage(data);
}

void CJoyFlyGuiController::OnUnparsable(const char *data)
{
    string errMsg = "Unable to parse message:  ";
    errMsg += data;
    OnChopperMessage(errMsg.c_str());
}

void CJoyFlyGuiController::OnDebug(const char *data)
{
    DebugMessage(data);
}

void CJoyFlyGuiController::Sent(const char *data)
{
    DebugMessage(data);
}

void CJoyFlyGuiController::OnVoltageChange(float newVoltage)
{
    _voltageHistory.Add(clock(),(double)newVoltage);

    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        CGraphView* pView = dynamic_cast<CGraphView*>(*it);
        if( pView != nullptr)
            pView->OnVoltageChange(newVoltage);
    }
}

void CJoyFlyGuiController::OnThrottleChange(int newThrottle)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        CGraphView* pView = dynamic_cast<CGraphView*>(*it);
        if( pView != nullptr)
            pView->OnThrottleChange(newThrottle);
    }
}

void CJoyFlyGuiController::OnCollective(double collective)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        CMainView* pMainView = dynamic_cast<CMainView*>(*it);
        if( pMainView != nullptr)
            pMainView->OnCollective(collective);
    }
}

void CJoyFlyGuiController::OnAutoNav(bool isOn)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        CMainView* pMainView = dynamic_cast<CMainView*>(*it);
        if( pMainView != nullptr)
            pMainView->OnAutoNav(isOn);
    }
}

void CJoyFlyGuiController::OnNewHeading( const int x, const int y, const int z)
{
    _heading_x = x;
    _heading_y = y;
    _heading_z = z;
}

void CJoyFlyGuiController::GetHeading( int& x, int& y, int &z)
{
    x = _heading_x;
    y = _heading_y;
    z = _heading_z;
}

void CJoyFlyGuiController::OnPing(float latency)
{
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        CMainView* pMainView = dynamic_cast<CMainView*>(*it);
        if( pMainView != nullptr)
            pMainView->OnPing(latency);
    }
}

int CJoyFlyGuiController::Connect(const string serialDevice, int secondsUpdate)
{
    if( _pChopperControl != NULL )
        throw domain_error("controller already started");

    if( serialDevice[0] != '/' )
        _pChopperControl = ConnectToSimulator(secondsUpdate);
    else
        _pChopperControl = ConnectToChopper( serialDevice.c_str(), secondsUpdate );

    return 0;
}

void CJoyFlyGuiController::AddJoyStick(int joyNum)
{
    _pJoystickInputer = new CJoystickInputer(joyNum, *this);
    _pJoystickInputer->Start();
}

void CJoyFlyGuiController::RunJoystickTests()
{
    CSdlJoystick::RunTests();
}

void CJoyFlyGuiController::ToggleAutoPilot()
{
    _autoNav = !_autoNav;
    _pChopperControl->SendCommand(":N", _autoNav ? 1 : 0);
    this->OnAutoNav( _autoNav );
}

void CJoyFlyGuiController::SetAutoPilot( bool isOn )
{
    _autoNav = isOn;
    _pChopperControl->SendCommand(":N", _autoNav ? 1 : 0);
    this->OnAutoNav( _autoNav );
}

void CJoyFlyGuiController::SetHome()
{
    _pChopperControl->SetHome();
}

void CJoyFlyGuiController::GetStatus()
{
    _pChopperControl->SendCommand(":S");
    _pChopperControl->SendCommand(":V");
}

void CJoyFlyGuiController::GetVoltage()
{
    _pChopperControl->SendCommand(":V");
}

void CJoyFlyGuiController::Bank(int val)
{
    _pChopperControl->SendSimpleCommand(":B",  val);
}

void CJoyFlyGuiController::Pitch(int val)
{
    _pChopperControl->SendSimpleCommand(":P",  val);
}

void CJoyFlyGuiController::Yaw(int val)
{
    _pChopperControl->SendSimpleCommand(":Y",  val);
}

void CJoyFlyGuiController::SetThrottle(int val)
{
    _pChopperControl->SendSimpleCommand(":T",  val);
    OnThrottleChange(val);
}

void CJoyFlyGuiController::Lift(int val)
{
    _pChopperControl->SendSimpleCommand(":L",  val);
}

void CJoyFlyGuiController::OnIMUChanged( const int x, const int y, const int z )
{
    _imuHistory[0].Add(clock(),x);
    _imuHistory[1].Add(clock(),y);
    _imuHistory[2].Add(clock(),z);

    _headingHistory[0].AddOrUpdate(clock(),_heading_x);
    _headingHistory[1].AddOrUpdate(clock(),_heading_y);
    _headingHistory[2].AddOrUpdate(clock(),_heading_z);

    //cout << "new IMU (x,y,z) = (" << x << "," << y << "," << z << ")" << endl;
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        CJoyFlyView* pView = *it;
        if( pView != nullptr)
        {
            pView->OnIMU(x,y,z);
            pView->OnBank(x);
            pView->OnPitch(y);
            pView->OnYaw(z);

        }
    }

}

TimeSeries<double,double>* CJoyFlyGuiController::GetVoltageHistory()
{
    return &_voltageHistory;
}

TimeSeries<double,double>* CJoyFlyGuiController::GetIMUHistory()
{
    return _imuHistory;
}

TimeSeries<double,double>* CJoyFlyGuiController::GetHeadingHistory()
{
    return _headingHistory;
}

