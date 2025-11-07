#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "keyboard.h"
#include "usb_keyboard.h"
#include "gpio.h"
#include "uart.h"

// X68000 state variables
static bool txInhibit = false;
static bool keyInhibit = false;
static bool msctrlAsserted = false;

static uint8_t currentLedLevel = 0;
static uint8_t currentLedState = 0;

static uint16_t keyRepeatDelay = 500;      // ms
static uint16_t keyRepeatInterval = 110;   // ms

// USB HID state
static HID_Keyboard_Data prevReport = {0};
static uint8_t keyRepeatKeyCode = 0x00;
static int32_t keyRepeatCountdown = 0;

// X68000 keyboard command masks (from X68000 Technical Guide, Chapter 5)
#define KEYB_MSCTRL                 0b01000000
#define KEYB_MSCTRL_MASK            0b11111000
#define KEYB_LED_BRIGHTNESS         0b01010100
#define KEYB_LED_BRIGHTNESS_MASK    0b11111100
#define KEYB_KEY_INHIBIT            0b01011000
#define KEYB_KEY_INHIBIT_MASK       0b11111000
#define KEYB_REPEAT_DELAY           0b01100000
#define KEYB_REPEAT_INTERVAL        0b01110000
#define KEYB_REPEAT_MASK            0b11110000
#define KEYB_LED_CTRL_MASK          0b10000000

// USB HID key code definitions
#define HID_KEY_NONE                0x00
#define HID_KEY_A                   0x04
#define HID_KEY_ESCAPE              0x29
#define HID_KEY_EUROPE_1            0x32
#define HID_KEY_EUROPE_2            0x64

// X68000 modifier scan codes
// Maps from USB HID modifier bits to X68000 scan codes
static const uint8_t modifierScans[] =
{
    0x71, // "CTRL"  = KEYBOARD_MODIFIER_LEFTCTRL
    0x70, // "SHIFT" = KEYBOARD_MODIFIER_LEFTSHIFT
    0x56, // "XF2"   = KEYBOARD_MODIFIER_LEFTALT
    0x55, // "XF1"   = KEYBOARD_MODIFIER_LEFTGUI
    0x59, // "XF5"   = KEYBOARD_MODIFIER_RIGHTCTRL
    0x70, // "SHIFT" = KEYBOARD_MODIFIER_RIGHTSHIFT
    0x57, // "XF3"   = KEYBOARD_MODIFIER_RIGHTALT
    0x58, // "XF4"   = KEYBOARD_MODIFIER_RIGHTGUI
};

// X68000 keycode scan codes
// Maps from USB HID key codes (starting at HID_KEY_A = 0x04) to X68000 scan codes
static const uint8_t keycodeScans[] =
{
    0x1e,    // "A"         = HID_KEY_A (0x04)
    0x2e,    // "B"         = HID_KEY_B
    0x2c,    // "C"         = HID_KEY_C
    0x20,    // "D"         = HID_KEY_D
    0x13,    // "E"         = HID_KEY_E
    0x21,    // "F"         = HID_KEY_F
    0x22,    // "G"         = HID_KEY_G
    0x23,    // "H"         = HID_KEY_H
    0x18,    // "I"         = HID_KEY_I
    0x24,    // "J"         = HID_KEY_J
    0x25,    // "K"         = HID_KEY_K
    0x26,    // "L"         = HID_KEY_L
    0x30,    // "M"         = HID_KEY_M
    0x2f,    // "N"         = HID_KEY_N
    0x19,    // "O"         = HID_KEY_O
    0x1a,    // "P"         = HID_KEY_P
    0x11,    // "Q"         = HID_KEY_Q
    0x14,    // "R"         = HID_KEY_R
    0x1f,    // "S"         = HID_KEY_S
    0x15,    // "T"         = HID_KEY_T
    0x17,    // "U"         = HID_KEY_U
    0x2d,    // "V"         = HID_KEY_V
    0x12,    // "W"         = HID_KEY_W
    0x2b,    // "X"         = HID_KEY_X
    0x16,    // "Y"         = HID_KEY_Y
    0x2a,    // "Z"         = HID_KEY_Z
    0x02,    // "1"         = HID_KEY_1
    0x03,    // "2"         = HID_KEY_2
    0x04,    // "3"         = HID_KEY_3
    0x05,    // "4"         = HID_KEY_4
    0x06,    // "5"         = HID_KEY_5
    0x07,    // "6"         = HID_KEY_6
    0x08,    // "7"         = HID_KEY_7
    0x09,    // "8"         = HID_KEY_8
    0x0a,    // "9"         = HID_KEY_9
    0x0b,    // "0"         = HID_KEY_0
    0x1d,    // "RETURN"    = HID_KEY_ENTER
    0x01,    // "ESC"       = HID_KEY_ESCAPE
    0x0f,    // "BS"        = HID_KEY_BACKSPACE
    0x10,    // "TAB"       = HID_KEY_TAB
    0x35,    // "SPACE"     = HID_KEY_SPACE
    0x0c,    // "-"         = HID_KEY_MINUS
    0x0d,    // "^"         = HID_KEY_EQUAL
    0x1b,    // "@"         = HID_KEY_BRACKET_LEFT
    0x1c,    // "["         = HID_KEY_BRACKET_RIGHT
    0x0e,    // "YEN"       = HID_KEY_BACKSLASH
    0x29,    // "]"         = HID_KEY_EUROPE_1
    0x27,    // ";"         = HID_KEY_SEMICOLON
    0x28,    // ":"         = HID_KEY_APOSTROPHE
    0x60,    // "ZENKAKU"   = HID_KEY_GRAVE
    0x31,    // "<"         = HID_KEY_COMMA
    0x32,    // ">"         = HID_KEY_PERIOD
    0x33,    // "?"         = HID_KEY_SLASH
    0x5d,    // "CAPS"      = HID_KEY_CAPS_LOCK
    0x63,    // "F1"        = HID_KEY_F1
    0x64,    // "F2"        = HID_KEY_F2
    0x65,    // "F3"        = HID_KEY_F3
    0x66,    // "F4"        = HID_KEY_F4
    0x67,    // "F5"        = HID_KEY_F5
    0x68,    // "F6"        = HID_KEY_F6
    0x69,    // "F7"        = HID_KEY_F7
    0x6a,    // "F8"        = HID_KEY_F8
    0x6b,    // "F9"        = HID_KEY_F9
    0x6c,    // "F10"       = HID_KEY_F10
    0x72,    // "KANA"      = HID_KEY_F11
    0x73,    // "LATIN"     = HID_KEY_F12
    0x62,    // "COPY"      = HID_KEY_PRINT_SCREEN
    0x54,    // "HELP"      = HID_KEY_SCROLL_LOCK
    0x61,    // "BREAK"     = HID_KEY_PAUSE
    0x5e,    // "INS"       = HID_KEY_INSERT
    0x36,    // "HOME"      = HID_KEY_HOME
    0x38,    // "ROLL UP"   = HID_KEY_PAGE_UP
    0x37,    // "DEL"       = HID_KEY_DELETE
    0x3a,    // "UNDO"      = HID_KEY_END
    0x39,    // "ROLL DOWN" = HID_KEY_PAGE_DOWN
    0x3d,    // "RIGHT"     = HID_KEY_ARROW_RIGHT
    0x3b,    // "LEFT"      = HID_KEY_ARROW_LEFT
    0x3e,    // "DOWN"      = HID_KEY_ARROW_DOWN
    0x3c,    // "UP"        = HID_KEY_ARROW_UP
    0x3f,    // "CLR"       = HID_KEY_NUM_LOCK
    0x40,    // "/"         = HID_KEY_KEYPAD_DIVIDE
    0x41,    // "*"         = HID_KEY_KEYPAD_MULTIPLY
    0x42,    // "-"         = HID_KEY_KEYPAD_SUBTRACT
    0x46,    // "+"         = HID_KEY_KEYPAD_ADD
    0x4e,    // "ENTER"     = HID_KEY_KEYPAD_ENTER
    0x4b,    // "1"         = HID_KEY_KEYPAD_1
    0x4c,    // "2"         = HID_KEY_KEYPAD_2
    0x4d,    // "3"         = HID_KEY_KEYPAD_3
    0x47,    // "4"         = HID_KEY_KEYPAD_4
    0x48,    // "5"         = HID_KEY_KEYPAD_5
    0x49,    // "6"         = HID_KEY_KEYPAD_6
    0x43,    // "7"         = HID_KEY_KEYPAD_7
    0x44,    // "8"         = HID_KEY_KEYPAD_8
    0x45,    // "9"         = HID_KEY_KEYPAD_9
    0x4f,    // "0"         = HID_KEY_KEYPAD_0
    0x51,    // "."         = HID_KEY_KEYPAD_DECIMAL
    0x0e,    // "YEN"       = HID_KEY_EUROPE_2
};

void FlashActivityLED(uint32_t flashRate);

static void sendKeycode(uint8_t keyCode, bool make)
{
    if (keyCode < HID_KEY_A || (keyCode - HID_KEY_A) >= sizeof(keycodeScans))
        return;

    uint8_t scan = keycodeScans[keyCode - HID_KEY_A];
    scan |= (make ? 0x00 : 0x80);  // Set bit 7 for break codes

	printf("sendKeycode(%02x)\n", scan);

    USART1_SendByte(scan);

    FlashActivityLED(100);
}

static bool isPresentInReport(HID_Keyboard_Data const *report, uint8_t keyCode)
{
    for (int i = 0; i < KEY_PRESSED_MAX; ++i)
    {
        if (report->keys[i] == keyCode)
            return true;
    }
    return false;
}

static void compareKeybReports(HID_Keyboard_Data const* reportA, HID_Keyboard_Data const* reportB, bool make)
{
    for (int i = 0; i < KEY_PRESSED_MAX; ++i)
    {
        uint8_t keyCode = reportA->keys[i];

        // Skip ErrorRollOver, POSTFail & ErrorUndefined
        if (keyCode == HID_KEY_NONE || keyCode <= 0x03)
            continue;

        if (!isPresentInReport(reportB, keyCode))
        {
            sendKeycode(keyCode, make);

            // Record the most recent KEYDOWN, or reset it
            if (make)
            {
                keyRepeatKeyCode = keyCode;
                keyRepeatCountdown = keyRepeatDelay;
            }
            else
            {
                if (keyRepeatKeyCode == keyCode)
                {
                    keyRepeatKeyCode = 0;
                    keyRepeatCountdown = 0;
                }
            }
        }
    }
}

void KeyboardInit(void)
{
    memset(&prevReport, 0, sizeof(prevReport));
    txInhibit = false;
    keyInhibit = false;
    msctrlAsserted = false;
    currentLedLevel = 0;
    currentLedState = 0;
    keyRepeatDelay = 500;
    keyRepeatInterval = 110;
    keyRepeatKeyCode = 0x00;
    keyRepeatCountdown = 0;
}

void KeyboardProcess(HID_Keyboard_Data *kbdata)
{
    if (kbdata == NULL) return;
    if (keyInhibit) return;

    // Evaluate modifiers (SHIFT/CTRL/ALT/GUI)
    uint8_t currentMod = (kbdata->lctrl << 0) | (kbdata->lshift << 1) |
                         (kbdata->lalt << 2) | (kbdata->lgui << 3) |
                         (kbdata->rctrl << 4) | (kbdata->rshift << 5) |
                         (kbdata->ralt << 6) | (kbdata->rgui << 7);

    uint8_t prevMod = (prevReport.lctrl << 0) | (prevReport.lshift << 1) |
                      (prevReport.lalt << 2) | (prevReport.lgui << 3) |
                      (prevReport.rctrl << 4) | (prevReport.rshift << 5) |
                      (prevReport.ralt << 6) | (prevReport.rgui << 7);

    uint8_t modified = prevMod ^ currentMod;
    if (modified)
    {
        for (int i = 0; i < 8; ++i)
        {
            uint8_t mod = 1 << i;
            if (modified & mod)
            {
                bool make = currentMod & mod;
                uint8_t scan = modifierScans[i] | (make ? 0x00 : 0x80);
                USART1_SendByte(scan);

                FlashActivityLED(100);
            }
        }
    }

    // Evaluate BREAK codes (key-up)
    compareKeybReports(&prevReport, kbdata, false);

    // Evaluate MAKE codes (key-down)
    compareKeybReports(kbdata, &prevReport, true);

    prevReport = *kbdata;
}

static void processCommands(void)
{
    while (USART1_DataAvailable())
    {
        uint8_t ch = USART1_ReadByte();

        if ((ch & KEYB_MSCTRL_MASK) == KEYB_MSCTRL)
        {
            msctrlAsserted = (ch & 0x01) == 0;
        }
        else if ((ch & KEYB_LED_BRIGHTNESS_MASK) == KEYB_LED_BRIGHTNESS)
        {
            uint8_t led_value = ch & 0x03;
            currentLedLevel = led_value;
        }
        else if ((ch & KEYB_KEY_INHIBIT_MASK) == KEYB_KEY_INHIBIT)
        {
            keyInhibit = (ch & 0x01) == 0;
        }
        else if ((ch & KEYB_REPEAT_MASK) == KEYB_REPEAT_DELAY)
        {
            uint16_t delayMS = 200 + (ch & 0x0f) * 100;
            keyRepeatDelay = delayMS;
        }
        else if ((ch & KEYB_REPEAT_MASK) == KEYB_REPEAT_INTERVAL)
        {
            uint16_t v = (ch & 0x0f);
            uint16_t intervalMS = 30 + v * v * 5;
            keyRepeatInterval = intervalMS;
        }
        else if ((ch & KEYB_LED_CTRL_MASK) == KEYB_LED_CTRL_MASK)
        {
            uint8_t led = ch & 0x7f;
            currentLedState = led;
        }
    }
}

bool KeyboardProcessCommands(uint32_t deltaTime)
{
    // Process X68000 keyboard commands
	bool wasAserted = msctrlAsserted;
	processCommands();
	bool isAsserted = msctrlAsserted;
	bool updateMouse = !wasAserted && isAsserted;

    // Handle key repeats
    if (keyRepeatCountdown > 0)
    {
        keyRepeatCountdown -= (int32_t)deltaTime;
        if (keyRepeatCountdown <= 0)
        {
            // Send repeat keycode
            sendKeycode(keyRepeatKeyCode, true);
            keyRepeatCountdown = keyRepeatInterval;
        }
    }

	return updateMouse;
}
