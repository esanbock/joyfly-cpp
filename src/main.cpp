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
#include <thread>

#include <QApplication>

#include "mainwindow.h"
#include "choppercontrol.h"
#include "c-joy-fly-controller.h"
#include "c-terminal-view.h"
#include "qtjoyflyview.h"
#include "joystickexception.h"

using namespace std;


int main (int argc, char * argv[])
{
    cout << "Hello world!" << std::endl;

    QApplication app(argc, argv);

    if( argc < 2 )
	{
		cout << "wrong number of parameters" << endl;
		return -1;
	}
    CJoyFlyController controller;
    string serialDevice = string(argv[1] );

    try
    {
        if( serialDevice == "/test" )
        {
            controller.RunJoystickTests();

            return 0;
        }
        if( argc < 3)
        {
            cout << "wrong number of parameters" << endl;
            return -1;
        }

        int secondsUpdate = atoi(argv[2]);

        CTerminalView terminalView;
        controller.AddView( &terminalView );
        CQtJoyFlyView guiView(new MainWindow());
        controller.AddView( &guiView );
        controller.Start(serialDevice, secondsUpdate);
        app.exec();
	}
	catch( JoystickException& jExcept )
	{
		cout << "Joystick error:  " << jExcept.what() << endl;
		return -2;
	}
}


