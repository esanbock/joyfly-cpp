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

#include "c-joy-fly-view.h"

using namespace std;

class CJoyFlyController
{
public:
	CJoyFlyController();
	void AddView(CJoyFlyView* pView);
	void OnChopperMessage( const char* szMsg );
	void DebugMessage( const char* szMsg );
    ChopperControl& ConnectToChopper( const string serialDevice, int secondsUpdate );
    virtual ~CJoyFlyController();
    void ProcessJoystickInput();
    void OpenJoystick(int joystickNum);
    void RunJoystickTests();
    void DoCommandLoop();
    
protected:
    int AxisCommandSimple( ChopperControl& control, CJoyTest& sidewinder, const char* szCommand, int joyAxis, int min, int max );
    int HatCommandIncrement( ChopperControl& control, CJoyTest& sidewinder, Uint8 down, Uint8 up, const char* szCommand, int lowVal, int highVal );
    int ButtonCommandToggle( ChopperControl& control, CJoyTest& sidewinder, const char* szCommand, int joyAxis );
    int ButtonCommandToggle( ChopperControl& control, CJoyTest& sidewinder, const char* szCommand, int joyAxis, int ifTrue, int ifFalse );
private:
	vector<CJoyFlyView*> _views;
    ChopperControl* _pChopperControl = NULL;
    CJoyTest* _sidewinder = NULL;
    
    static const int JOYSTICK_X = 0;
    static const int JOYSTICK_Y = 1;
    static const int JOYSTICK_Z = 2;
    static const int JOYSTICK_THROTTLE = 3;
    static const int JOYSTICK_AUTOPILOT = 4;
    static const int JOYSTICK_HOME = 1;
    static const int JOYSTICK_STATUS = 8;
    static const int JOYSTICK_VOLTAGE = 5;
    
    
    int _currentHatY;
    
    int _prevVals[4];
    int _curVals[4];
    
    int _prevButtonVals[5];
    int _curButtonVals[5];
    bool _buttonToggle[5];
};

#endif // _C_JOY_FLY_CONTROLLER_H_

