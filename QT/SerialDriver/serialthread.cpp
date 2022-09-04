#include "serialthread.h"

SerialThread::SerialThread(QString port, long baud)
{
    myport = port;
    myBaud = baud;
    StateMachine = 0;
    static bool status = false;
    serial = new QSerialPort(this);
    serial->setPortName(myport);
    qDebug() << "port name:" << myport;
    status = serial->open(QIODevice::ReadWrite);
    if(status)
    {
        qDebug() << "Uart open success!";
        serial->setBaudRate(myBaud);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        connect(serial, SIGNAL(readyRead()), this, SLOT(read_serial_data()));
    }
    else
    {
        qDebug() << "Uart open fail!";
    }
}

void SerialThread::read_serial_data()
{
    QByteArray Array = serial->readAll();
    if(!Array.isEmpty())
    {
        for(int i = 0 ; i < Array.length(); i++)
            SensorDataParse(Array[i]);
    }
}

void SerialThread::SensorDataParse(uint8_t Data)
{
    static int GetFrequency = 0;
    static uint16_t Counter = 0;
    static bool RecvStatus = false;

    if(RecvStatus)
    {
        Counter = 0;
        RecvStatus = false;
        StateMachine = STATUS_GET_FRAME_HEADER;
    }

    RecvBuff[Counter] = Data;
    if(0 == Counter && Data != 0xAA)
    {
        RecvStatus = false;
        emit this->DataSend("数据异常");
        return;
    }

    switch(StateMachine)
    {
    case STATUS_GET_FRAME_HEADER:
        StateMachine = STATUS_GET_LENGTH;
        break;

    case STATUS_GET_LENGTH:
        StateMachine = STATUS_GET_SENSOR_MODE;
        break;

    case STATUS_GET_SENSOR_MODE:
        StateMachine = STATUS_GET_SENSOR_DATA;
        break;

    case STATUS_GET_SENSOR_DATA:
        if(2 == GetFrequency)
        {
            GetFrequency = 0;
            StateMachine = STATUS_GET_DATA_CHECKSUM;
            break;
        }
        GetFrequency++;
        break;

    case STATUS_GET_DATA_CHECKSUM:
        if(2 == GetFrequency)
        {
            RecvStatus = true;
            for(int i = 0; i < RecvBuff[1] + 4 ; i++)
                DataInfo.append(RecvBuff[i]);
            if(DataChecksum(RecvBuff))
            {
                DataPackFrame(RecvBuff);
                //转成16进制后以空格分开
                emit this->DataAnalyze(DataParse);
                emit this->DataSend(DataInfo.toHex(' '));
            }
            else
                emit this->DataSend("校验出错");
            DataInfo.clear();
            break;
        }
        GetFrequency++;
        break;
    }
    Counter++;
}

bool SerialThread::DataChecksum(uint8_t Buf[])
{
    int Sum = 0;
    int Checksum = 0;
    for(int i = 2 ; i < 5 ; i++)
        Sum += Buf[i];
    Checksum = Buf[5] + Buf[6];
    if(Sum == Checksum)
        return true;
    return false;
}

void SerialThread::DataPackFrame(uint8_t Buf[])
{
    memset(&DataParse, 0, sizeof(Sensor_Data));
    memcpy(&DataParse, Buf, sizeof(Sensor_Data));
}

SerialThread::~SerialThread()
{
    serial->clear();
    serial->close();
    serial->deleteLater();
}
