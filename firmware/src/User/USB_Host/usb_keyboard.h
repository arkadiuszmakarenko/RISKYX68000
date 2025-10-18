#ifndef __USB_KEYBD_H
#define __USB_KEYBD_H

#include "usb_host_config.h"


typedef struct
{
  uint8_t lctrl;
  uint8_t lshift;
  uint8_t lalt;
  uint8_t lgui;
  uint8_t rctrl;
  uint8_t rshift;
  uint8_t ralt;
  uint8_t rgui;
  uint8_t keys[6];
}
HID_Keyboard_Data;


HID_Keyboard_Data *USBH_HID_GetKeyboardInfo(Interface *Itf);

#endif
