#include "gpio.h"

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
    GPIO_WriteBit(RHQ_GPIO_Port, RHQ_Pin, Bit_RESET);
    GPIO_WriteBit(LVQ_GPIO_Port, LVQ_Pin, Bit_RESET);
    GPIO_WriteBit(BH_GPIO_Port, BH_Pin, Bit_RESET);
    GPIO_WriteBit(FV_GPIO_Port, FV_Pin, Bit_RESET);
    GPIO_WriteBit(LB_GPIO_Port, LB_Pin, Bit_SET);
    GPIO_WriteBit(RB_GPIO_Port, RB_Pin, Bit_SET);
    GPIO_WriteBit(KBD_DATA_GPIO_Port, KBD_DATA_Pin, Bit_SET);
    GPIO_WriteBit(KBD_CLOCK_GPIO_Port, KBD_CLOCK_Pin,Bit_SET);
    GPIO_WriteBit(KB_RESET_GPIO_Port,KB_RESET_GPIO_Pin,Bit_SET);

    GPIO_WriteBit(GPIOA,GPIO_Pin_13,Bit_RESET);
    GPIO_WriteBit(GPIOA,GPIO_Pin_14,Bit_RESET);



    /*Configure GPIO pins : PCPin PCPin PCPin */
    GPIO_InitStructure.GPIO_Pin = LED_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = LB_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LB_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RB_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RB_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RHQ_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RHQ_GPIO_Port, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = LVQ_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LVQ_GPIO_Port, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = BH_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BH_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = FV_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(FV_GPIO_Port, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = KBD_CLOCK_Pin ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Mode =
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;;
    GPIO_Init(KBD_CLOCK_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KBD_DATA_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;;
    GPIO_Init(KBD_DATA_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = KB_RESET_GPIO_Pin ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;;
    GPIO_Init(KB_RESET_GPIO_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);




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
}
