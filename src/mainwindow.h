#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "c-joy-fly-view.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public CJoyFlyView
{
    Q_OBJECT

public:
    explicit MainWindow(IControllerInputer* pController, QWidget *parent=0);
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
    virtual void OnThrottleChange( int newThrottle );
    virtual void OnAutoNav( bool isOn );
    void Debug( QString log );

protected slots:
    void onAppendLog(QString log);
    void onSetAutoPilot(bool isOn);
    void onChangeVoltage( float newVoltage );
    void onPing( float latency );
private slots:
    void on_throttleControl_sliderMoved(int position);
    void on_connectButton_clicked();
    void on_connectJoystick_clicked();

    void on_statusButton_clicked();

    void on_autoNavButton_clicked();

    void on_rehomeButton_clicked();

    void on_connectButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
