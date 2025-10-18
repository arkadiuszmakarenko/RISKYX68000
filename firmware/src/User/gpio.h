#ifndef __GPIO_H
#define __GPIO_H

#include "ch32v20x_gpio.h"
#include "ch32v20x_rcc.h"

#define LED_Pin GPIO_Pin_14
#define LED_GPIO_Port GPIOC

#define RB_Pin GPIO_Pin_11
#define RB_GPIO_Port GPIOB

#define MB_Pin GPIO_Pin_10
#define MB_GPIO_Port GPIOB

#define LB_Pin GPIO_Pin_15
#define LB_GPIO_Port GPIOB

#define RHQ_Pin GPIO_Pin_12
#define RHQ_GPIO_Port GPIOB
#define LVQ_Pin GPIO_Pin_13
#define LVQ_GPIO_Port GPIOB



#define BH_Pin GPIO_Pin_14
#define BH_GPIO_Port GPIOB

#define FV_Pin GPIO_Pin_8
#define FV_GPIO_Port GPIOA

#define KBD_DATA_GPIO_Port GPIOA
#define KBD_DATA_Pin GPIO_Pin_11

#define KBD_CLOCK_GPIO_Port GPIOA
#define KBD_CLOCK_Pin GPIO_Pin_12

#define KB_RESET_GPIO_Port GPIOA
#define KB_RESET_GPIO_Pin GPIO_Pin_10


void GPIO_Config();


#endif
