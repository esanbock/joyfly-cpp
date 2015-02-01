#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void OnAppendLog(const char* szLog);
    void OnChangeVoltage( float newVoltage );

protected slots:
    void AppendLog(const char* szLog);
    void changeVoltage( float newVoltage );
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
