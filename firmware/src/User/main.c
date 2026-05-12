#include <usb_gamepad.h>
#include <usb_mouse.h>
#include <usb_keyboard.h>
#include "usb_host_config.h"
#include "utils.h"
#include "tim.h"
#include "mouse.h"
#include "gpio.h"
#include "keyboard.h"
#include "gamepad.h"
#include "uart.h"


/*

7pin miniDIN

red = vcc
black = gnd
white = key rxd
green = key txd
blue = mouse data
orange = ready

USART1 (keyboard)
PA9 = TX
PA10 = RX

UART2 (mouse)
PA2 = TX
PA3 = RX (not used)

USART3 (printf)

*/

/*

RX1 <- pin 4 (TxD)
TX1 -> pin 3 (RxD)

TX2 -> pin 2 (MDATA)
J1.1 <- pin 5 (READY)

*/


void IWDG_Feed_Init(u16 prer, u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(prer);
    IWDG_SetReload(rlr);
    IWDG_ReloadCounter();
    IWDG_Enable();
}

void FlashActivityLED(uint32_t flashRate)
{
    static uint32_t lastUpdated = 0;
    static bool activityLED = false;

    uint32_t currentTimer = GetMillis();

    if ((currentTimer - lastUpdated) < flashRate)
        return;

	GPIO_WriteBit(LED_GPIO_Port, LED_Pin, activityLED ? Bit_SET : Bit_RESET);
    activityLED = !activityLED;
    lastUpdated += flashRate;
}

int main( void )
{
	if (DEF_DEBUG_PRINTF)
		USART_Printf_Init( 115200 );
    DUG_PRINTF( "SystemClk:%d\r\n", SystemCoreClock );
    Delay_Init( );
    TIM3_Init( 9, SystemCoreClock / 10000 - 1 );

#if DEF_USBFS_PORT_EN
    USBFS_RCC_Init( );
    USBFS_Host_Init( ENABLE );
    memset( &RootHubDev.bStatus, 0, sizeof( ROOT_HUB_DEVICE ) );
    memset( &HostCtl[ DEF_USBFS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL ].InterfaceNum, 0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof( HOST_CTL ) );
#endif
    TIM1_Init();
    TIM2_Init();
    TIM4_Init();
    GPIO_Config();

	// need to reinit USART after gpio init
	if (DEF_DEBUG_PRINTF)
		USART_Printf_Init( 115200 );

	// Initialize keyboard and mouse UARTs
	USART1_Init();  // Keyboard - 2400 baud, 8N1
	USART2_Init();  // Mouse - 4800 baud, 8N2

	// Initialize X68000 keyboard and mouse
	KeyboardInit();
	MouseInit();

    IWDG_Feed_Init( IWDG_Prescaler_32, 4000 );

	uint32_t lastTimer = GetMillis();

	while (1) {
	    IWDG_ReloadCounter();
		USBH_MainDeal();

		uint32_t currentTimer = GetMillis();
		uint32_t deltaTime = currentTimer - lastTimer;
		lastTimer = currentTimer;

		FlashActivityLED(500);

		// Process keyboard commands and key repeats
		if (KeyboardProcessCommands(deltaTime))
			MouseSend();

		//Handle HID Device
		if (RootHubDev.bType == USB_DEV_CLASS_HID) {

			for (int itf = 0; itf < DEF_INTERFACE_NUM_MAX; itf++) {
				//Handle mouse
				if (HostCtl[0].Interface[itf].HIDRptDesc.type
						== REPORT_TYPE_MOUSE) {
				    HID_MOUSE_Data *mousemap = USB_GetMouseInfo(
							&HostCtl[0].Interface[itf]);
					MouseProcess(mousemap);
				}

				//Handle gamepad
				if (HostCtl[0].Interface[itf].HIDRptDesc.type
						== REPORT_TYPE_JOYSTICK) {

		HID_gamepad_Info_TypeDef *gamepad = GetGamepadInfo(
							&HostCtl[0].Interface[itf]);
						ProcessGamepad(gamepad);
				}

				// Handle Keyboard
				if (HostCtl[0].Interface[itf].HIDRptDesc.type
						== REPORT_TYPE_KEYBOARD) {
					//HID_KEYBD_Info_TypeDef *USBH_HID_GetKeybdInfo(Interface *Itf)
				    HID_Keyboard_Data *kbd = USBH_HID_GetKeyboardInfo(
							&HostCtl[0].Interface[itf]);

					KeyboardProcess(kbd);

				}

			}
		}

		//Handle HUB Device

		if (RootHubDev.bType == USB_DEV_CLASS_HUB) {

			//Iterate over all devices
			for (uint8_t device = 1; device < 5; device++)
			{
				//Iterate over all interfaces
				for (int itf = 0; itf < DEF_INTERFACE_NUM_MAX; itf++) {
					//Handle mouse
					if (HostCtl[device].Interface[itf].HIDRptDesc.type
							== REPORT_TYPE_MOUSE) {
					    HID_MOUSE_Data *mousemap = USB_GetMouseInfo(
								&HostCtl[device].Interface[itf]);
							MouseProcess(mousemap);

					}

					//Handle gamepad
					if (HostCtl[device].Interface[itf].HIDRptDesc.type
							== REPORT_TYPE_JOYSTICK) {

			HID_gamepad_Info_TypeDef *gamepad = GetGamepadInfo(
								&HostCtl[device].Interface[itf]);
							ProcessGamepad(gamepad);
					}

					// Handle Keyboard
					if (HostCtl[device].Interface[itf].HIDRptDesc.type
							== REPORT_TYPE_KEYBOARD) {
					    HID_Keyboard_Data *kbd = USBH_HID_GetKeyboardInfo(
								&HostCtl[device].Interface[itf]);
							KeyboardProcess(kbd);

					}

				}
			}



			}

		}

	}

