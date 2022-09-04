#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QDebug>
#include <QObject>
#include <QThread>
#include "public.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

using namespace std;

class SerialThread : public QObject
{
    Q_OBJECT
public:
    ~SerialThread();
    void SensorDataParse(uint8_t);
    bool DataChecksum(uint8_t Buf[]);
    void DataPackFrame(uint8_t Buf[]);
    explicit SerialThread(QString, long);

signals:
    void DataSend(QByteArray);
    void DataAnalyze(Sensor_Data);

public slots:
    void read_serial_data();

private:
    long myBaud;
    QString myport;
    QSerialPort *serial;

    uint8_t data;
    int StateMachine;
    QByteArray DataInfo;
    Sensor_Data DataParse;
    uint8_t RecvBuff[MAX_LEN];
};

#endif // SERIALTHREAD_H
