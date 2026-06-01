#ifndef __UART_H
#define __UART_H

#include "ch32v20x_usart.h"
#include "ch32v20x_rcc.h"

// USART1 (keyboard) - 2400 baud, 8N1
// PA9 = TX
// PA10 = RX
void USART1_Init(void);

// USART2 (mouse) - 4800 baud, 8N2
// PA2 = TX
// PA3 = RX (not used)
void USART2_Init(void);

// Send a byte via USART1 (keyboard)
void USART1_SendByte(uint8_t byte);

// Send a byte via USART2 (mouse)
void USART2_SendByte(uint8_t byte);

// Send data via USART2 (mouse)
void USART2_SendData(uint8_t *data, uint16_t len);

// Check if data is available on USART1
uint8_t USART1_DataAvailable(void);

// Read a byte from USART1 (keyboard)
uint8_t USART1_ReadByte(void);

#endif
