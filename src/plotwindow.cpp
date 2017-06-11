#include <QColor>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include "timeseries.h"
#include "joyflyview.h"
#include "plotwindow.h"
#include "ui_plotwindow.h"


PlotWindow::PlotWindow(CJoyFlyGuiController* pController,QWidget *parent) :
    QDialog(parent),
    CGraphView(pController),
   ui(new Ui::PlotWindow)
{
    ui->setupUi(this);

    connect(this,SIGNAL(OnVoltageChange(float)), this,SLOT(onChangeVoltage(float)));
    connect(this,SIGNAL(OnIMU(const int, const int, const int)), this,SLOT(onIMU(const int, const int, const int)));

    _pTempCurve = new QwtPlotCurve("Temperature");
    _pTempCurve->attach(ui->qwtPlot);

    // IMU X
    _pCurve_imu[0] = new QwtPlotCurve("IMU X");
    _pCurve_imu[0]->attach(ui->qwtPlot_PIDx);
    _pCurve_imu[0]->setPen(Qt::red);

    // IMU Y
    _pCurve_imu[1] = new QwtPlotCurve("IMU Y");
    _pCurve_imu[1]->attach(ui->qwtPlot_PIDy);
    _pCurve_imu[1]->setPen(Qt::red);

    // HEADING X
    _pCurve_heading[0] = new QwtPlotCurve("HEADING X");
    _pCurve_heading[0]->attach(ui->qwtPlot_PIDx);
    _pCurve_heading[0]->setPen(Qt::green);

    // HEADING Y
    _pCurve_heading[1] = new QwtPlotCurve("HEADING Y");
    _pCurve_heading[1]->attach(ui->qwtPlot_PIDy);
    _pCurve_heading[1]->setPen(Qt::green);

    // Motors X
    _pCurve_motor[0] = new QwtPlotCurve("MOTOR X");
    _pCurve_motor[0]->attach(ui->qwtPlot_PIDx);
    _pCurve_motor[0]->setPen(Qt::blue);

    // Motors Y
    _pCurve_motor[1] = new QwtPlotCurve("MOTOR Y");
    _pCurve_motor[1]->attach(ui->qwtPlot_PIDy);
    _pCurve_motor[1]->setPen(Qt::blue);

}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::onChangeVoltage( float  )
{
    _pTempCurve->setSamples(Controller().GetVoltageHistory()->GetTimes(),
                            Controller().GetVoltageHistory()->GetVals(),
                            Controller().GetVoltageHistory()->GetSize() );

    ui->qwtPlot->replot();

}

void PlotWindow::onIMU( const int , const int , const int  )
{
    // X
    _pCurve_imu[0]->setSamples(Controller().GetIMUHistory()[0].GetTimes(), Controller().GetIMUHistory()[0].GetVals(), Controller().GetIMUHistory()[0].GetSize());
    _pCurve_heading[0]->setSamples(Controller().GetHeadingHistory()[0].GetTimes(), Controller().GetHeadingHistory()[0].GetVals(), Controller().GetHeadingHistory()[0].GetSize());
    _pCurve_motor[0]->setSamples(Controller().GetMotorHistory()[0].GetTimes(), Controller().GetMotorHistory()[0].GetVals(), Controller().GetMotorHistory()[0].GetSize());
    ui->qwtPlot_PIDx->replot();

    // Y
    _pCurve_imu[1]->setSamples(Controller().GetIMUHistory()[1].GetTimes(), Controller().GetIMUHistory()[1].GetVals(), Controller().GetIMUHistory()[1].GetSize());
    _pCurve_heading[1]->setSamples(Controller().GetHeadingHistory()[1].GetTimes(), Controller().GetHeadingHistory()[1].GetVals(), Controller().GetHeadingHistory()[1].GetSize());
    _pCurve_motor[1]->setSamples(Controller().GetMotorHistory()[1].GetTimes(), Controller().GetMotorHistory()[1].GetVals(), Controller().GetMotorHistory()[0].GetSize());
    ui->qwtPlot_PIDy->replot();

    // Z
}

void PlotWindow::on_counterPidXkP_valueChanged(double value)
{
    Controller().GetChopper().ChangePid(0,value, 10, 10);
}

void PlotWindow::on_pushClearX_clicked()
{
     Controller().GetIMUHistory()[0].Clear();
     Controller().GetHeadingHistory()[0].Clear();
     Controller().GetMotorHistory()[0].Clear();
}

void PlotWindow::on_counterPidYkP_valueChanged(double value)
{
    Controller().GetChopper().ChangePid(1,value, 10, 10);
}
