#include <thread>
#include "joystickinputer.h"

using namespace std;

CJoystickInputer::CJoystickInputer(int joyNum, IControllerInputer& controller)
    :_controller(controller),
    _prevVals {0,0,0,127} ,_curVals{0,0,0,127}
{

}

CJoystickInputer::~CJoystickInputer()
{

}

void CJoystickInputer::Start()
{
      pSdlLoopThread = new std::thread([this]() {DoSdlLoop();});
}

void CJoystickInputer::DoSdlLoop()
{
    do
    {
        SDL_JoystickUpdate();
        ProcessJoystickInput();
    }while (!_quitting);
}

int CJoystickInputer::AxisCommandSimple( CJoyTest& sidewinder, const char* szCommand, int joyAxis, int min, int max )
{
    _curVals[joyAxis] = sidewinder.GetAxisNormalized (joyAxis, min, max);
    if( _prevVals[joyAxis] != _curVals[joyAxis] )
    {
        control.SendSimpleCommand(szCommand, _curVals[joyAxis]);
    }
    _prevVals[joyAxis] = _curVals[joyAxis];

    return _prevVals[joyAxis];
}

int CJoystickInputer::ButtonCommandToggle(CJoyTest& sidewinder, function<void (int)> toggleFunction, int joyAxis)
{
    _curButtonVals[joyAxis] = sidewinder.GetButton (joyAxis);
    if( _prevButtonVals[joyAxis] != _curButtonVals[joyAxis] && _curButtonVals[joyAxis] == 1 )
    {
        _buttonToggle[joyAxis] = !_buttonToggle[joyAxis];

        toggleFunction(_buttonToggle[joyAxis]);
    }
    _prevButtonVals[joyAxis] = _curButtonVals[joyAxis];
    return _prevButtonVals[joyAxis];
}



int CJoystickInputer::HatCommandIncrement(CJoyTest& sidewinder, Uint8 down, Uint8 up, const char* szCommand, int lowVal, int highVal )
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

void CJoystickInputer::OpenJoystick(int joystickNum)
{
    _controller.DebugMessage("Opening joystick");
    _sidewinder = new CJoyTest(joystickNum);

}

void CJoystickInputer::RunJoystickTests()
{
    _sidewinder->RunTests();
}

void CJoystickInputer::ProcessJoystickInput()
{
    AxisCommandSimple( *_sidewinder, ":T", JOYSTICK_THROTTLE, 255, 0);
    AxisCommandSimple( *_sidewinder, ":B", JOYSTICK_X, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
    AxisCommandSimple( *_sidewinder, ":P", JOYSTICK_Y, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
    AxisCommandSimple( *_sidewinder, ":Y", JOYSTICK_Z, -255, 255);

    ButtonCommandToggle( *_sidewinder, [&] (bool result) {_controller.SetAutoPilot(result);}, JOYSTICK_AUTOPILOT );
    ButtonCommandToggle( *_sidewinder, [&] (bool) {_controller.SetHome();}, JOYSTICK_HOME);
    ButtonCommandToggle( *_sidewinder, [&] (bool) {_controller.GetStatus();}, JOYSTICK_STATUS);
    ButtonCommandToggle( *_sidewinder, [&] (bool) {_controller.GetVoltage();}, JOYSTICK_VOLTAGE);

    HatCommandIncrement( *_sidewinder, SDL_HAT_DOWN, SDL_HAT_UP, ":L", -20, 20 );
}
