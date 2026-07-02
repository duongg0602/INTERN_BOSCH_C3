#include <Arduino.h>

typedef struct{
	float temp;
	float humi;
	uint16_t pressure;
	uint8_t flag;
}sensor_t;

typedef struct{
	uint8_t sof;
	uint8_t add;
	uint8_t len;
	uint8_t ID;
	sensor_t data;
	uint8_t eof;
}UartFrame_t;

typedef enum{
	SOF = 0,
	ADDR,
	LEN,
	ID,
	DATA,
	EOF_
}UartStatus_t;

sensor_t SensorReading;
uint8_t frameAddr = 0x01;
uint8_t  frameId   = 0x10;

UartStatus_t status = SOF;
UartFrame_t Frame;

uint8_t data_index = 0;
uint8_t frame_ready_flag = 0;
void Serial_SendFrame(uint8_t addr, uint8_t id, void* data, uint8_t data_len);

void ProcessUart(uint8_t rxByteData){
  switch(status){
		case SOF:
			if(rxByteData == 0xAA){
				status = ADDR;
			}
			break;
		case ADDR:
			Frame.add = rxByteData;
			status = LEN;
			break;
		case LEN:
			if(rxByteData != sizeof(sensor_t)){
				status = SOF;
				break;
			}
			Frame.len = rxByteData;
			status = ID;
			break;
		case ID:
			Frame.ID = rxByteData;
			data_index = 0;
			memset(&Frame.data, 0, sizeof(sensor_t));
			status = DATA;
			break;
		case DATA:
			((uint8_t*)&Frame.data)[data_index] = rxByteData;
			data_index++;
			if(data_index >= sizeof(sensor_t)){
				status = EOF_;
			}
			break;
		case EOF_:
			if(rxByteData == 0x55){
				Frame.eof = rxByteData;
				frame_ready_flag = 1;
			}
			status = SOF;
			break;
		default:
			status = SOF;
			break;

	}

}
void setup() {
	Serial.begin(115200);
}

void loop() {
	SensorReading.temp = 30.5f;
	SensorReading.humi = 30.0f;
	SensorReading.pressure = 100;
	SensorReading.flag = 1;	
	Serial_SendFrame(frameAddr, frameId, &SensorReading, sizeof(sensor_t));
	delay(1000);

}

void Serial_SendFrame(uint8_t addr, uint8_t id, void* data, uint8_t data_len){
	Serial.write((uint8_t)0xAA);
	Serial.write((uint8_t)addr);
	Serial.write((uint8_t)data_len);
	Serial.write((uint8_t)id);
	Serial.write((uint8_t*)data, data_len);
	Serial.write((uint8_t)0x55);
}