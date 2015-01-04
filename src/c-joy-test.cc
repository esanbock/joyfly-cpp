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

#include <iostream>
#include "SDL2/SDL.h"
#include "c-joy-test.h"

using namespace std;

CJoyTest::CJoyTest()
{
	_pJoystick = NULL;
	SDL_Init( SDL_INIT_JOYSTICK );
}

CJoyTest::CJoyTest( int selectedJoy )
{
	CJoyTest();
	
	_currentJoy = selectedJoy;
	_pJoystick = SDL_JoystickOpen(_currentJoy);

	if( _pJoystick == NULL )
		throw JoystickException( "unable to open joystick" );
}

CJoyTest::~CJoyTest()
{
	if( _pJoystick )
	{
		SDL_JoystickClose( _pJoystick );
		_pJoystick = NULL;
	}
}

int CJoyTest::GetJoyCount()
{
	return SDL_NumJoysticks();
}

int CJoyTest::GetAxisVal( int axis )
{
	SDL_JoystickUpdate();
	return SDL_JoystickGetAxis(_pJoystick, axis);
}

int CJoyTest::GetAxisNormalized( int axis, int min, int max )
{
	int axisVal = GetAxisVal( axis );
	float axisPct = ((float)axisVal + (float)32767) / ((float)65534);

	return min + (( max - min ) * axisPct);
}

int CJoyTest::GetAxisCount( )
{
	 return SDL_JoystickNumAxes( _pJoystick );
}

int CJoyTest::GetHatCount()
{
	return SDL_JoystickNumAxes( _pJoystick );
}


int CJoyTest::GetButton( int button )
{
	return SDL_JoystickGetButton( _pJoystick, button);
}

Uint8 CJoyTest::GetHat( int hat )
{
	return SDL_JoystickGetHat( _pJoystick, hat );
}

int CJoyTest::RunTests()
{
	int joyCount = SDL_NumJoysticks();

	cout << "I found " << joyCount << " joysticks\n";
	
	// using joystick # 0
	int selectedJoystick = 0;
	SDL_Joystick* pJoystick = SDL_JoystickOpen(selectedJoystick);
	if( pJoystick == NULL )
	{
		cout << "ERROR opening joystick # " << selectedJoystick << endl;
		return -1;
	}
    
    cout << "joystick name is: " << SDL_JoystickName(pJoystick) << endl;

	// get the axis
	int numAxis = SDL_JoystickNumAxes( pJoystick );
	cout << "joystick " << selectedJoystick << " has " << numAxis << " axis" << endl;

	int numButtons = SDL_JoystickNumButtons( pJoystick );
	cout << "joystick " << selectedJoystick << " has " << numButtons << " buttons" << endl;

	int numHats = GetHatCount();
	cout << "joystick " << selectedJoystick << " has " << numHats << " hats" << endl;
	
	//char key;
	do
	{
		SDL_JoystickUpdate();
		// dump axis
		for( int axis = 0; axis < numAxis; axis++ )
		{
			int axisVal = SDL_JoystickGetAxis(pJoystick, axis);
			cout << axisVal << " ";
		}
		cout << " | ";
		// dump buttons
		for( int button = 0; button < numButtons; button++ )
		{
			int buttonVal = GetButton(button);
			cout << buttonVal << " ";
		}
		// dump hats
		for( int hat=0; hat < numHats; hat++ )
		{
			int hatVal = GetHat( hat );
			cout << hatVal << " ";
		}
		cout << endl;
		//cin >> key;
    } while (1);
}