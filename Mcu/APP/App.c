#include "App.h"

#define DEFAULT_STK_SIZE		2048
void DefaultTask(void *pdata);
static void Delay(uint32_t Ms);
osThreadDef(DefaultTask, osPriorityNormal, 1, DEFAULT_STK_SIZE);

void RunApp(void)
{
    osKernelInitialize();
    osThreadCreate(osThread(DefaultTask), NULL);
    osKernelStart();
}

void UartSendByte(uint8_t Data)
{
	USART1->TDR = Data;
	while((USART1->ISR & 0x40) == 0);
}

void DefaultTask(void *pdata)
{
    int32_t Lux;
    int SmokeValue;
    static uint8_t Status = 0;
    uint8_t ProcolBuf[sizeof(SensorProtocol)] = {0};
    Bh750SensorInit(LUX_1_MODE);

    while(1)
    {
        switch(Status)
        {
            case 0:

                ReadBh750Lux(&Lux, 150, Delay);

                if(-1 == Lux)
                {
                    Status = 1;
                }
                else
                {
                    Status = 0;
                    memset(ProcolBuf, 0, sizeof(ProcolBuf));
                    SetSensorType(BEARPI_BH750_SENSOR);
                    SensorProcolPacket(SensorGetType(), Lux);
                    memcpy(ProcolBuf, &Protocol, sizeof(SensorProtocol));
										for(int i = 0; i < sizeof(ProcolBuf) / sizeof(ProcolBuf[0]); i++)
											UartSendByte(ProcolBuf[i]);
                }

                break;

            case 1:
							
                SmokeValue = GetSmokeValue() ;
                memset(ProcolBuf, 0, sizeof(ProcolBuf));
                SetSensorType(BEARPI_MQ_2_SENSOR);
                SensorProcolPacket(SensorGetType(), SmokeValue);
                memcpy(ProcolBuf, &Protocol, sizeof(SensorProtocol));
								for(int i = 0; i < sizeof(ProcolBuf) / sizeof(ProcolBuf[0]); i++)
											UartSendByte(ProcolBuf[i]);
                Status = 0;
						
                break;
        }
    }
}

static void Delay(uint32_t Ms)
{
    osDelay(Ms);
}
