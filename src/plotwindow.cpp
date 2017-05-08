#include <QColor>
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
    connect(this,SIGNAL(OnIMU(const int, const int, const int)), this,SLOT(onIMU(const int, const int, const int)));

    _pTempCurve = new QwtPlotCurve("Temperature");
    _pTempCurve->attach(ui->qwtPlot);

    // IMU
    _pCurve_imu_x = new QwtPlotCurve("IMU X");
    _pCurve_imu_x->attach(ui->qwtPlot_PIDx);
    _pCurve_imu_x->setPen(Qt::red);

    // HEADING
    _pCurve_heading_x = new QwtPlotCurve("HEADING X");
    _pCurve_heading_x->attach(ui->qwtPlot_PIDx);
    _pCurve_heading_x->setPen(Qt::green);

    // Motors
    _pCurve_motor_x = new QwtPlotCurve("MOTOR X");
    _pCurve_motor_x->attach(ui->qwtPlot_PIDx);
    _pCurve_motor_x->setPen(Qt::blue);
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::onChangeVoltage( float newVoltage )
{
    _pTempCurve->setSamples(Controller().GetVoltageHistory()->GetTimes(),
                            Controller().GetVoltageHistory()->GetVals(),
                            Controller().GetVoltageHistory()->GetSize() );

    ui->qwtPlot->replot();

}

void PlotWindow::onIMU( const int x, const int y, const int z )
{
    _pCurve_imu_x->setSamples(Controller().GetIMUHistory()[0].GetTimes(), Controller().GetIMUHistory()[0].GetVals(), Controller().GetIMUHistory()[0].GetSize());
    _pCurve_heading_x->setSamples(Controller().GetHeadingHistory()[0].GetTimes(), Controller().GetHeadingHistory()[0].GetVals(), Controller().GetHeadingHistory()[0].GetSize());
    _pCurve_motor_x->setSamples(Controller().GetMotorHistory()[0].GetTimes(), Controller().GetMotorHistory()[0].GetVals(), Controller().GetMotorHistory()[0].GetSize());
    ui->qwtPlot_PIDx->replot();
}

void PlotWindow::on_counterPidXkP_valueChanged(double value)
{
    Controller().GetChopper().ChangePid(value, 10, 10);
}
