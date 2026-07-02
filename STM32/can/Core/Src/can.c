/*
 * can.c
 *
 *  Created on: Jun 30, 2026
 *      Author: ASUS_PC
 */
#include "can.h"

void CAN_init(){
	uint32_t* RCC_APB1ENR = (uint32_t*)(0x40021000 + 0x1C);
	uint32_t* RCC_APB2ENR = (uint32_t*)(0x40021000 + 0x18);
	uint32_t* AFIO_MAPR = (uint32_t*)(0x40010000 + 0x04);
	uint32_t* GPIOA_CRH = (uint32_t*)(0x40010800  + 0x04);
	uint32_t* GPIOA_BSRR = (uint32_t*)(0x40010800  + 0x10);
	uint32_t* CAN_MCR = (uint32_t*)(0x40006400);
	uint32_t* CAN_MSR = (uint32_t*)(0x40006400 + 0x04);
	uint32_t* CAN_BTR = (uint32_t*)(0x40006400 + 0x1C);
	uint32_t* CAN_FMR = (uint32_t*)(0x40006400 + 0x200);
	uint32_t* CAN_FA1R = (uint32_t*)(0x40006400 + 0x21C);
	uint32_t* CAN_FS1R = (uint32_t*)(0x40006400 + 0x20C);
	uint32_t* CAN_F0R1 = (uint32_t*)(0x40006400 + 0x240);
	uint32_t* CAN_F0R2 = (uint32_t*)(0x40006400 + 0x244);

	*RCC_APB1ENR |= (1 << 25);
	*RCC_APB2ENR |= (1 << 0);
	*RCC_APB2ENR |= (1 << 2);

	*AFIO_MAPR &= ~(0b11 << 13); //CAN RX --> PA11, CAN TX --> PA12
	*GPIOA_CRH &= ~(0xf << 12);
	*GPIOA_CRH &= ~(0xf << 16);
	*GPIOA_CRH |= (0b0100 << 12);
	*GPIOA_BSRR |= (1 << 11);
	*GPIOA_CRH |= (0b1010 << 16);

	*CAN_MCR |= (1 << 0); // Initialization request
	while(((*CAN_MSR >> 0) & 1) == 0); //wait until hardware has confirmed the request
	*CAN_MCR |= (1 << 4) | (1 << 5);


	*CAN_BTR = 0;
	*CAN_BTR |= 1; //BRP = 1
	*CAN_BTR |= 11 << 16; //TS1 = 12tq
	*CAN_BTR |= 2 << 20; //TS2 = 3tq
	//==> Baud = 500kbps
//	*CAN_BTR |= (1 << 30); //loop back mode

	*CAN_MCR &= ~(1 << 0);

	*CAN_FMR |= (1 << 0);
	*CAN_FA1R &= ~(1 << 0);
	*CAN_FS1R |= (1 << 0);

	*CAN_F0R1 &= ~(0x7ff << 21);
	*CAN_F0R2 &= ~(0x7ff << 21);
	*CAN_F0R1 |= (0x100 << 21); //sample id
	*CAN_F0R2 |= (0x000 << 21); //mask

	*CAN_FA1R |= (1 << 0);

	*CAN_FMR &= ~(1 << 0);
	*CAN_MSR &= ~(1 << 0);
}

void CAN_SendMessage(uint32_t ID, uint8_t* data, uint8_t len){
	uint32_t* CAN_TSR = (uint32_t*)(0x40006400 + 0x08);
	uint32_t* CAN_TI0R = (uint32_t*)(0x40006400 + 0x180);
	uint32_t* CAN_TDT0R = (uint32_t*)(0x40006400 + 0x184);
	uint32_t* CAN_TDL0R = (uint32_t*)(0x40006400 + 0x188);

	while(((*CAN_TSR >> 26) & 1) == 0);
	*CAN_TI0R = (ID << 21);
	*CAN_TDT0R = (len << 0);
	*CAN_TDL0R = *data;
	*CAN_TI0R |= (1 << 0);

}

void CAN_ReceiveMessage(uint32_t* ID, uint8_t* data, uint8_t* len){
	uint32_t* CAN_RF0R = (uint32_t*)(0x40006400 + 0x0c);
	uint32_t* CAN_RI0R = (uint32_t*)(0x40006400 + 0x1b0);
	uint32_t* CAN_RTD0R = (uint32_t*)(0x40006400 + 0x1b4);
	uint32_t* CAN_RDL0R = (uint32_t*)(0x40006400 + 0x1b8);

	while(((*CAN_RF0R >> 0) & 0b11) == 0);
	*ID = (*CAN_RI0R >> 21) & 0x7ff;
	*len = (*CAN_RTD0R >> 0) & 0xf;
	*data = (*CAN_RDL0R >> 0) & 0xff;

	*CAN_RF0R |= (1 << 5);
}
