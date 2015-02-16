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
    connect(this,SIGNAL(OnAutoNav(bool)), this,SLOT(onSetAutoPilot(bool)));
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

void MainWindow::OnBank(float newAngle )
{
    ui->attitude->setAngle(270 - newAngle);
    ui->label_RollAngle->setText(to_string(newAngle).c_str());
}

void MainWindow::OnPitch(float newAngle )
{
    ui->attitude->setGradient((90 - newAngle)/90);
    ui->label_PitchAngle->setText(to_string(newAngle).c_str());
}

void MainWindow::OnYaw(float newAngle)
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
    typedef QPalette Palette;

    int c;

    Palette colorGroup;
    for ( c = 0; c < Palette::NColorRoles; c++ )
        colorGroup.setColor((Palette::ColorRole)c, QColor());

    colorGroup.setColor(Palette::Base,
                        palette().color(backgroundRole()).light(120));
    colorGroup.setColor(Palette::Foreground,
                        colorGroup.color(Palette::Base));

    ui->compass->setLineWidth(4);
    //ui->compass->setFrameShadow(QwtCompass::Sunken);
    colorGroup.setColor(Palette::Base, Qt::darkBlue);
    colorGroup.setColor(Palette::Foreground,
                        QColor(Qt::darkBlue).dark(120));
    colorGroup.setColor(Palette::Text, Qt::white);

    /*ui->compass->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
            ui->compass->setScaleTicks(1, 1, 3);
            ui->compass->setScale(36, 5, 0);*/
    ui->compass->setScale(0,360);

    ui->compass->setNeedle(
                new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::ThinStyle));
    ui->compass->setValue(220.0);


    QPalette newPalette = ui->compass->palette();
    for ( c = 0; c < Palette::NColorRoles; c++ )
    {
        if ( colorGroup.color((Palette::ColorRole)c).isValid() )
        {
            for ( int cg = 0; cg < QPalette::NColorGroups; cg++ )
            {
                newPalette.setColor(
                            (QPalette::ColorGroup)cg,
                            (Palette::ColorRole)c,
                            colorGroup.color((Palette::ColorRole)c));
            }
        }
    }

    for ( int i = 0; i < QPalette::NColorGroups; i++ )
    {
        QPalette::ColorGroup cg = (QPalette::ColorGroup)i;

        const QColor light =
                newPalette.color(cg, Palette::Base).light(170);
        const QColor dark = newPalette.color(cg, Palette::Base).dark(170);
        const QColor mid = ui->compass->frameShadow() == QwtDial::Raised
                ? newPalette.color(cg, Palette::Base).dark(110)
                : newPalette.color(cg, Palette::Base).light(110);

        newPalette.setColor(cg, Palette::Dark, dark);
        newPalette.setColor(cg, Palette::Mid, mid);
        newPalette.setColor(cg, Palette::Light, light);
    }
    ui->compass->setPalette(newPalette);

}
