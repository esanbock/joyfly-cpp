/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * c-joy-fly-controller.h
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

#ifndef _C_JOY_FLY_CONTROLLER_H_
#define _C_JOY_FLY_CONTROLLER_H_

#include "serialstream.h"
#include "joyflycontroller.h"
#include "joyflyview.h"
#include "joystickinputer.h"
#include "abstractchopper.h"


using namespace std;

class CJoyFlyGuiController : public IJoyflyController, public IChopperMessages
{
public:
    CJoyFlyGuiController();
    virtual ~CJoyFlyGuiController();

    // GUI
    void AddView(CJoyFlyView* pView);

    // joystick
    virtual void AddJoyStick(int joyNum);
    void RunJoystickTests();

    // NAV
    virtual AbstractChopper& GetChopper();
    virtual void ToggleAutoPilot();
    virtual void SetAutoPilot(bool isOn);
    virtual void SetThrottle(int val);
    virtual void GetHeading( int&x, int& y, int &z);

    // go
    virtual int Connect(const string serialDevice, int secondsUpdate);
    void Quit();

    // message sink
    virtual void OnVoltageChange(float newVoltage);
    virtual void OnThrottleChange(int newThrottle);
    virtual void OnAutoNav(bool isOn);
    virtual void OnMessage(const char* data);
    virtual void OnUnparsable(const char* data);
    virtual void Sent(const char* data);
    virtual void OnPing(float latency);
    virtual void OnDebug(const char* data);
    virtual void OnIMUChanged(const int x, const int y, const int z );
    virtual void OnNewHeading(const int x, const int y, const int z );
    virtual void OnCollective(double collective);
    virtual void OnNewMotors( const int x, const int y, const int z);

    // stats
    virtual TimeSeries<double,double>* GetVoltageHistory();
    virtual TimeSeries<double,double>* GetIMUHistory();
    virtual TimeSeries<double,double>* GetHeadingHistory();
    virtual TimeSeries<double,double>* GetMotorHistory();

protected:
    void OnChopperMessage( const char* szMsg );
    void DebugMessage( const char* szMsg );
    void DoCommandLoop();
    AbstractChopper* ConnectToChopper( const string serialDevice, int secondsUpdate );
    AbstractChopper* ConnectToSimulator( int secondsUpdate );
    void ProcessJoystickInput();

private:
    vector<CJoyFlyView*> _views;
    AbstractChopper* _pChopperControl = NULL;
    CJoystickInputer* _pJoystickInputer = NULL;
    SerialStream* _pComPort = NULL;
    bool _autoNav = false;
    TimeSeries<double,double> _voltageHistory;
    int _heading_x=0, _heading_y=0, _heading_z=0;
    TimeSeries<double,double> _imuHistory[3];
    TimeSeries<double,double> _headingHistory[3];
    TimeSeries<double,double> _motorHistory[3];
    clock_t _startClock;
};

#endif // _C_JOY_FLY_CONTROLLER_H_

