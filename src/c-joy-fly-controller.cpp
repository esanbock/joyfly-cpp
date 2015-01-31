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

#include "c-joy-test.h"
#include "c-joy-fly-controller.h"

CJoyFlyController::CJoyFlyController()
    :_prevVals {0,0,0,127} ,_curVals{0,0,0,127}
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

int CJoyFlyController::AxisCommandSimple( ChopperControl& control, CJoyTest& sidewinder, const char* szCommand, int joyAxis, int min, int max )
{
    _curVals[joyAxis] = sidewinder.GetAxisNormalized (joyAxis, min, max);
    if( _prevVals[joyAxis] != _curVals[joyAxis] )
    {
        control.SendSimpleCommand(szCommand, _curVals[joyAxis]);
    }
    _prevVals[joyAxis] = _curVals[joyAxis];
    
    return _prevVals[joyAxis];
}

int CJoyFlyController::ButtonCommandToggle( ChopperControl& control, CJoyTest& sidewinder, const char* szCommand, int joyAxis, int ifTrue, int ifFalse )
{
    _curButtonVals[joyAxis] = sidewinder.GetButton (joyAxis);
    if( _prevButtonVals[joyAxis] != _curButtonVals[joyAxis] && _curButtonVals[joyAxis] == 1 )
    {
        _buttonToggle[joyAxis] = !_buttonToggle[joyAxis];
        
        control.SendCommand(szCommand, _buttonToggle[joyAxis] ? ifTrue : ifFalse);
    }
    _prevButtonVals[joyAxis] = _curButtonVals[joyAxis];
    return _prevButtonVals[joyAxis];
}

int CJoyFlyController::ButtonCommandToggle( ChopperControl& control, CJoyTest& sidewinder, const char* szCommand, int joyAxis )
{
    _curButtonVals[joyAxis] = sidewinder.GetButton (joyAxis);
    if( _prevButtonVals[joyAxis] != _curButtonVals[joyAxis] && _curButtonVals[joyAxis] == 1 )
    {
        _buttonToggle[joyAxis] = !_buttonToggle[joyAxis];
        control.SendCommand(szCommand);
    }
    _prevButtonVals[joyAxis] = _curButtonVals[joyAxis];
    return _prevButtonVals[joyAxis];
}



int CJoyFlyController::HatCommandIncrement( ChopperControl& control, CJoyTest& sidewinder, Uint8 down, Uint8 up, const char* szCommand, int lowVal, int highVal )
{
    static long lastHatTime=666;
    
    clock_t now = clock();
    
    
    if( (now - lastHatTime) < CLOCKS_PER_SEC / 10 )
    {
        return 0;
    }
    
    lastHatTime = now;
    
    
    Uint8 hatVal = sidewinder.GetHat( 0 );
    if( hatVal & up )
    {
        _currentHatY++;
    }
    else if( hatVal & down )
    {
        _currentHatY--;
    }
    else
    {
        return -1;
    }
    
    _currentHatY = min( highVal, max( lowVal, _currentHatY) );
    control.SendCommand( szCommand, _currentHatY);
    
    return _currentHatY;
}

void CJoyFlyController::OpenJoystick(int joystickNum)
{
    DebugMessage("Opening joystick");
    _sidewinder = new CJoyTest(joystickNum);
}

void CJoyFlyController::RunJoystickTests()
{
    _sidewinder->RunTests();
}

void CJoyFlyController::ProcessJoystickInput()
{
    
    AxisCommandSimple( *_pChopperControl, *_sidewinder, ":T", JOYSTICK_THROTTLE, 255, 0);
    AxisCommandSimple( *_pChopperControl, *_sidewinder, ":B", JOYSTICK_X, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
    AxisCommandSimple( *_pChopperControl, *_sidewinder, ":P", JOYSTICK_Y, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
    AxisCommandSimple( *_pChopperControl, *_sidewinder, ":Y", JOYSTICK_Z, -255, 255);
    ButtonCommandToggle( *_pChopperControl, *_sidewinder, ":N", JOYSTICK_AUTOPILOT, 0, 1 );
    ButtonCommandToggle( *_pChopperControl, *_sidewinder, ":H", JOYSTICK_HOME);
    ButtonCommandToggle( *_pChopperControl, *_sidewinder, ":S", JOYSTICK_STATUS);
    ButtonCommandToggle( *_pChopperControl, *_sidewinder, ":V", JOYSTICK_VOLTAGE);
    
    HatCommandIncrement( *_pChopperControl, *_sidewinder, SDL_HAT_DOWN, SDL_HAT_UP, ":L", -20, 20 );
}

void CJoyFlyController::DoCommandLoop()
{
    bool quitting = false;
    do
    {
        ProcessJoystickInput();
        SDL_JoystickUpdate();
        _pChopperControl->ProcessData();
        
        
    } while( !quitting );
}

int CJoyFlyController::Start(string& serialDevice, int secondsUpdate)
{
    OpenJoystick(0);

    SDL_JoystickUpdate();


    ConnectToChopper( serialDevice.c_str(), secondsUpdate );

    pCommandLoopThread = new std::thread([this]() {DoCommandLoop();});

    return 0;
}
