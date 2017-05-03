#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <chrono>

#include <QMainWindow>
#include "abstractchopper.h"
#include "joystickinputer.h"
#include "serialstream.h"
#include "joyflyview.h"
#include "joyflyguicontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public CMainView
{
    Q_OBJECT

public:
    explicit MainWindow(CJoyFlyGuiController* pController, QWidget *parent=0);
    ~MainWindow();
    virtual void OnChopperMessage( const char* szMsg );
    virtual void OnDebugMessage( const char* szMsg );
    virtual void OnVoltageChange( float newVoltage );

    virtual void OnPing( float latency );
    virtual void Sent(const char* szMsg);
    virtual void OnBank( float newAngle );
    virtual void OnPitch( float newAngle );
    virtual void OnYaw( float newAngle );

protected:
    void populatejoysticks();
    void initCompass();

signals:
    void AppendLog(QString log);
    void ChangeVoltage( float newVoltage );
    void Ping( float latency );
    void OnThrottleChange( int newThrottle );
    void OnAutoNav( bool isOn );
    void Debug( QString log );
    void Bank( float latency );
    void Pitch( float latency );
    void Yaw( float latency );
    void OnCollective( double collective );

protected slots:
    void onAppendLog(QString log);
    void onSetAutoPilot(bool isOn);
    void onChangeVoltage( float newVoltage );
    void onPing( float latency );

    void on_bank( float latency );
    void on_pitch( float latency );
    void on_yaw( float latency );
private slots:
    void on_throttleControl_sliderMoved(int position);
    void on_connectButton_clicked();
    void on_connectJoystick_clicked();

    void on_statusButton_clicked();

    void on_autoNavButton_clicked();

    void on_rehomeButton_clicked();

    void on_connectButton_2_clicked();

    void on_pushGraph_clicked();
    void on_collective(double collective);

private:
    Ui::MainWindow *ui;
    CJoyFlyGuiController* _pController;
};

#endif // MAINWINDOW_H
