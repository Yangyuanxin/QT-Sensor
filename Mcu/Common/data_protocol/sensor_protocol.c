#include "sensor_protocol.h"


uint8_t GasType;
SensorProtocol Protocol;


void SetSensorType(uint8_t Type)
{
	GasType = Type;
}

uint8_t SensorGetType(void)
{
	return GasType;
}


void SensorProcolPacket(uint8_t Type, uint16_t val)
{
	Protocol.FrameHeader = 0xAA;
	
	Protocol.Length = sizeof(Protocol.Type) + \
										sizeof(Protocol.ValueHBit) + \
										sizeof(Protocol.ValueLBit);
											 
	Protocol.Type = Type;
											 
	Protocol.ValueHBit = (val & 0xFF00) >> 8;
											 
  Protocol.ValueLBit = val & 0x00FF;
											 
	Protocol.ChecksumHBit = ((Protocol.Type + \
											      Protocol.ValueHBit + \
													  Protocol.ValueLBit) & 0xFF00)>> 8;
											 
	Protocol.ChecksumHBit = (Protocol.Type + \
											     Protocol.ValueHBit + \
													 Protocol.ValueLBit) & 0x00FF;
}


