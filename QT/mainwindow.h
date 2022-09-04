#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <QDebug>
#include <QTime>
#include <QQueue>
#include <QPainter>
#include <QMouseEvent>
#include <QMainWindow>
#include <QButtonGroup>
#include "SerialDriver/serialthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define SETTEXT
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Serial_Scan();
    void Register_Uart_thread();
    int SetBacklight(int Level);
    void SensorDrawPlot(QVector<double>,QVector<double>);
    void FunCal(int a[20], int n, int *max, int *min, float *average);

public slots:
    void DisplayData(QByteArray);
    void DisplayValue(Sensor_Data);

private slots:
    void on_ClearUartWindow_clicked();

    void on_PlotCtrl_clicked();
    void on_AlarmCtr_clicked();
    void on_AlarmLightCtr_clicked();
    void on_BellCtrl_clicked();

private:
    Ui::MainWindow *ui;
    QVector <double>Time;
    QVector <double>Data;
    QList <QString> serial_port ;
    int alalysis_value[20];
    SerialThread   *uartWorker = nullptr;
    QThread *uartThread = nullptr;

    bool PlotMode;
    bool AlarmMode;
    bool LedStatus;
    bool BellStatus;
};
#endif // MAINWINDOW_H
