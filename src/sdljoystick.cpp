/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * joyfly-cpp
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

#include <stdexcept>
#include <iostream>
#include "SDL2/SDL.h"
#include "sdljoystick.h"

using namespace std;

bool CSdlJoystick::_inited = false;

void CSdlJoystick::Initialize()
{
    if( !_inited )
    SDL_Init( SDL_INIT_JOYSTICK );
    _inited = true;
}

CSdlJoystick::CSdlJoystick( int selectedJoy )
{
	_currentJoy = selectedJoy;
	_pJoystick = SDL_JoystickOpen(_currentJoy);

	if( _pJoystick == NULL )
		throw JoystickException( "unable to open joystick" );
}

CSdlJoystick::~CSdlJoystick()
{
	if( _pJoystick )
	{
		SDL_JoystickClose( _pJoystick );
		_pJoystick = NULL;
	}
}

int CSdlJoystick::GetJoyCount()
{
    Initialize();
	return SDL_NumJoysticks();
}

int CSdlJoystick::GetAxisVal( int axis )
{
	SDL_JoystickUpdate();
	return SDL_JoystickGetAxis(_pJoystick, axis);
}

int CSdlJoystick::GetAxisNormalized( int axis, int min, int max )
{
	int axisVal = GetAxisVal( axis );
	float axisPct = ((float)axisVal + (float)32767) / ((float)65534);

	return min + (( max - min ) * axisPct);
}

int CSdlJoystick::GetAxisCount( )
{
	 return SDL_JoystickNumAxes( _pJoystick );
}

int CSdlJoystick::GetButtonCount()
{
    return SDL_JoystickNumButtons( _pJoystick );
}

int CSdlJoystick::GetHatCount()
{
	return SDL_JoystickNumAxes( _pJoystick );
}


int CSdlJoystick::GetButton( int button )
{
	return SDL_JoystickGetButton( _pJoystick, button);
}

Uint8 CSdlJoystick::GetHat( int hat )
{
	return SDL_JoystickGetHat( _pJoystick, hat );
}

string CSdlJoystick::GetName()
{
    return SDL_JoystickName( _pJoystick);
}

string CSdlJoystick::GetJoyName(int joyNum)
{
    Initialize();
    SDL_Joystick* pJoystick = SDL_JoystickOpen(joyNum);
    if( pJoystick == NULL )
    {
        throw runtime_error("unable to open joystick to get its name");
    }
    string result = SDL_JoystickName(pJoystick);
    SDL_JoystickClose(pJoystick);
    return result;
}

void CSdlJoystick::Update()
{
    SDL_JoystickUpdate();
}

int CSdlJoystick::RunTests()
{
    Initialize();
	int joyCount = SDL_NumJoysticks();

	cout << "I found " << joyCount << " joysticks\n";

    if( joyCount == 0 )
        return -2;
	
	// using joystick # 0
	int selectedJoystick = 0;

    CSdlJoystick testStick(selectedJoystick);
    
    cout << "joystick name is: " << testStick.GetName() << endl;

	// get the axis
    int numAxis = testStick.GetAxisCount();
	cout << "joystick " << selectedJoystick << " has " << numAxis << " axis" << endl;

    int numButtons = testStick.GetButtonCount();
	cout << "joystick " << selectedJoystick << " has " << numButtons << " buttons" << endl;

    int numHats = testStick.GetHatCount();
	cout << "joystick " << selectedJoystick << " has " << numHats << " hats" << endl;
	
	//char key;
	do
	{
        testStick.Update();
		// dump axis
		for( int axis = 0; axis < numAxis; axis++ )
		{
            int axisVal = testStick.GetAxisVal(axis);
			cout << axisVal << " ";
		}
		cout << " | ";
		// dump buttons
		for( int button = 0; button < numButtons; button++ )
		{
            int buttonVal = testStick.GetButton(button);
			cout << buttonVal << " ";
		}
		// dump hats
		for( int hat=0; hat < numHats; hat++ )
		{
            int hatVal = testStick.GetHat( hat );
			cout << hatVal << " ";
		}
		cout << endl;
		//cin >> key;
    } while (1);
}
