#ifndef CJOYSTICKINPUTER_H
#define CJOYSTICKINPUTER_H


#include "SDL2/SDL.h"
#include <thread>

#include "controllerinputer.h"
#include "sdljoystick.h"

using namespace std;

class CJoystickInputer
{
public:
    CJoystickInputer(int joyNum, IControllerInputer& controller);
    ~CJoystickInputer();
    void Start();
    void Stop();
    static vector<string> GetJoystickNames();

protected:
    void DoSdlLoop();
    int AxisCommandSimple( CSdlJoystick& sidewinder, function<void (int)> axisFunction, int joyAxis, int min, int max );
    int HatCommandIncrement( CSdlJoystick& sidewinder, Uint8 down, Uint8 up, function<void (int)> axisFunction, int lowVal, int highVal );
    int ButtonCommandToggle(CSdlJoystick& sidewinder, function<void (int)> toggleFunction, int joyAxis);
    void ProcessJoystickInput();
    void OpenJoystick(int joystickNum);

private:
    IControllerInputer& _controller;
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

    int _prevVals[4];
    int _curVals[4];

    int _prevButtonVals[5];
    int _curButtonVals[5];
    bool _buttonToggle[5];
    CSdlJoystick* _sidewinder = NULL;
    int _joyNum = -1;
};

#endif // CJOYSTICKINPUTER_H
