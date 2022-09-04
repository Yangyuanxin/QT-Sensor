#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdint.h>

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))

#define MAX_LEN 128
/*帧头  长度  模式  气体值H  气体值L  校验H  校验L*/
typedef struct Sensor_Data
{
    uint8_t frame_header;       /*帧头*/
    uint8_t length;             /*长度*/
    uint8_t mode;               /*模式*/
    uint8_t GasHighBit;         /*气体值高位*/
    uint8_t GasLowBit;          /*气体值低位*/
    uint8_t CheckSumHighBit;    /*校验值高位*/
    uint8_t CheckSumLowBit;     /*校验值低位*/
}Sensor_Data;

enum StatusMachine
{
    STATUS_GET_FRAME_HEADER = 0,
    STATUS_GET_LENGTH,
    STATUS_GET_SENSOR_MODE,
    STATUS_GET_SENSOR_DATA,
    STATUS_GET_DATA_CHECKSUM
};

#endif // PUBLIC_H
