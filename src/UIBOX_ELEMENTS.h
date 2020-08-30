#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

#include "UIBOXES.h"

extern void UIBOX_INPUT_UPDATE();

struct UIBOX_ELEMENT_MAIN {
	bool highlight = true;
	bool over = false;
	bool prev_sel = false;
	bool prev_over = false;
	bool const_update = false;
	bool is_input = false;
	COLOR bg_col = COL_BLACK;

	UIBOX_INFO* parent_uibox;

	std::string text = "";
	std::string sel_text = "";
	uint16_t x = 0;
	uint16_t y = 0;
	// for W & H: 0=text size, -1=window size
	int16_t w = 0;
	int16_t h = 0;

	bool update_uibox = false;

	virtual void create(UIBOX_INFO*) = 0;

	virtual void set(void)
	{
		//
	}

	virtual void update(UIBOX_INFO* uibox)
	{
		for (uint16_t iy = 0; iy < h; iy++)
		{
			for (uint16_t ix = 0; ix < w; ix++)
			{
				uibox_set_char(uibox, x + ix + ((y + iy) * uibox->chr_w),
					prev_sel ? (ix < sel_text.size() ? (sel_text.c_str())[ix] : 32) : (ix < text.size() ? (text.c_str())[ix] : 32),
					(prev_over || prev_sel) ? COL_ACCENT : bg_col,
					1);
			}
		}
	}

	virtual bool is_sel()
	{
		return false;
	}
};

extern UIBOX_ELEMENT_MAIN* UIBOX_ELEMENT_CLICK;

struct UIBOX_ELEMENT_TEXTBOX : public UIBOX_ELEMENT_MAIN {
	void create(UIBOX_INFO* uibox) override
	{
		uibox_set_string(uibox, text, x, y, (highlight ? COL_ACCENT : COL_BGUPDATE), uibox->update);
		highlight = 0;
	}
};

template<typename T>
struct UIBOX_ELEMENT_BUTTON : public UIBOX_ELEMENT_MAIN {
	T* input_var = nullptr;
	T button_var;

	void create(UIBOX_INFO* uibox) override
	{
		uibox_set_string(uibox, text, x, y, COL_BGUPDATE, true);
	}

	void set() override
	{
		*input_var = button_var;
		if (update_uibox)
		{
			parent_uibox->update = true;
		}
	}

	bool is_sel() override
	{
		if (input_var == nullptr) return 0;
		return *input_var == button_var;
	}
};

template<typename T>
struct UIBOX_ELEMENT_VARBOX : public UIBOX_ELEMENT_MAIN {
	T* input_var = nullptr;
	T button_var;

	void create(UIBOX_INFO* uibox) override
	{
		highlight = 0;
		uibox_set_string(uibox, text, x, y, COL_WHITE, uibox->update);
	}

	void update(UIBOX_INFO* uibox) override
	{
		if (button_var == *input_var) return;
		button_var = *input_var;

		if (input_var == nullptr) return;

		//PRINT("BEEP");

		std::string var_text;
		
		if (typeid(T) == typeid(std::string))
		{
			var_text = *input_var;
		}
		else
		{
			char buffer[128];
			sprintf_s(buffer, "%i", *input_var);
			var_text = std::string(buffer);
		}

		for (uint16_t ix = 0; ix < 8; ix++)
		{
			uibox_set_char(uibox, x + ix + (y * uibox->chr_w),
				ix < var_text.size() ? (var_text.c_str())[ix] : 32,
				COL_BGUPDATE,
				1);
		}
	}

	bool is_sel() override
	{
		if (input_var == nullptr) return 0;
		
		return *input_var != button_var;
	}
};

struct UIBOX_ELEMENT_FILE_NEW : public UIBOX_ELEMENT_MAIN {
	std::string path = "";
	std::string filename = "";
	FILE_TYPE file_type;

	void create(UIBOX_INFO* uibox) override
	{
		uibox_set_string(uibox, text, x, y, COL_BGUPDATE, uibox->update);
	}

	void set() override
	{
		NEW_FILE_TYPE = file_type;
		UIBOX_PTR_HOME->update = 1;
	}

	bool is_sel() override
	{
		return NEW_FILE_TYPE == file_type;
	}
};

struct UIBOX_ELEMENT_CREATE_FILE : public UIBOX_ELEMENT_MAIN {

	void create(UIBOX_INFO* uibox) override
	{
		uibox_set_string(uibox, text, x, y, COL_BGUPDATE, uibox->update);
	}

	void set() override
	{
		if (NEW_FILE_PIXELS_CANVAS_W <= 0 || NEW_FILE_PIXELS_CANVAS_H <= 0) return;

		switch (NEW_FILE_TYPE)
		{
		case EMPTY:
			break;
		case ALL:
			break;
		case PIXELS:
			add_file_pixels(FILE_TYPE::PIXELS, "", "untitled.png", NEW_FILE_PIXELS_CANVAS_W, NEW_FILE_PIXELS_CANVAS_H);
			break;
		case MAP:
			break;
		case TEXT:
			break;
		case UNSUPPORTED:
			break;
		default:
			break;
		}
		UIBOX_PTR_HOME->uibox_shrink();
	}
};

template<typename T>
struct UIBOX_ELEMENT_INPUT : public UIBOX_ELEMENT_MAIN {
	T* input_var = nullptr;
	T default_value;

	void create(UIBOX_INFO* uibox) override
	{
		is_input = 1;
		bg_col = COL_DKGRAY;
		//uibox_set_string(uibox, text, x, y, COL_DKGRAY, uibox->update);
		update(uibox);
	}

	void set() override
	{
		if (UIBOX_ELEMENT_CLICK == this)
		{
			if (UIBOX_ELEMENT_TYPE >= 0)
			{
				UIBOX_ELEMENT_CLICK = nullptr;
				switch (UIBOX_ELEMENT_TYPE)
				{
				case 0: // int?
					*input_var = UIBOX_INPUT_VALUE_I;
					UIBOX_INPUT_VALUE_I = 0;
					break;

				case 1: // uint8_t
					*input_var = UIBOX_INPUT_VALUE_U8;
					UIBOX_INPUT_VALUE_U8 = 0;
					break;

				case 2: // uint16_t
					*input_var = UIBOX_INPUT_VALUE_U16;
					UIBOX_INPUT_VALUE_U16 = 0;
					break;

				default:
					break;
				}
				UIBOX_ELEMENT_TYPE = -1;
				KEY_TEXT_INT = 0;
				KEY_TEXT = "";
			}
			return;
		}

		UIBOX_ELEMENT_CLICK = this;

		int* UIBOX_INPUT_PTR_I = nullptr;
		int UIBOX_INPUT_VALUE_I = 0;
		uint8_t* UIBOX_INPUT_PTR_U8 = nullptr;
		uint8_t UIBOX_INPUT_VALUE_U8 = 0;
		uint16_t* UIBOX_INPUT_PTR_U16 = nullptr;
		uint16_t UIBOX_INPUT_VALUE_U16 = 0;

		if (typeid(T) == typeid(int))
		{
			UIBOX_ELEMENT_TYPE = 0;
			UIBOX_INPUT_PTR_I = (int*)input_var;
			UIBOX_INPUT_VALUE_I = default_value;
			//*input_var = default_value;
		}
		else
		if (typeid(T) == typeid(uint8_t))
		{
			UIBOX_ELEMENT_TYPE = 1;
			UIBOX_INPUT_PTR_U8 = (uint8_t*)input_var;
			UIBOX_INPUT_VALUE_U8 = default_value;
			//*input_var = default_value;
		}
		else
		if (typeid(T) == typeid(uint16_t))
		{
			UIBOX_ELEMENT_TYPE = 2;
			UIBOX_INPUT_PTR_U16 = (uint16_t*)input_var;
			UIBOX_INPUT_VALUE_U16 = default_value;
			//*input_var = default_value;
		}

		UIBOX_INPUT_UPDATE();
	}

	bool is_sel() override
	{
		if (input_var == nullptr) return false;
		return UIBOX_ELEMENT_CLICK == this;
	}
};


