#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include "gpio.h"
#include "usb_keyboard.h"

#define KEY_PRESSED_MAX 6

typedef enum {
	NO_LED = 0,
	LED_CAPS_LOCK_ON,
	LED_NUM_LOCK_ON,
	LED_SCROLL_LOCK_ON,
	LED_CAPS_LOCK_OFF,
	LED_NUM_LOCK_OFF,
	LED_SCROLL_LOCK_OFF,
} led_status_t;

typedef enum {
	NUM_LOCK_LED = (1 << 0),
	CAPS_LOCK_LED = (1 << 1),
	SCROLL_LOCK_LED = (1 << 2),
} keyboard_led_t;

void KeyboardInit(void);
void KeyboardProcess(HID_Keyboard_Data *kbdata);
bool KeyboardProcessCommands(uint32_t deltaTime);

#endif
