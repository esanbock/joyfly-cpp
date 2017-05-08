/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * c-joy-fly-view.h
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

#ifndef _C_JOY_FLY_VIEW_H_
#define _C_JOY_FLY_VIEW_H_

#include <string>
#include "joyflycontroller.h"

class CJoyFlyView
{
private:
    IJoyflyController* _pController;
protected:
    CJoyFlyView(IJoyflyController* pController){_pController = pController;}

    virtual const char* Name(){return "your mom";}

    IJoyflyController& Controller()
    {
        return *_pController;
    }
public:
    virtual void OnIMU( const int , const int , const int ){}
    virtual void OnBank( float ){}
    virtual void OnPitch( float ){}
    virtual void OnYaw( float ){}
    virtual void OnVoltageChange( float newVoltage )=0;
};

class CGraphView : public CJoyFlyView
{
public:
    CGraphView(IJoyflyController* pController)
        :CJoyFlyView(pController){}
    virtual void OnThrottleChange( int newThrottle )=0;
};

class CMainView : public CJoyFlyView
{
public:
    CMainView(IJoyflyController* pController)
        :CJoyFlyView(pController){}

	virtual void OnChopperMessage( const char* szMsg )=0;
	virtual void OnDebugMessage( const char* szMsg )=0;
    virtual void OnPing( float latency )=0;
    virtual void OnAutoNav( bool ){}
    virtual void OnCollective( double ){}

};

#endif // _C_JOY_FLY_VIEW_H_

