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

protected slots:
    virtual void onChangeVoltage( float newThrottle );

private:
    Ui::PlotWindow *ui;

    QwtPlotCurve* _pTempCurve;
};

#endif // PLOTWINDOW_H
