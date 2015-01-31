/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) Douglas Esanbock 2011 <douglas@>
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
#include <iomanip>
#include <sstream>

#include "mainwindow.h"

#include <QApplication>

// apple junk
#ifdef __APPLE__
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "SerialStream.h"
#include "SerialPort.h"
#include "c-joy-test.h"
#include "choppercontrol.h"
#include "c-joy-fly-controller.h"
#include "c-terminal-view.h"

using namespace std;


int main (int argc, char * argv[])
{
    cout << "Hello world!" << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();


	if( argc < 1 )
	{
		cout << "wrong number of parameters" << endl;
		return -1;
	}
    
	string serialDevice = string(argv[1] );

	CJoyFlyController controller;
    
    CTerminalView terminalView;
    controller.AddView( &terminalView );

	try
	{
        controller.OpenJoystick(0);

		SDL_JoystickUpdate();

		if( serialDevice == "/test" )
		{
            controller.RunJoystickTests();

			return 0;
		}
        if( argc < 2)
        {
            cout << "wrong number of parameters" << endl;
            return -1;
        }

        int secondsUpdate = atoi(argv[2]);
        controller.ConnectToChopper( serialDevice.c_str(), secondsUpdate );
        
        // this doesn't return until it's over
        controller.DoCommandLoop();

		return 0;
	}
	catch( JoystickException& jExcept )
	{
		cout << "Joystick error:  " << jExcept.what() << endl;
		return -2;
	}
}


