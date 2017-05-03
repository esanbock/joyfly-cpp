#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include "timeseries.h"
#include "joyflyview.h"
#include "plotwindow.h"
#include "ui_plotwindow.h"


PlotWindow::PlotWindow(CJoyFlyGuiController* pController,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotWindow),
    CGraphView(pController)
{
    ui->setupUi(this);

    connect(this,SIGNAL(OnVoltageChange(float)), this,SLOT(onChangeVoltage(float)));


    _pTempCurve = new QwtPlotCurve("Temperature");
    _pTempCurve->attach(ui->qwtPlot);
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::OnThrottleChange( int newThrottle )
{
}

void PlotWindow::onChangeVoltage( float newVoltage )
{
    _pTempCurve->setSamples(Controller().GetVoltageHistory()->GetTimes(),
                            Controller().GetVoltageHistory()->GetVals(),
                            Controller().GetVoltageHistory()->GetSize() );

//    _pTempCurve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();

}
