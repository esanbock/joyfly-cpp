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

public:
    explicit PlotWindow(CJoyFlyGuiController* pController, QWidget *parent = 0);
    ~PlotWindow();

    virtual void OnThrottleChange( int newThrottle );

signals:
    virtual void OnVoltageChange( float newThrottle );
    virtual void OnIMU( const int x, const int y, const int z );

protected slots:
    virtual void onChangeVoltage( float newThrottle );
    virtual void onIMU( const int x, const int y, const int z );

private:
    Ui::PlotWindow *ui;

    QwtPlotCurve* _pTempCurve;
    QwtPlotCurve* _pCurve_imu_x;
};

#endif // PLOTWINDOW_H
