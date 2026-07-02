/*
 * uart.c
 *
 *  Created on: 1 thg 7, 2026
 *      Author: ASUS_PC
 */
#include <string.h>
#include <stdint.h>
#include "uart.h"

void UART_init(){
	uint32_t* RCC_APB2ENR = (uint32_t*)(0x40021000 + 0x18);
	uint32_t* AFIO_MAPR = (uint32_t *)(0x40010000 + 0x04);
	uint32_t* CRH = (uint32_t*)(0x40010800 + 0x04);

	*RCC_APB2ENR |= (1 << 2);
	*RCC_APB2ENR |= (1 << 0);
	*CRH &= ~((0xf << 4) | (0xf << 8)); //Clear bit for PA9, PA10 register
	*CRH |= (0xb << 4) | (0x4 << 8); //set AF for PA9 TX, PA10 RX
	*AFIO_MAPR &= ~(1 << 2); 			//set AFIO for PA9, PA10

	*RCC_APB2ENR |= (1 << 14);
	uint32_t* USART_BRR = (uint32_t *)(0x40013800 + 0x08);
	*USART_BRR = (8 << 4) | (11 << 0);	//set baudrate = 115200
	uint32_t* USART_CR1 = (uint32_t *)(0x40013800 + 0x0c);
	*USART_CR1 &= ~(1 << 10); //disable Parity bits
	*USART_CR1 &= ~(1 << 12); //Set 8bit data
	*USART_CR1 |= (1 << 13) | (1 << 2) | (1 << 3) | (1 << 5); //Enable Usart, transmitter, receiver

//	uint32_t* ISER1 = (uint32_t*)(0xE000E104);
//	*ISER1 |= (1 << 5);
}


void UART_Send_1byte_Data(char data){
	uint32_t* SR = (uint32_t *)(0x40013800);
	uint32_t* DR = (uint32_t *)(0x40013800 + 0x04);
	while((*SR >> 7 & 1) == 0); 	//Wait until data empty
	*DR = data;						//Write data to DR reg to uart transfer data by PA9
	while((*SR >> 6 & 1) == 0); 	// Wait until it finish transmit
	*SR &= ~(1 << 6); 				//Clear TC flag

}

void UART_send_string(char* msg){
	int msg_len = strlen(msg);
	for(int i = 0; i < msg_len; i++){
		Uart_Send_1byte_Data(msg[i]);
	}
}

char UART_receive_1byte(){
	uint32_t* SR = (uint32_t *)(0x40013800);
	while((*SR >> 5 & 1) == 0); //wait to read the receive data
	uint32_t* DR = (uint32_t *)(0x40013800 + 0x04);
	char data = *DR;
	return data;
}

void UART_SendFrame(uint8_t addr, uint8_t id, void* data, uint8_t data_len){
	UART_Send_1byte_Data((char)0xAA);      //SOF
	UART_Send_1byte_Data((char)addr);      //ADDR
	UART_Send_1byte_Data((char)data_len);  //LEN
	UART_Send_1byte_Data((char)id);        //ID

	uint8_t* p = (uint8_t*)data;
	for(uint8_t i = 0; i < data_len; i++){
		UART_Send_1byte_Data((char)p[i]);  //DATA
	}

	UART_Send_1byte_Data((char)0x55);      //EOF
}

