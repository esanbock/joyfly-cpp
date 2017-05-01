#include <string>
#include <thread>
#include <chrono>
#include <vector>

#include "controllerinputer.h"
#include "c-joy-fly-view.h"
#include "abstractchopper.h"
#include "joystickinputer.h"

#include "serialstream.h"
#include "c-joy-fly-controller.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"

using namespace std;

MainWindow::MainWindow(CJoyFlyController* pController, QWidget *parent) :
    CMainView(pController),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _pController = pController;
    ui->setupUi(this);
    connect(this,SIGNAL(ChangeVoltage(float)), this,SLOT(onChangeVoltage(float)));
    connect(this,SIGNAL(AppendLog(QString)), this,SLOT(onAppendLog(QString)));
    connect(this,SIGNAL(Ping(float)), this,SLOT(onPing(float)));
    connect(this,SIGNAL(OnThrottleChange(int)), ui->throttleControl,SLOT(setValue(int)));
    connect(this,SIGNAL(Debug(QString)), ui->textDebug,SLOT(appendPlainText(QString)));
    connect(this,SIGNAL(OnAutoNav(bool)), this,SLOT(onSetAutoPilot(bool)));

    connect(this,SIGNAL(Bank(float)), this,SLOT(on_bank(float)));
    connect(this,SIGNAL(Pitch(float)), this,SLOT(on_pitch(float)));
    connect(this,SIGNAL(Yaw(float)), this,SLOT(on_yaw(float)));

    populatejoysticks();

    initCompass();

    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAppendLog(QString log)
{
    ui->textLog->appendPlainText(log);
    ui->textLog->ensureCursorVisible();
}

void MainWindow::onSetAutoPilot(bool isOn)
{
    ui->autoNavCheck->setChecked(isOn);
}

void MainWindow::onChangeVoltage(float newVoltage)
{
    ui->voltage->setValue(newVoltage);
}

void MainWindow::onPing(float latency)
{
    ui->latency->display(latency);
    if( ui->statusOnPingCheckBox->isChecked())
        _pController->GetStatus();
}

void MainWindow::on_throttleControl_sliderMoved(int position)
{
    _pController->SetThrottle(position);
}

void MainWindow::OnChopperMessage( const char* szMsg )
{
    AppendLog(szMsg);
}

void MainWindow::OnDebugMessage(const char* szMsg )
{
    Debug(szMsg);
}

void MainWindow::Sent(const char* szMsg )
{
    OnDebugMessage(szMsg);
}

void MainWindow::OnVoltageChange( float newVoltage )
{
    ChangeVoltage(newVoltage);
}


void MainWindow::OnPing( float latency )
{
    Ping(latency);
}

void MainWindow::on_connectButton_clicked()
{
    _pController->Connect( ui->serialDevice->text().toStdString(), ui->secondsUpdate->value());
    ui->connectButton->setEnabled(false);
}

void MainWindow::populatejoysticks()
{
    vector<string> joyNames = CJoystickInputer::GetJoystickNames();

    for (auto it = std::begin(joyNames); it != std::end(joyNames); ++it)
    {
        ui->joystickList->addItem(it->c_str());
    }
}

void MainWindow::on_connectJoystick_clicked()
{
    _pController->AddJoyStick( ui->joystickList->currentIndex());
    ui->connectJoystick->setEnabled(false);
}

void MainWindow:: OnBank( float newAngle )
{
    Bank(newAngle);
}

void MainWindow:: OnPitch( float newAngle )
{
    Pitch(newAngle);
}

void MainWindow:: OnYaw( float newAngle )
{
    Yaw(newAngle);
}


void MainWindow::on_bank(float newAngle )
{
    ui->attitude->setAngle(270 - newAngle);
    ui->label_RollAngle->setText(to_string(newAngle).c_str());
}

void MainWindow::on_pitch(float newAngle )
{
    ui->attitude->setGradient((90 - newAngle)/90);
    ui->label_PitchAngle->setText(to_string(newAngle).c_str());
}

void MainWindow::on_yaw(float newAngle)
{
    ui->compass->setValue(newAngle);
    ui->label_Yaw->setText(to_string(newAngle).c_str());
}

void MainWindow::on_statusButton_clicked()
{
    _pController->GetStatus();
}

bool isToggling;
void MainWindow::on_autoNavButton_clicked()
{
    if( !isToggling )
    {
        isToggling = true;
        _pController->ToggleAutoPilot();
    }
    isToggling = false;

}

void MainWindow::on_rehomeButton_clicked()
{
    _pController->SetHome();
}

void MainWindow::on_connectButton_2_clicked()
{
    ui->serialDevice->clear();
    on_connectButton_clicked();
}

void MainWindow::initCompass()
{
    QwtCompassScaleDraw *scaleDraw = new QwtCompassScaleDraw();
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Ticks, true );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Labels, false );
    scaleDraw->enableComponent( QwtAbstractScaleDraw::Backbone, false );
    scaleDraw->setTickLength( QwtScaleDiv::MinorTick, 5 );
    scaleDraw->setTickLength( QwtScaleDiv::MajorTick, 10 );

    ui->compass->setScaleDraw( scaleDraw );

    ui->compass->setScaleMaxMajor( 36 );
    ui->compass->setScaleMaxMinor( 5 );

    ui->compass->setNeedle(new QwtCompassMagnetNeedle( QwtCompassMagnetNeedle::TriangleStyle,
                    Qt::white, Qt::gray));
    ui->compass->setValue( 0 );

}

void MainWindow::on_pushGraph_clicked()
{
    PlotWindow* pPlotWindow = new PlotWindow(_pController, this);
    pPlotWindow->show();
    _pController->AddView(pPlotWindow);
}
