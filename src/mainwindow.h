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
    void AppendLog(const char* szLog);
    void ChangeVoltage( float newVoltage );
    void Ping( float latency );

protected slots:
    void onAppendLog(const char* szLog);
    void onChangeVoltage( float newVoltage );
    void onPing( float latency );
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
