#include <string>
#include "controllerinputer.h"
#include "c-joy-fly-view.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(IControllerInputer* pController, QWidget *parent) :
    CJoyFlyView(pController),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(ChangeVoltage(float)), this,SLOT(onChangeVoltage(float)));
    connect(this,SIGNAL(AppendLog(QString)), this,SLOT(onAppendLog(QString)));
    connect(this,SIGNAL(Ping(float)), this,SLOT(onPing(float)));
    connect(this,SIGNAL(OnThrottleChange(int)), ui->throttleControl,SLOT(setValue(int)));
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

void MainWindow::onChangeVoltage(float newVoltage)
{
    ui->voltage->setValue(newVoltage);
}

void MainWindow::onPing(float latency)
{
    ui->latency->display(latency);
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
    OnChopperMessage(szMsg);
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
    _pController->Start( ui->serialDevice->text().toStdString(), ui->secondsUpdate->value());
    ui->connectButton->setEnabled(false);
}
