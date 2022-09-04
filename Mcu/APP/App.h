#ifndef _APP_H_
#define _APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "spi.h"
#include "adc.h"
#include "gpio.h"
#include "usart.h"

#include "Lcd.h"
#include "MQ_2.h"
#include "Bh750.h"
#include "cmsis_os.h"
#include "sensor_protocol.h"

enum SensorType_t
{
    BEARPI_BH750_SENSOR,
    BEARPI_MQ_2_SENSOR
};

void RunApp(void);

#ifdef __cplusplus
}
#endif
#endif  //ifndef _APP_H_
