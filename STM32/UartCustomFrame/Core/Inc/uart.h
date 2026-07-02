/*
 * uart.h
 *
 *  Created on: Jul 2, 2026
 *      Author: ASUS_PC
 */

#ifndef INC_UART_H_
#define INC_UART_H_
#include <stdint.h>

void UART_init();
void UART_Send_1byte_Data(char data);
void UART_send_string(char* msg);
char UART_receive_1byte();
void UART_SendFrame(uint8_t addr, uint8_t id, void* data, uint8_t data_len);

#endif /* INC_UART_H_ */
