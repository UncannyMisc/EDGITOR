#include "UIBOX_ELEMENTS.h"

// update UIBOX element input from keyboard
void UIBOX_INPUT_UPDATE()
{
	if (UIBOX_ELEMENT_CLICK != nullptr && UIBOX_ELEMENT_TYPE >= 0 && UIBOX_ELEMENT_CLICK->is_input)
	{
		char buffer[128] = {};
		switch (UIBOX_ELEMENT_TYPE)
		{
		case 0: // int?
			UIBOX_INPUT_VALUE_I = (int)clamp(KEY_TEXT_INT, 0, INT_MAX);
			sprintf_s(buffer, "%i", UIBOX_INPUT_VALUE_I);
			break;

		case 1: // uint8_t
			UIBOX_INPUT_VALUE_U8 = (uint8_t)clamp(KEY_TEXT_INT, 0, UINT8_MAX);
			sprintf_s(buffer, "%i", UIBOX_INPUT_VALUE_U8);
			break;

		case 2: // uint16_t
			UIBOX_INPUT_VALUE_U16 = (uint16_t)clamp(KEY_TEXT_INT, 0, UINT16_MAX);
			sprintf_s(buffer, "%i", UIBOX_INPUT_VALUE_U16);
			break;

		default:
			break;
		}

		UIBOX_ELEMENT_CLICK->text = "> " + std::string(buffer);
		UIBOX_ELEMENT_CLICK->sel_text = " > " + std::string(buffer);
		UIBOX_ELEMENT_CLICK->update(UIBOX_ELEMENT_CLICK->parent_uibox);
	}
}

UIBOX_ELEMENT_MAIN* UIBOX_ELEMENT_CLICK = nullptr;
