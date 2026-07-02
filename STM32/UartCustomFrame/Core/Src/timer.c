/*
 * timer.c
 *
 *  Created on: 1 thg 7, 2026
 *      Author: ASUS_PC
 */
#include "timer.h"

void timer_init(){
	uint32_t* RCC_APB2ENR = (uint32_t*)(0x40021000 + 0x18);
	*RCC_APB2ENR |= (1 << 11);

	uint16_t* CR1 = (uint16_t *)(0x40012C00);
	uint16_t* ARR = (uint16_t *)(0x40012C00 + 0x2C);
	uint16_t* PSC = (uint16_t *)(0x40012C00 + 0x28);
	uint32_t* ISER0 = (uint32_t *)(0xE000E100);
	uint16_t* DIER = (uint16_t *)(0x40012C00 + 0x0c);
	*ARR = 999;
	*PSC = 16 - 1;

	*CR1 |= (1 << 0); //enable timer
	*DIER |= (1 << 0); //enable interrupt

	*ISER0 |= (1 << 25);


}
volatile uint32_t cnt = 0;
void TIM1_UP_IRQHandler(){
	uint16_t* SR = (uint16_t *)(0x40012C00 + 0x10);
	*SR &= ~(1 << 0); //clear flag
	cnt++;
}

void set_timer(uint32_t msec){
	uint32_t current_cnt = cnt;
	while((cnt - current_cnt) < msec);
}
