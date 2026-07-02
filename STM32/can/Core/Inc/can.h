/*
 * can.h
 *
 *  Created on: Jun 30, 2026
 *      Author: ASUS_PC
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_
#include <stdint.h>

void CAN_init();
void CAN_SendMessage(uint32_t ID, uint8_t* data, uint8_t len);
void CAN_ReceiveMessage(uint32_t* ID, uint8_t* data, uint8_t* len);

#endif /* INC_CAN_H_ */
