#include "gpio.h"
#include "ch32v20x_exti.h"

void GPIO_Config()
{

    //Reset RCC
    GPIO_DeInit(GPIOA);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};


    /*Configure GPIO pin Output Level */
    GPIO_WriteBit(LED_GPIO_Port,LED_Pin, Bit_SET);

    // DE-9
    GPIO_WriteBit(UP_GPIO_Port, UP_Pin, Bit_RESET);
    GPIO_WriteBit(DN_GPIO_Port, DN_Pin, Bit_RESET);
    GPIO_WriteBit(LT_GPIO_Port, LT_Pin, Bit_RESET);
    GPIO_WriteBit(RT_GPIO_Port, RT_Pin, Bit_RESET);
    GPIO_WriteBit(B1_GPIO_Port, B1_Pin, Bit_SET);
    GPIO_WriteBit(B2_GPIO_Port, B2_Pin, Bit_SET);

    // GPIO_WriteBit(KBD_DATA_GPIO_Port, KBD_DATA_Pin, Bit_SET);
    // GPIO_WriteBit(KBD_CLOCK_GPIO_Port, KBD_CLOCK_Pin,Bit_SET);
    // GPIO_WriteBit(KB_RESET_GPIO_Port,KB_RESET_GPIO_Pin,Bit_SET);

    GPIO_WriteBit(GPIOA,GPIO_Pin_13,Bit_RESET);
    GPIO_WriteBit(GPIOA,GPIO_Pin_14,Bit_RESET);



    /*Configure GPIO pins : PCPin PCPin PCPin */
    GPIO_InitStructure.GPIO_Pin = LED_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = B1_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(B1_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = B2_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(B2_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UP_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UP_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = DN_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DN_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LT_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LT_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RT_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RT_GPIO_Port, &GPIO_InitStructure);

    // STROBE input (from X68000)
    GPIO_InitStructure.GPIO_Pin = STROBE_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(STROBE_GPIO_Port, &GPIO_InitStructure);

    // READY input (pull-up) - PB9
    GPIO_InitStructure.GPIO_Pin = RDY_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RDY_GPIO_Port, &GPIO_InitStructure);

    // MSCTRL input (pull-up) - PB8
    GPIO_InitStructure.GPIO_Pin = MSCTRL_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MSCTRL_GPIO_Port, &GPIO_InitStructure);

    // Configure EXTI interrupts for READY (PB9) and MSCTRL (PB8)
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // READY (PB9) - EXTI9: falling and rising edge
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
    EXTI_InitStructure.EXTI_Line = EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // MSCTRL (PB8) - EXTI8: falling edge
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Enable EXTI9_5 IRQ (covers EXTI lines 5-9, including both PB8 and PB9)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // SWD pins - why to we configure them?
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);




#if 0 // amiga mouse wheel
       //NVIC EXTI IRQ setup

    //MOUSE PIN 5 - MMB setup for Scroll support
    //On this PCB it is Port B Pin 10

	EXTI_InitTypeDef EXTI_InitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = MB_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(MB_GPIO_Port, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}
