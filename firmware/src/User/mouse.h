#ifndef __MOUSE_H
#define __MOUSE_H

#include "stdint.h"
#include "stdbool.h"
#include "usb_mouse.h"

// X68000 mouse 3-byte packet structure
typedef struct
{
    union
    {
        uint8_t data[3];
        struct
        {
            union
            {
                struct
                {
                    uint8_t Lbtn:1;     // Bit 0: Left button
                    uint8_t Rbtn:1;     // Bit 1: Right button
                    uint8_t unused:2;   // Bits 2-3
                    uint8_t Xover:1;    // Bit 4: X overflow
                    uint8_t Xundr:1;    // Bit 5: X underflow
                    uint8_t Yover:1;    // Bit 6: Y overflow
                    uint8_t Yundr:1;    // Bit 7: Y underflow
                };
                uint8_t state;
            };
            int8_t  dx;
            int8_t  dy;
        };
    };
} X68K_MouseData;

void MouseInit(void);
void MouseProcess(HID_MOUSE_Data *mousemap);
void MouseSend(void);

void MouseReadyISR(void);
void MouseMsctrlISR(void);

#endif
