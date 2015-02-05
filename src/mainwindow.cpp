#include <string>
#include "controllerinputer.h"
#include "c-joy-fly-view.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "joystickinputer.h"

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
    connect(this,SIGNAL(Debug(QString)), ui->textDebug,SLOT(appendPlainText(QString)));

    populatejoysticks();

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

void MainWindow::OnBank(float newAngle )
{
    //ui->attitude->setAngle(newAngle);
}

void MainWindow::OnPitch(float newAngle )
{
    ui->attitude->setGradient(newAngle);
}
