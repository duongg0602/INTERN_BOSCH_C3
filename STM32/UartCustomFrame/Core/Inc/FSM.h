/*
 * FSM.h
 *
 *  Created on: Jul 2, 2026
 *      Author: ASUS_PC
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_
#include <stdint.h>
extern uint8_t frame_ready_flag;

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
	EOF
}UartStatus_t;

extern UartStatus_t status;
extern UartFrame_t Frame;

void ProcessUart(uint8_t rxByteData);

#endif /* INC_FSM_H_ */
