#include <vector>
#include <thread>
#include "joystickinputer.h"

using namespace std;

CJoystickInputer::CJoystickInputer(int joyNum, IControllerInputer& controller)
    :_controller(controller),
    _joyNum(joyNum),
    _prevVals {0,0,0,127} ,_curVals{0,0,0,127}
{

}

CJoystickInputer::~CJoystickInputer()
{
    if( _sidewinder != NULL)
        delete _sidewinder;
}

vector<string> CJoystickInputer::GetJoystickNames()
{
    vector<string> results;
    int numSticks = CSdlJoystick::GetJoyCount();
    for( int i=0; i < numSticks; i++ )
    {
        results.push_back(CSdlJoystick::GetJoyName(i));
    }
    return results;
}

void CJoystickInputer::Start()
{
      pSdlLoopThread = new std::thread([this]() {DoSdlLoop();});
}


void CJoystickInputer::Stop()
{
      _quitting = true;
}

void CJoystickInputer::DoSdlLoop()
{
    do
    {
        SDL_JoystickUpdate();
        ProcessJoystickInput();
    }while (!_quitting);
}

int CJoystickInputer::AxisCommandSimple( CSdlJoystick& sidewinder, function<void (int)> axisFunction, int joyAxis, int min, int max )
{
    _curVals[joyAxis] = sidewinder.GetAxisNormalized (joyAxis, min, max);
    if( _prevVals[joyAxis] != _curVals[joyAxis] )
    {
        axisFunction(_curVals[joyAxis]);
    }
    _prevVals[joyAxis] = _curVals[joyAxis];

    return _prevVals[joyAxis];
}

int CJoystickInputer::ButtonCommandToggle(CSdlJoystick& sidewinder, function<void (int)> toggleFunction, int joyAxis)
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



int CJoystickInputer::HatCommandIncrement(CSdlJoystick& sidewinder, Uint8 down, Uint8 up, function<void (int)> axisFunction, int lowVal, int highVal )
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
    axisFunction(_currentHatY);

    return _currentHatY;
}

void CJoystickInputer::OpenJoystick(int joystickNum)
{
    _controller.DebugMessage("Opening joystick");
    _sidewinder = new CSdlJoystick(joystickNum);

}

void CJoystickInputer::ProcessJoystickInput()
{
    AxisCommandSimple( *_sidewinder, [&](int val) {_controller.SetThrottle(val);}, JOYSTICK_THROTTLE, 255, 0);
    AxisCommandSimple( *_sidewinder, [&](int val) {_controller.Bank(val);}, JOYSTICK_X, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
    AxisCommandSimple( *_sidewinder, [&](int val) {_controller.Pitch(val);}, JOYSTICK_Y, 70, 110);  // 90 needs to be middle.  Robot won't let servo kick up at 110 degrees
    AxisCommandSimple( *_sidewinder, [&](int val) {_controller.Yaw(val);}, JOYSTICK_Z, -255, 255);

    ButtonCommandToggle( *_sidewinder, [&] (bool result) {_controller.SetAutoPilot(result);}, JOYSTICK_AUTOPILOT );
    ButtonCommandToggle( *_sidewinder, [&] (bool) {_controller.SetHome();}, JOYSTICK_HOME);
    ButtonCommandToggle( *_sidewinder, [&] (bool) {_controller.GetStatus();}, JOYSTICK_STATUS);
    ButtonCommandToggle( *_sidewinder, [&] (bool) {_controller.GetVoltage();}, JOYSTICK_VOLTAGE);

    HatCommandIncrement( *_sidewinder, SDL_HAT_DOWN, SDL_HAT_UP, [&](int val) {_controller.Lift(val);}, -20, 20 );
}
