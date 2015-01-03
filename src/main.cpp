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
#include "SDL2/SDL.h"
#include "c-joy-test.h"
#include "SerialStream.h"
#include "SerialPort.h"

const int JOYSTICK_X = 0;
const int JOYSTICK_Y = 1;
const int JOYSTICK_Z = 2;
const int JOYSTICK_THROTTLE = 3;
const int JOYSTICK_AUTOPILOT = 4;
const int JOYSTICK_HOME = 1;
const int JOYSTICK_STATUS = 8;
const int JOYSTICK_VOLTAGE = 5;

int lastPingNum;
clock_t sentPingClock;
int currentHatY;

int prevVals[4] = {0,0,0,127};
int curVals[4]  = {0,0,0,127};

int prevButtonVals[5];
int curButtonVals[5];
bool buttonToggle[5];

using namespace std;
using namespace LibSerial;

int AxisCommandSimple( SerialPort& serialPort, CJoyTest& sidewinder, const char* szCommand, int joyAxis, int min, int max )
{
	curVals[joyAxis] = sidewinder.GetAxisNormalized (joyAxis, min, max);
	if( prevVals[joyAxis] != curVals[joyAxis] )
	{
		stringstream sstream;
		sstream << szCommand << setfill('0') << setw(3) << curVals[joyAxis];
		string command = sstream.str();
		cout << command << endl;
		serialPort.Write( command );
	}
	prevVals[joyAxis] = curVals[joyAxis];
    
    return prevVals[joyAxis];
}

int ButtonCommandToggle( SerialPort& serialPort, CJoyTest& sidewinder, const char* szCommand, int joyAxis, int ifTrue, int ifFalse )
{
	curButtonVals[joyAxis] = sidewinder.GetButton (joyAxis);
	if( prevButtonVals[joyAxis] != curButtonVals[joyAxis] && curButtonVals[joyAxis] == 1 )
	{
		buttonToggle[joyAxis] = !buttonToggle[joyAxis];
		stringstream sstream;
		sstream << szCommand << setfill('0') << setw(3) << (buttonToggle[joyAxis] ? ifTrue : ifFalse);
		string command = sstream.str();
		cout << command << endl;
		serialPort.Write( command );
	}
	prevButtonVals[joyAxis] = curButtonVals[joyAxis];
    return prevButtonVals[joyAxis];
}

int ButtonCommandToggle( SerialPort& serialPort, CJoyTest& sidewinder, const char* szCommand, int joyAxis )
{
	curButtonVals[joyAxis] = sidewinder.GetButton (joyAxis);
	if( prevButtonVals[joyAxis] != curButtonVals[joyAxis] && curButtonVals[joyAxis] == 1 )
	{
		buttonToggle[joyAxis] = !buttonToggle[joyAxis];
		stringstream sstream;
		sstream << szCommand;
		string command = sstream.str();
		cout << command << endl;
		serialPort.Write( command );
	}
	prevButtonVals[joyAxis] = curButtonVals[joyAxis];
    return prevButtonVals[joyAxis];
}



int HatCommandIncrement( SerialPort& serialPort, CJoyTest& sidewinder, Uint8 down, Uint8 up, const char* szCommand, 
                         int lowVal, int highVal )
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
		currentHatY++;
	}
	else if( hatVal & down )
	{
		currentHatY--;
	}
	else
	{
		return -1;
	}

	currentHatY = min( highVal, max( lowVal, currentHatY) );

	stringstream sstream;
	sstream << szCommand << setfill('0') << setw(3) << currentHatY;
	string command = sstream.str();
	cout << command << endl;
	serialPort.Write( command );
	
	return currentHatY;
}

void SendPing(SerialPort& serialPort)
{
	sentPingClock = clock();
	lastPingNum++;
	if( lastPingNum > 999 )
		lastPingNum = 1;

	stringstream sstream;
	sstream << ":E" << setfill('0') << setw(3) << lastPingNum;
	string command = sstream.str();
	cout << command << endl;
	serialPort.Write( command );
}

void ProcessPingResponse( string& line )
{
	int pingResponse = atoi( line.substr(3).c_str() );
	if( pingResponse != lastPingNum )
	{
		cout << "unmatched ping " << pingResponse << ".  Expected " << lastPingNum << endl;
		return;
	}
	float latency = (float)(clock() - sentPingClock ) / CLOCKS_PER_SEC ;
	cout << "latency = " << latency << ": " << " " << clock() << "," << sentPingClock << endl; ;
}

void ProcessCommandResponse( string& line )
{
	if( line.compare(0,3,":ER") == 0 )
	{
		ProcessPingResponse( line );
		return;
	}

	// otherwise, NAK
	cout << "unrecognized command from chopper" << endl;
}

int main (int argc, char * const argv[])
{
	cout << "Hello world!" << std::endl;

	if( argc < 2 )
	{
		cout << "wrong number of parameters" << endl;
		return -1;
	}
    
    int secondsUpdate = atoi(argv[1]);

	lastPingNum = 3;
	sentPingClock = clock();

	string serialDevice = string(argv[1] );

	cout << "Opening joystick" << endl;
	try
	{
		CJoyTest sidewinder(0);
		SDL_JoystickUpdate();

		if( serialDevice == "/test" )
		{
			sidewinder.RunTests();
			return 0;
		}

		cout << "Opening serial port " << serialDevice << endl;
		SerialPort comPort( serialDevice );
		comPort.Open( SerialPort::BAUD_9600, SerialPort::CHAR_SIZE_8, SerialPort::PARITY_NONE, SerialPort::STOP_BITS_1) ;
	
		bool quitting = false;

		clock_t lastTime = clock();
		//bool flipper = false;


		do
		{
			SDL_JoystickUpdate();

			AxisCommandSimple( comPort, sidewinder, ":T", JOYSTICK_THROTTLE, 255, 0);
			AxisCommandSimple( comPort,sidewinder, ":B", JOYSTICK_X, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
			AxisCommandSimple( comPort,sidewinder, ":P", JOYSTICK_Y, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
			AxisCommandSimple( comPort,sidewinder, ":Y", JOYSTICK_Z, -255, 255);
			ButtonCommandToggle( comPort, sidewinder, ":N", JOYSTICK_AUTOPILOT, 0, 1 );
			ButtonCommandToggle( comPort, sidewinder, ":H", JOYSTICK_HOME);
			ButtonCommandToggle( comPort, sidewinder, ":S", JOYSTICK_STATUS);
			ButtonCommandToggle( comPort, sidewinder, ":V", JOYSTICK_VOLTAGE);

			HatCommandIncrement( comPort, sidewinder, SDL_HAT_DOWN, SDL_HAT_UP, ":L", -20, 20 );
		
			if( comPort.IsDataAvailable() )
			{
				try
				{
					string line = comPort.ReadLine( 200 );

					if( line[0] == ':' )
						ProcessCommandResponse(line);

					cout << line << endl;
				}
				catch( SerialPort::ReadTimeout& timeout )
				{
					cerr << "I got nothing" << endl;
				}
			}

			clock_t now = clock();



			if( now - lastTime >= CLOCKS_PER_SEC * secondsUpdate )
			{
				lastTime = clock();
				SendPing(comPort);
			}

		} while( !quitting );

		return 0;
	}
	catch( JoystickException& jExcept )
	{
		cout << "Joystick error:  " << jExcept.what() << endl;
		return -2;
	}
}


