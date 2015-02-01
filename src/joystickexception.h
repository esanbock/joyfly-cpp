#ifndef JOYSTICKEXCEPTION_H

using namespace std;

class JoystickException: public exception
{
    private:
        string _msg;
    public:
    JoystickException( string msg ) throw()
    {
        _msg = msg;
    }

    virtual ~JoystickException() throw()
    {
    }

    virtual const char* what() const throw()
    {
        return _msg.c_str();
    }
} ;

#define JOYSTICKEXCEPTION_H

#endif // JOYSTICKEXCEPTION_H
