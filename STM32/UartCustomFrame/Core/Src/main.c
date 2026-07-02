/*
 * main.c
 *
 *  Created on: Jul 2, 2026
 *      Author: ASUS_PC
 */
#include <stdint.h>
#include "clock.h"
#include "uart.h"
#include "timer.h"
#include "FSM.h"


int main(){
	uint8_t rxByteData = 0;
	clock_init();
	UART_init();
	timer_init();
	sensor_t SensorReading;
	uint8_t frameAddr = 0x01;
	uint8_t  frameId   = 0x10;

	while(1){
		SensorReading.temp = 30.5f;
		SensorReading.humi = 30.0f;
		SensorReading.pressure = 100;
		SensorReading.flag = 1;
		UART_SendFrame(frameAddr, frameId, &SensorReading, sizeof(sensor_t));
		set_timer(1000);
	}
}

