/*
 * main.c
 *
 *  Created on: Jun 30, 2026
 *      Author: ASUS_PC
 */
#include "clock.h"
#include "can.h"
#include <stdint.h>
#include "timer.h"
#include "uart.h"
int main(){
	clock_init();
	timer_init();
	CAN_init();
	Uart_init();
	uint8_t data[8] = "Duong\n";
	uint32_t id;
	uint8_t data[8] = {0};
	uint8_t dlc;
	while(1){
		for(int i = 0; i < 8; i++){
			CAN_SendMessage(0x100, data + i, 8);
		}

		CAN_ReceiveMessage(&id, data, &dlc);
		Uart_Send_1byte_Data(data[0]);
		set_timer(1000);
	}
}
