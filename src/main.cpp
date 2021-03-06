/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) Douglas Esanbock 2011 <douglas@esanbock.com>
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
#include <string>
#include <chrono>

#include <QApplication>

#include "joyflycontroller.h"
#include "joyflyview.h"
#include "abstractchopper.h"
#include "teensychopper.h"
#include "joystickinputer.h"
#include "joyflyguicontroller.h"
#include "mainwindow.h"
#include "terminalview.h"
#include "joystickexception.h"

using namespace std;


int main (int argc, char * argv[])
{
    cout << "Hello world!" << std::endl;

    QApplication app(argc, argv);

    try
    {
        CJoyFlyGuiController controller;

        if( argc > 1 )
        {
            string serialDevice = string(argv[1] );
            if( serialDevice == "/test" )
            {
                controller.RunJoystickTests();

                return 0;
            }
        }

        CTerminalView terminalView(&controller);
        controller.AddView( &terminalView );
        MainWindow guiView(&controller);
        controller.AddView( &guiView );
        app.exec();
        controller.Quit();
	}
	catch( JoystickException& jExcept )
	{
		cout << "Joystick error:  " << jExcept.what() << endl;
		return -2;
	}
}


