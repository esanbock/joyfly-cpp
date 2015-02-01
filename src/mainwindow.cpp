#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(ChangeVoltage(float)), this,SLOT(onChangeVoltage(float)));
    connect(this,SIGNAL(AppendLog(const char*)), this,SLOT(onAppendLog(const char*)));
    connect(this,SIGNAL(Ping(float)), this,SLOT(onPing(float)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAppendLog(const char* szLog)
{
    ui->textLog->appendPlainText(szLog);
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

}
