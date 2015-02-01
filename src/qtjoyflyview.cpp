#include <QApplication>
#include "mainwindow.h"
#include "c-joy-fly-view.h"
#include "qtjoyflyview.h"

using namespace std;

CQtJoyFlyView::CQtJoyFlyView(MainWindow *pMainWindow)
{
    _pMainWindow = pMainWindow;
    _pMainWindow->show();
}

CQtJoyFlyView::~CQtJoyFlyView()
{

}

void CQtJoyFlyView::OnChopperMessage( const char* szMsg )
{
    _pMainWindow->OnAppendLog(szMsg);

}

void CQtJoyFlyView::OnDebugMessage(const char* szMsg )
{
    _pMainWindow->OnAppendLog(szMsg);
}

void CQtJoyFlyView::OnVoltageChange( float newVoltage )
{
    _pMainWindow->OnChangeVoltage(newVoltage);
}
