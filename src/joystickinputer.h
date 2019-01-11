#ifndef CJOYSTICKINPUTER_H
#define CJOYSTICKINPUTER_H


#include "SDL2/SDL.h"
#include <thread>
#include <functional>

#include "joyflycontroller.h"
#include "sdljoystick.h"

using namespace std;

class CJoystickInputer
{

private:
    thread* pSdlLoopThread;
    static const int JOYSTICK_X = 0;
    static const int JOYSTICK_Y = 1;
    static const int JOYSTICK_Z = 2;
    static const int JOYSTICK_THROTTLE = 3;
    static const int JOYSTICK_AUTOPILOT = 4;
    static const int JOYSTICK_HOME = 1;
    static const int JOYSTICK_STATUS = 8;
    static const int JOYSTICK_VOLTAGE = 5;
    bool _quitting=false;
    int _currentHatY;
    bool _buttonToggle[10];
    CSdlJoystick* _sidewinder = NULL;
    int _joyNum = -1;
    int _prevVals[10];
    int _curVals[10];
    int _prevButtonVals[10];
    int _curButtonVals[10];
    IJoyflyController& _controller;

protected:
    void DoSdlLoop();
    int AxisCommandSimple( CSdlJoystick& sidewinder, function<void (int)> axisFunction, int joyAxis, int min, int max );
    int HatCommandIncrement( CSdlJoystick& sidewinder, Uint8 down, Uint8 up, function<void (int)> axisFunction, int lowVal, int highVal );
    int ButtonCommandToggle(CSdlJoystick& sidewinder, function<void (int)> toggleFunction, int joyAxis);
    void ProcessJoystickInput();
    void OpenJoystick(int joystickNum);

public:
    CJoystickInputer(int joyNum, IJoyflyController& controller);
    ~CJoystickInputer();
    void Start();
    void Stop();
    static vector<string> GetJoystickNames();

};

#endif // CJOYSTICKINPUTER_H
