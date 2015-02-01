#ifndef CONTROLLERINPUTER
#define CONTROLLERINPUTER

class IControllerInputer
{
public:
    virtual void DebugMessage( const char* szMsg )=0;
};

#endif // CONTROLLERINPUTER

