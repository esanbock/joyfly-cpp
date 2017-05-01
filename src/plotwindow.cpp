#include "c-joy-fly-view.h"
#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(IControllerInputer* pController,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotWindow),
    CThrottleView(pController)
{
    ui->setupUi(this);
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::OnThrottleChange( int newThrottle )
{

}
