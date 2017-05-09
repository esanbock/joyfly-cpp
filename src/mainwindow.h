#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <chrono>

#include <QMainWindow>
#include "abstractchopper.h"
#include "joystickinputer.h"
#include "joyflyview.h"
#include "joyflyguicontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public CMainView
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    CJoyFlyGuiController* _pController;

public:
    explicit MainWindow(CJoyFlyGuiController* pController, QWidget *parent=0);
    ~MainWindow();
    void OnChopperMessage( const char* szMsg );
    void OnDebugMessage( const char* szMsg );
signals:
    void OnVoltageChange( float newVoltage );
    void AppendLog(QString log);
    void OnPing( float latency );
    void OnThrottleChange( int newThrottle );
    void OnAutoNav( bool isOn );
    void SendDebugMessage( QString log );
    void OnBank( float latency );
    void OnPitch( float latency );
    void OnYaw( float latency );
    void OnCollective( double collective );
    void OnBankControl( double val );

protected:
    void populatejoysticks();
    void initCompass();

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
    void on_pushGraph_clicked();
    void on_collective(double collective);

    void on_simulateButton_clicked();
};

#endif // MAINWINDOW_H
