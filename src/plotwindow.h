#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "c-joy-fly-view.h"

#include <QDialog>
#include <qwt_plot_curve.h>

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QDialog, public CThrottleView
{
    Q_OBJECT

public:
    explicit PlotWindow(IControllerInputer* pController, QWidget *parent = 0);
    ~PlotWindow();

    virtual void OnThrottleChange( int newThrottle );

private:
    Ui::PlotWindow *ui;

    QwtPlotCurve* _pTempCurve;
};

#endif // PLOTWINDOW_H
