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

#include "abstractchopper.h"
#include "teensychopper.h"
#include "simulatedchopper.h"

#include "SDL2/SDL.h"

#include "joyflycontroller.h"
#include "joyflyview.h"
#include "joystickinputer.h"
#include "joyflyguicontroller.h"

using namespace std;

CJoyFlyGuiController::CJoyFlyGuiController()
{
    _startClock = clock();
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
        //_pComPort->close();
        delete _pComPort;
    }
}

AbstractChopper& CJoyFlyGuiController::GetChopper()
{
    return *_pChopperControl;
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

    _pComPort = new LibSerial::SerialStream();
    _pComPort->Open(serialDevice);
    _pComPort->SetBaudRate(LibSerial::SerialStreamBuf::BAUD_38400);
    _pComPort->SetCharSize( LibSerial::SerialStreamBuf::CHAR_SIZE_8 );
    _pComPort->SetParity(LibSerial::SerialStreamBuf::PARITY_NONE);
    _pComPort->SetNumOfStopBits(1);
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
    double elapsed = (clock() - _startClock) / (double)CLOCKS_PER_SEC;
    _voltageHistory.Add(elapsed,(double)newVoltage);

    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        (*it)->OnVoltageChange(newVoltage);
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

void CJoyFlyGuiController::OnNewMotors( const int x, const int y, const int z)
{
    double elapsed = (clock() - _startClock);
    _motorHistory[0].Add(elapsed,x);
    _motorHistory[1].Add(elapsed,y);
    _motorHistory[2].Add(elapsed,z);
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

int CJoyFlyGuiController::Connect( const int secondsUpdate )
{
    if( _pChopperControl != NULL )
        throw domain_error("controller already started");

    _pChopperControl = ConnectToSimulator(secondsUpdate);

    return 0;
}

int CJoyFlyGuiController::Connect(const string serialDevice)
{
    if( _pChopperControl != NULL )
        throw domain_error("controller already started");

    _pChopperControl = ConnectToChopper( serialDevice.c_str(), 1 ); // ping every second

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
    _pChopperControl->EnableAutopilot(_autoNav);
    this->OnAutoNav( _autoNav );
}

void CJoyFlyGuiController::SetAutoPilot( bool isOn )
{
    _autoNav = isOn;
    _pChopperControl->EnableAutopilot(_autoNav);
    this->OnAutoNav( _autoNav );
}

void CJoyFlyGuiController::SetThrottle(int val)
{
    _pChopperControl->SetThrottle(val);
    OnThrottleChange(val);
}

void CJoyFlyGuiController::OnIMUChanged( const int x, const int y, const int z )
{
    double elapsed = (clock() - _startClock);

    _imuHistory[0].Add(elapsed,x);
    _imuHistory[1].Add(elapsed,y);
    _imuHistory[2].Add(elapsed,z);

    _headingHistory[0].AddOrUpdate(elapsed,_heading_x);
    _headingHistory[1].AddOrUpdate(elapsed,_heading_y);
    _headingHistory[2].AddOrUpdate(elapsed,_heading_z);

    //cout << "new IMU (x,y,z) = (" << x << "," << y << "," << z << ")" << endl;
    for( vector<CJoyFlyView*>::iterator it = _views.begin(); it != _views.end(); ++ it )
    {
        CJoyFlyView* pView = *it;
        if( pView != nullptr)
        {
            pView->OnIMU(x,y,z);

            pView->OnBank( GetChopper().IMUVoltageToAngleXY(x) );
            pView->OnPitch( GetChopper().IMUVoltageToAngleXY(y) );
            pView->OnYaw( GetChopper().IMUVoltageToAngleZ(z) );
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

TimeSeries<double,double>* CJoyFlyGuiController::GetMotorHistory()
{
    return _motorHistory;
}
