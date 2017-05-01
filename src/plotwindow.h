#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <vector>
#include "c-joy-fly-view.h"

#include <QDialog>
#include <qwt_plot_curve.h>

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QDialog, public CGraphView
{
    Q_OBJECT

public:
    explicit PlotWindow(IControllerInputer* pController, QWidget *parent = 0);
    ~PlotWindow();

    virtual void OnThrottleChange( int newThrottle );
    virtual void OnVoltageChange( float newThrottle );

private:
    Ui::PlotWindow *ui;

    QwtPlotCurve* _pTempCurve;
    std::vector<double> _xVals;
    std::vector<double> _yVals;
};

#endif // PLOTWINDOW_H
