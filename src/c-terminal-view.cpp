/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * c-terminal-view.cpp
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

#include <iostream>
#include "c-terminal-view.h"

using namespace std;

void CTerminalView::OnChopperMessage( const char* szMsg )
{
	cout << szMsg << endl;
}

void CTerminalView::OnDebugMessage(const char* szMsg )
{
	cout << szMsg << endl;
}

void CTerminalView::OnVoltageChange( float newVoltage )
{
    cout << "new voltage:" << newVoltage << endl;
}

void CTerminalView::OnPing( float latency )
{
    cout << "ping latency " << latency << endl;
}
