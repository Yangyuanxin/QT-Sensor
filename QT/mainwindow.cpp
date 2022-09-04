#include "mainwindow.h"
#include "ui_mainwindow.h"

Sensor_Data sensor_data;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    qRegisterMetaType<Sensor_Data>("Sensor_Data");
    Register_Uart_thread();

    this->SetBacklight(3);
    this->PlotMode = true;
    this->AlarmMode = false;
    this->LedStatus = false;
    this->BellStatus = false;
    memset(alalysis_value, 0, sizeof(alalysis_value));

    /*曲线显示区域参数设置*/
    ui->curveShowWidget->setInteractions(QCP::iRangeDrag |QCP::iRangeZoom | QCP::iSelectAxes |
                                         QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->curveShowWidget->legend->setSelectableParts(QCPLegend::spItems);

    ui->curveShowWidget->addGraph();
    ui->curveShowWidget->graph(0)->setPen(QPen(Qt:: red));
    ui->curveShowWidget->yAxis->setLabel(QString(tr("浓度值PPM")));
    ui->curveShowWidget->xAxis->setLabel(QString(tr("时间(小时:分钟:秒)")));
    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime); //时间日期作为X轴
    timeTicker->setDateTimeFormat("hh:mm:ss");
    ui->curveShowWidget->xAxis->setTicker(timeTicker);
    connect(ui->curveShowWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->curveShowWidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->curveShowWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->curveShowWidget->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::DisplayData(QByteArray data)
{
    ui->textBrowser->append(QString("<font color=\"#00FF00\">%1</font> ").
                            arg("[" + QTime::currentTime().toString("hh:mm:ss") + "]收→◆ " + data));
}

void MainWindow::SensorDrawPlot(QVector<double> Time,QVector<double> SensorValue)
{
    ui->curveShowWidget->graph(0)->setData(Time, SensorValue);
    ui->curveShowWidget->graph(0)->rescaleAxes();
    ui->curveShowWidget->replot(QCustomPlot::rpQueuedReplot);
}

void MainWindow::DisplayValue(Sensor_Data info)
{
    static uint8_t Counter = 0;
    int max = 0, min = 0;
    float average = 0.0;
    int Value = info.GasHighBit << 8 | info.GasLowBit;
    ui->SensorValue->display(Value);
    if(this->PlotMode)
    {
        double Key = QDateTime::currentDateTime()
                .toMSecsSinceEpoch() / 1000.0;

        if(Data.length() > 50)
        {
            this->Time.pop_front();
            this->Data.pop_front();
        }

        this->Time.append(Key);
        this->Data.append(Value);
        this->SensorDrawPlot(Time, Data);
    }

    if(ARR_LEN(alalysis_value) == Counter)
    {
        FunCal(alalysis_value, ARR_LEN(alalysis_value),
               &max, &min, &average);
        ui->MaxValue->display(QString::number(max));
        ui->MinValue->display(QString::number(min));
        ui->AverageValue->display(QString::number(average));
        ui->DifferValue->display(QString::number(max - min));
        memset(alalysis_value, 0, sizeof(alalysis_value));
        Counter = 0;
    }

    alalysis_value[Counter] = Value;
    Counter++;
}

/*串口扫描*/
void MainWindow::Serial_Scan()
{
    QString Serial_Port_Name ;
    serial_port.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        Serial_Port_Name = info.portName();
        if(serial.open(QIODevice::ReadWrite))
        {
            serial_port.append(Serial_Port_Name);
            serial.close();
        }
    }
}

/*注册串口线程*/
void MainWindow::Register_Uart_thread()
{
    int index = 0;
    this->Serial_Scan();
    //为了支持嵌入式Linux平台
    for(int i = 0; i < serial_port.length(); i++)
    {
        if(serial_port.at(i).contains("ttyA") || serial_port.at(i).contains("COM"))
            index = i;
    }

    uartThread = new QThread(this);
    uartWorker  = new SerialThread(serial_port.at(index),115200);
    uartWorker->moveToThread(uartThread);
    connect(uartThread, &QThread::finished, uartWorker, &QObject::deleteLater);
    connect(uartWorker, &SerialThread::DataSend, this, &MainWindow::DisplayData);
    connect(uartWorker, &SerialThread::DataAnalyze, this, &MainWindow::DisplayValue);
    uartThread->start();
    uartThread->setPriority(QThread::TimeCriticalPriority);
}

void MainWindow::on_ClearUartWindow_clicked()
{
    ui->textBrowser->clear();
}

MainWindow::~MainWindow()
{
    if(uartThread != nullptr)
    {
        uartThread->quit();
        uartThread->wait();
    }
    delete ui;
}

void MainWindow::on_PlotCtrl_clicked()
{
    static uint8_t status = 0;
    switch(status)
    {
    case 0:
        status = 1;
        this->PlotMode = false;
        ui->curveShowWidget->graph(0)->data().data()->clear();
        ui->curveShowWidget->replot();
        ui->PlotCtrl->setText(tr("开启曲线"));
        break;
    case 1:
        status = 0;
        this->PlotMode = true;
        ui->PlotCtrl->setText(tr("关闭曲线"));
        break;
    default:
        break;
    }

}

void MainWindow::on_AlarmCtr_clicked()
{
    static uint8_t status = 0;
    switch(status)
    {
    case 0:
        status = 1;
        break;
    case 1:
        status = 0;
        break;
    default:
        break;
    }
}

void MainWindow::on_AlarmLightCtr_clicked()
{
    static uint8_t status = 0;
    switch(status)
    {
    case 0:
        status = 1;
        break;
    case 1:
        status = 0;
        break;
    default:
        break;
    }
}

void MainWindow::on_BellCtrl_clicked()
{
    static uint8_t status = 0;
    switch(status)
    {
    case 0:
        status = 1;
        break;
    case 1:
        status = 0;
        break;
    default:
        break;
    }
}
