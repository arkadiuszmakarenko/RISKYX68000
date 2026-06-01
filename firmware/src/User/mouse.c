#include "mouse.h"
#include "gpio.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// X68000 state variables
static bool txInhibit = false;
static bool msctrlAsserted = false;
static bool prevMsctrlState = false;

// USB HID mouse state
static int16_t dx = 0;
static int16_t dy = 0;
static bool lmbPressed = false;
static bool rmbPressed = false;

void MouseInit(void)
{
    txInhibit = false;
    msctrlAsserted = false;
    prevMsctrlState = false;
    dx = 0;
    dy = 0;
    lmbPressed = false;
    rmbPressed = false;
}

void MouseProcess(HID_MOUSE_Data *mousemap)
{
    if (mousemap == NULL) return;

    // Accumulate mouse movement
    dx += mousemap->x;
    dy += mousemap->y;

    // Update button states
    lmbPressed = mousemap->buttons[0];
    rmbPressed = mousemap->buttons[1];
}

void FlashActivityLED(uint32_t flashRate);

void MouseSend(void)
{
    if (txInhibit)
        return;

    X68K_MouseData mdata = {0};

    // Set button states
    mdata.Lbtn = lmbPressed;
    mdata.Rbtn = rmbPressed;

    // Set overflow/underflow flags
    mdata.Xover = (dx > 127);
    mdata.Xundr = (dx < -128);
    mdata.Yover = (dy > 127);
    mdata.Yundr = (dy < -128);

    // Set delta values (clamp to 8-bit signed range)
    mdata.dx = (int8_t)(dx & 0xff);
    mdata.dy = (int8_t)(dy & 0xff);

    // Send the 3-byte packet via USART2
    USART2_SendData(mdata.data, sizeof(mdata));

    // Flash LED to indicate activity (only if packet is not empty)
    if (mdata.data[0] != 0 || mdata.data[1] != 0 || mdata.data[2] != 0)
    {
        printf("mdata = { %02x, %02x, %02x }\n", mdata.data[0], mdata.data[1], mdata.data[2]);
        FlashActivityLED(100);
    }

    // Reset mouse delta
    dx = 0;
    dy = 0;
}

// ISR callback for READY signal (PB9)
// Called on both rising and falling edges
void MouseReadyISR(void)
{
    // Read READY GPIO to update txInhibit
    // READY falling edge = X68000 not ready (txInhibit = true)
    // READY rising edge = X68000 ready (txInhibit = false)
    bool readyState = GPIO_ReadInputDataBit(RDY_GPIO_Port, RDY_Pin);
    txInhibit = !readyState;
}

// ISR callback for MSCTRL signal (PB8)
// Called on falling edge - triggers mouse data transmission
void MouseMsctrlISR(void)
{
    MouseSend();
}
