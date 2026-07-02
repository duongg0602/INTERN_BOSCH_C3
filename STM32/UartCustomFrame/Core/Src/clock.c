/*
 * clock.c
 *
 *  Created on: Jun 30, 2026
 *      Author: ASUS_PC
 */
#include "clock.h"
#include <stdint.h>

void clock_init(){
	uint32_t* RCC_CR = (uint32_t*)(0x40021000);
	uint32_t* RCC_CFGR = (uint32_t*)(0x40021000 + 0x04);
	uint32_t* RCC_AHBENR = (uint32_t*)(0x40021000 + 0x14);

	*RCC_CR |= (1 << 16); //enable HSE Clock
	while(((*RCC_CR >> 17) & 1) != 1); //wait HSE ready

	*RCC_CFGR |= (1 << 16); // HSE oscillator clock selected as PLL input clock
	*RCC_CFGR &= ~(1 << 17);
	*RCC_CFGR &= ~(0b1111 << 18);
	*RCC_CFGR &= ~(0b1111 << 4); //AHB prs not divided ==> 8Mhz
	*RCC_CFGR &= ~(0b111 << 8);  //ABP1 not divided ==> 8MHz
	*RCC_CFGR &= ~(0b111 << 11); //ABP2 not divided ==> 8MHz

	*RCC_CR |= (1 << 24);	//enable PLL
	while(((*RCC_CR >> 25) & 1) != 1); //wait PLL ready

	*RCC_CFGR &= ~(0b11 << 0);
	*RCC_CFGR |= (0b10 << 0); //PLL selected as system clock

	while(((*RCC_CFGR >> 2) & 0b11) != 0b10); //wait until PLL is set as sys clock
}
