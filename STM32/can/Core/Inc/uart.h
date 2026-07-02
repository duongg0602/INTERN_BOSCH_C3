/*
 * uart.h
 *
 *  Created on: 1 thg 7, 2026
 *      Author: ASUS_PC
 */
#include <stdint.h>

void Uart_init();
void Uart_Send_1byte_Data(char data);
void Uart_send_string(char* msg);
char Uart_receive_1byte();
