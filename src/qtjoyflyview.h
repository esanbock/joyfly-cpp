#ifndef CQTJOYFLYVIEW_H
#define CQTJOYFLYVIEW_H

#include <QMainWindow>

class CQtJoyFlyView : public CJoyFlyView
{
public:
    CQtJoyFlyView(MainWindow* pMainWindow);
    virtual ~CQtJoyFlyView();

    virtual void OnChopperMessage( const char* szMsg );
    virtual void OnDebugMessage( const char* szMsg );
    virtual void OnVoltageChange( float newVoltage );
    virtual void OnPing( float latency );
    virtual void Sent(const char* szMsg);
private:
    MainWindow* _pMainWindow;

signals:

public slots:
};

#endif // CQTJOYFLYVIEW_H
