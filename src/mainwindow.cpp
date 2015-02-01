#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(OnChangeVoltage(float)), this,SLOT(changeVoltage(float)));
    connect(this,SIGNAL(OnAppendLog(const char*)), this,SLOT(AppendLog(const char*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AppendLog(const char* szLog)
{
    ui->textLog->appendPlainText(szLog);
}

void MainWindow::changeVoltage(float newVoltage)
{
    ui->voltage->setValue(newVoltage);
}
