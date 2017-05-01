#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include "c-joy-fly-view.h"
#include "plotwindow.h"
#include "ui_plotwindow.h"


PlotWindow::PlotWindow(IControllerInputer* pController,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotWindow),
    CGraphView(pController)
{
    ui->setupUi(this);

    _pTempCurve = new QwtPlotCurve("Temperature");
    //CurveData *data = static_cast<CurveData *>( d_curve->data() );
    //data->append( point );
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::OnThrottleChange( int newThrottle )
{
}

void PlotWindow::OnVoltageChange( float newVoltage )
{
    _yVals.push_back((double)newVoltage);
    _xVals.push_back(clock());

    _pTempCurve->setSamples(&_xVals[0], &_yVals[0], _yVals.size() );

    _pTempCurve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();

}
