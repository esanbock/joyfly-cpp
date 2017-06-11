#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <vector>
#include "joyflyguicontroller.h"

#include <QDialog>
#include <qwt_plot_curve.h>


namespace Ui {
class PlotWindow;
}

class PlotWindow : public QDialog, public CGraphView
{
    Q_OBJECT


private:
    Ui::PlotWindow *ui;

    QwtPlotCurve* _pTempCurve;
    QwtPlotCurve* _pCurve_imu[3];
    QwtPlotCurve* _pCurve_heading[3];
    QwtPlotCurve* _pCurve_motor[3];

public:
    explicit PlotWindow(CJoyFlyGuiController* pController, QWidget *parent = 0);
    ~PlotWindow();


signals:
    void OnVoltageChange( float newThrottle );
    void OnIMU( const int x, const int y, const int z );
    void OnThrottleChange( int newThrottle );

protected slots:
    virtual void onChangeVoltage( float newThrottle );
    virtual void onIMU( const int x, const int y, const int z );

private slots:
    void on_counterPidXkP_valueChanged(double value);
    void on_pushClearX_clicked();
    void on_counterPidYkP_valueChanged(double value);
};

#endif // PLOTWINDOW_H
