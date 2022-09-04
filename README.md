##### 1、设备端

通信串口配置：

- 波特率：115200
- 数据位：8
- 停止位：1
- 校验位：0

##### 1.1、通信协议

| 帧头   | 数据长度 | 传感器类型 | 数据高位 | 数据低位 | 校验和高位 | 校验和低位 |
| ------ | -------- | ---------- | -------- | -------- | ---------- | ---------- |
| 1 Byte | 1 Byte   | 1 Byte     | 1 Byte   | 1 Byte   | 1 Byte     | 1 Byte     |

**帧头**：固定为`0xAA`

**数据长度**：从传感器模式一直到数据低位

**传感器类型**：默认为0，即为`MQ-2`气体传感器，具体类型如下表所示：

| 传感器类型 |           含义            |
| :--------: | :-----------------------: |
|     0      | `E53_SF1 MQ-2`气体传感器  |
|     1      | `E53_SC1 Bh750`光强传感器 |
|     2      |          待添加           |
|     3      |          待添加           |

**数据高位**：(AD_Value & `0xFF00`) >> 8 

**数据低位**：AD_Value & `0x00FF`

**校验和高位**：((传感器模式 + 数据高位 + 数据低位) & `0xFF00`) >> 8

**校验和低位**：(传感器模式 + 数据高位 + 数据低位) & `0x00FF`

设备端默认以`100ms`频率上报。

##### 1.2、结构体定义与数据打包

###### 1.2.1、结构体定义

```c
typedef struct SensorProtocol
{
    uint8_t  FrameHeader;       //帧头
    uint8_t  Length;            //数据长度
    uint8_t  Type;              //传感器类型
    uint8_t  ValueHBit;         //数据高位
    uint8_t  ValueLBit;         //数据低位
    uint8_t  ChecksumHBit;      //校验和高位
    uint8_t  ChecksumLBit;      //校验和低位
}SensorProtocol;
SensorProtocol Protocol;
```

###### 1.2.2、数据打包

```C
void SensorProcolPacket(uint8_t Type, uint16_t val)
{
    Protocol.FrameHeader = 0xAA;
    Protocol.Length = sizeof(Protocol.Type) + \
                      sizeof(Protocol.ValueHBit) + \
                      sizeof(Protocol.ValueLBit);
    Protocol.Type = Type;
    Protocol.ValueHBit = (val & 0xFF00) >> 8;
    Protocol.ValueLBit =  val & 0x00FF;
    Protocol.ChecksumHBit = ((Protocol.Type + \
                              Protocol.ValueHBit + \
                              Protocol.ValueLBit) & 0xFF00) >> 8;
    Protocol.ChecksumHBit = (Protocol.Type + \
                             Protocol.ValueHBit + \
                             Protocol.ValueLBit) & 0x00FF;
}
```

##### 2、上位机协议

待完善。