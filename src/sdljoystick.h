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

#ifndef _C_JOY_TEST_H_
#define _C_JOY_TEST_H_

#include <string>
#include <exception>

#include "joystickexception.h"

using namespace std;

class CSdlJoystick
{
public:
    CSdlJoystick( int joyNum );
    virtual ~CSdlJoystick();

    static void Initialize();
    static int GetJoyCount();
    static string GetJoyName(int joyNum);
    static int RunTests();

    string GetName();
    int GetAxisVal( int axis );
	int GetAxisCount();
    int GetButtonCount();
	int GetAxisNormalized( int axis, int min, int max );
	int GetButton( int button );
	int GetHatCount();
	Uint8 GetHat( int hat );
    void Update();

protected:
	int _currentJoy;
	SDL_Joystick* _pJoystick;
private:
    static bool _inited;
};

#endif // _C_JOY_TEST_H_
