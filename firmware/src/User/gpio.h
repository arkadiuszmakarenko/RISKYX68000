#ifndef __GPIO_H
#define __GPIO_H

#include "ch32v20x_gpio.h"
#include "ch32v20x_rcc.h"

// J7 pin header
#define RDY_Pin GPIO_Pin_9      // miniDIN-7 READY pin (pin 5)
#define RDY_GPIO_Port GPIOB
#define MSCTRL_Pin GPIO_Pin_8   // Mouse control signal (Mouse Mini-DIN 5-pin, pin 2)
#define MSCTRL_GPIO_Port GPIOB
#define P3_Pin GPIO_Pin_3
#define P3_GPIO_Port GPIOB
#define P4_Pin GPIO_Pin_15
#define P4_GPIO_Port GPIOA

#define LED_Pin GPIO_Pin_14     // the RDY LED by the USB cnx
#define LED_GPIO_Port GPIOC

#define LED1_Pin GPIO_Pin_1
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_Pin_0
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_Pin_1
#define LED3_GPIO_Port GPIOD
#define LED4_Pin GPIO_Pin_0
#define LED4_GPIO_Port GPIOD
#define LED5_Pin GPIO_Pin_15
#define LED5_GPIO_Port GPIOC

// DE-9 Gamepad
#define UP_Pin GPIO_Pin_12
#define UP_GPIO_Port GPIOB
#define DN_Pin GPIO_Pin_13
#define DN_GPIO_Port GPIOB
#define LT_Pin GPIO_Pin_14
#define LT_GPIO_Port GPIOB
#define RT_Pin GPIO_Pin_8
#define RT_GPIO_Port GPIOA

#define B1_Pin GPIO_Pin_15
#define B1_GPIO_Port GPIOB
#define B2_Pin GPIO_Pin_5
#define B2_GPIO_Port GPIOB

// STROBE (out from x68k)
#define STROBE_Pin GPIO_Pin_4
#define STROBE_GPIO_Port GPIOB

void GPIO_Config();

#endif
