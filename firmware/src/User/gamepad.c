#include "gamepad.h"

void ProcessGamepad(HID_gamepad_Info_TypeDef* joymap)
{

	if (joymap == NULL) return;

	if (joymap->gamepad_data)
		printf("gamepad_data = %08x\n", joymap->gamepad_data);
	if (joymap->gamepad_extraBtn)
		printf("gamepad_extraBtn = %08x\n", joymap->gamepad_extraBtn);

	// $TODO - double check this mapping
	GPIO_WriteBit(RT_GPIO_Port, RT_Pin, !((joymap->gamepad_data >> 0) & 0x1));
	GPIO_WriteBit(LT_GPIO_Port, LT_Pin, !((joymap->gamepad_data >> 1) & 0x1));
	GPIO_WriteBit(DN_GPIO_Port, DN_Pin, !((joymap->gamepad_data >> 2) & 0x1));
	GPIO_WriteBit(UP_GPIO_Port, UP_Pin, !((joymap->gamepad_data >> 3) & 0x1));
	GPIO_WriteBit(B1_GPIO_Port, B1_Pin, !((joymap->gamepad_data >> 4) & 0x1));
	GPIO_WriteBit(B2_GPIO_Port, B2_Pin, !((joymap->gamepad_data >> 6) & 0x1));

}
