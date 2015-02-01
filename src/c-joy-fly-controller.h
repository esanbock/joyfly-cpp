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

#include "controllerinputer.h"
#include "mainwindow.h"
#include "c-joy-fly-view.h"
#include "joystickinputer.h"

using namespace std;

class CJoyFlyController : public IControllerInputer, public IChopperMessages
{
public:
	CJoyFlyController();
    virtual ~CJoyFlyController();

    // GUI
    void AddView(CJoyFlyView* pView);

    // joystick
    void AddJoyStick(int joyNum);
    void RunJoystickTests();

    // NAV
    virtual void SetAutoPilot(bool onOff);
    virtual void SetHome();
    virtual void GetStatus();
    virtual void GetVoltage();
    virtual void Bank(int val);
    virtual void Pitch(int val);
    virtual void Yaw(int val);
    virtual void SetThrottle(int val);
    virtual void Lift(int val);

    // go
    int Start(string& serialDevice, int secondsUpdate);
    void Quit();

    // message sink
    virtual void OnVoltageChange(float newVoltage);
    virtual void OnMessage(const char* data);
    virtual void Sent(const char* data);
    virtual void OnPing(float latency);
    virtual void OnDebug(const char* data);

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
    thread* pCommandLoopThread;
    CJoystickInputer* _pJoystickInputer = NULL;
    bool _quitting = false;

};

#endif // _C_JOY_FLY_CONTROLLER_H_

