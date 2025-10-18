#include "usb_keyboard.h"

static USB_Status USBH_HID_KeyboardDecode(Interface *Itf);


HID_Keyboard_Data  keybd_info;
uint8_t                   keybd_report_data[16];

HID_Keyboard_Data *USBH_HID_GetKeyboardInfo(Interface *Itf)
{
  if (USBH_HID_KeyboardDecode(Itf) == USB_OK)
  {
    return &keybd_info;
  }
  else
  {
    return NULL;
  }
}

static USB_Status USBH_HID_KeyboardDecode(Interface *Itf)
{
	uint32_t x;

	if (Itf->HidRptLen == 0U)
	{
	    return USB_FAIL;
	}


	if (FifoRead(&Itf->buffer, &keybd_report_data, Itf->HidRptLen) !=0)
    {
	    if(keybd_report_data[0]   &   0b00000001) { keybd_info.lctrl  = 1;} else {keybd_info.lctrl    = 0;}
	    if(keybd_report_data[0]   &   0b00000010) { keybd_info.lshift = 1;} else {keybd_info.lshift   = 0;}
	    if(keybd_report_data[0]   &   0b00000100) { keybd_info.lalt   = 1;} else {keybd_info.lalt     = 0;}
	    if(keybd_report_data[0]   &   0b00001000) { keybd_info.lgui   = 1;} else {keybd_info.lgui     = 0;}
	    if(keybd_report_data[0]   &   0b00010000) { keybd_info.rctrl  = 1;} else {keybd_info.rctrl    = 0;}
	    if(keybd_report_data[0]   &   0b00100000) { keybd_info.rshift = 1;} else {keybd_info.rshift   = 0;}
	    if(keybd_report_data[0]   &   0b01000000) { keybd_info.ralt   = 1;} else {keybd_info.ralt     = 0;}
	    if(keybd_report_data[0]   &   0b10000000) { keybd_info.rgui   = 1;} else {keybd_info.rgui     = 0;}

	     for (x = 0; x < 6; x++)
	     {
	       keybd_info.keys[x] = keybd_report_data[x+2] ;
	     }

    return USB_OK;
  }
  return   USB_FAIL;
}
