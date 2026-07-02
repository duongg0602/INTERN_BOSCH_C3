/*
 * FSM.c
 *
 *  Created on: Jul 2, 2026
 *      Author: ASUS_PC
 */
#include "FSM.h"
#include <string.h>
#include <stdint.h>

UartStatus_t status = 0;
UartFrame_t Frame;
static uint8_t data_index = 0;
uint8_t frame_ready_flag = 0;

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
				status = EOF;
			}
			break;
		case EOF:
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
