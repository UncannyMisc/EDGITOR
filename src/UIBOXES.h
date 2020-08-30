#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

#include "plf_stack.h"

#include "FILES.h"

struct UIBOX_INFO;
struct UIBOX_ELEMENT_MAIN;

extern void uibox_set_char(UIBOX_INFO* ui, uint16_t char_pos, uint8_t _chr, COLOR _col, bool update);

extern void uibox_set_string(UIBOX_INFO* uibox, std::string _charlist, uint16_t x, uint16_t y, COLOR col, bool update);

extern UIBOX_INFO* UIBOX_PTR_HOME;
extern UIBOX_INFO* UIBOX_PTR_FILE_EXPLORER;
extern UIBOX_INFO* UIBOX_PTR_OPENED_FILES;

struct UIBOX_INFO {
	int file_type = -1;
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t w = 0;
	uint16_t h = 0;
	uint8_t chr_w = 0;
	uint8_t chr_h = 0;
	uint16_t pan_x = 0;
	uint16_t pan_y = 0;
	int8_t align_x = 0;
	int8_t align_y = 0;
	std::string title = "";
	SDL_Texture* texture = nullptr;
	plf::stack<uint16_t> update_stack;

	std::vector<bool> chr_update_bool;
	std::vector<uint8_t> chr_chr;
	std::vector<COLOR> chr_col;

	std::vector<std::shared_ptr<UIBOX_ELEMENT_MAIN>> element_list;

	bool update = true;
	bool shrink = false;
	bool is_menubar = false;
	bool pinned = false;
	bool can_scroll = false;

	enum TOPBAR_ELEMENT
	{
		GRAB,
		SHRINK,
		PIN,
		CLOSE
	};

	std::vector<std::pair<TOPBAR_ELEMENT, std::pair<uint8_t, bool>>> topbar_elements;

	void uibox_shrink()
	{
		if (!shrink)
		{
			shrink = 1;
			h = FONT_CHRH;
			uibox_set_char(this, 0, CHAR_BOXH, COL_BGUPDATE, 1);
			uibox_set_char(this, chr_w - 1, CHAR_BOXH, COL_BGUPDATE, 1);
			uibox_set_char(this, chr_w - 3, CHAR_ARWL, COL_ACCENT, 1);
		}
		else
		{
			shrink = 0;
			h = (chr_h * FONT_CHRH);
			uibox_set_char(this, 0, CHAR_BOXTL, COL_BGUPDATE, 1);
			uibox_set_char(this, chr_w - 1, CHAR_BOXTR, COL_BGUPDATE, 1);
			uibox_set_char(this, chr_w - 3, CHAR_ARWD, COL_ACCENT, 1);
		}
	}

	void uibox_pin()
	{
		if (!pinned)
		{
			pinned = 1;
			uibox_set_char(this, chr_w - 6, 0x0a, COL_ACCENT, 1);
		}
		else
		{
			pinned = 0;
			uibox_set_char(this, chr_w - 6, 0x09, COL_ACCENT, 1);
		}
	}

	void topbar_add_grab(uint8_t w)
	{
		std::pair<TOPBAR_ELEMENT, std::pair<uint8_t, bool>> _t = { TOPBAR_ELEMENT::GRAB,{w,0} };
		topbar_elements.push_back(_t);
	}

	void topbar_add_shrink(uint8_t w)
	{
		std::pair<TOPBAR_ELEMENT, std::pair<uint8_t, bool>> _t = { TOPBAR_ELEMENT::SHRINK,{w,0} };
		topbar_elements.push_back(_t);
		uibox_set_string(this, STR_NBSP STR_ARWD STR_NBSP, chr_w - 4, 0, COL_EMPTY, false);
	}

	void topbar_add_pin(uint8_t w)
	{
		std::pair<TOPBAR_ELEMENT, std::pair<uint8_t, bool>> _t = { TOPBAR_ELEMENT::PIN,{w,0} };
		topbar_elements.push_back(_t);
		uibox_set_string(this, STR_NBSP "\x09" STR_NBSP, chr_w - 7, 0, COL_EMPTY, false);
	}

	virtual void topbar_init() {};

	virtual void update_loop() {};

	virtual void init(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string _t, int _f)
	{
		file_type = _f;
		x = _x;
		y = _y;
		w = _w;
		h = _h;
		title = _t;

		chr_w = (uint8_t)floor(((float)_w / (float)FONT_CHRW) + 0.5f);
		chr_h = (uint8_t)floor(((float)_h / (float)FONT_CHRH) + 0.5f);
		update_stack.reserve(chr_w * chr_h);

		// MAKE WINDOW THE NEW ROUNDED CHAR SIZE
		w = (chr_w * FONT_CHRW);
		h = (chr_h * FONT_CHRH);

		texture = SDL_CreateTexture(RENDERER_MAIN, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, chr_w * FONT_CHRW, chr_h * FONT_CHRH);

		// FILL WINDOW WITH EMPTY CHARS
		for (uint16_t j = 0; j < chr_w * chr_h; j++)
		{
			chr_update_bool.push_back(1);
			chr_chr.push_back(' ');
			chr_col.push_back(COL_BLACK);
			update_stack.push(j);
		}

		// ADD BORDER
		for (uint8_t j = 0; j < chr_h; j++)
		{
			for (uint8_t i = 0; i < chr_w; i++)
			{
				if ((j > 0 && j < chr_h - 1) && (i > 0 && i < chr_w - 1))
				{
					continue;
				}
				chr_chr[j * chr_w + i] = (j == 0) ? ((i == 0) ? CHAR_BOXTL : ((i == chr_w - 1) ? CHAR_BOXTR : CHAR_BOXH)) :
					((j == chr_h - 1) ? ((i == 0) ? CHAR_BOXBL : ((i == chr_w - 1) ? CHAR_BOXBR : CHAR_BOXH)) : CHAR_BOXV);
			}
		}

		uibox_set_string(this, STR_NBSP + title + STR_NBSP, 1, 0, COL_EMPTY, 0);

		topbar_init();
	}
};

struct UIBOX_INFO_SCROLLABLE : public UIBOX_INFO {
	uint16_t scroll_x = 0;
	uint16_t scroll_y = 0;
	std::vector<uint16_t> scroll_element_list;
	bool scroll_element_list_create = true;
	uint16_t scroll_element_list_size = 0;
};

struct UIBOX_INFO_TOOLS : public UIBOX_INFO {
	void topbar_init() override
	{
		topbar_add_grab(chr_w - 7);
		topbar_add_pin(3);
		topbar_add_shrink(3);
	}

	void update_loop() override;
};

struct UIBOX_INFO_FILE_EXPLORER : public UIBOX_INFO_SCROLLABLE {
private:
	std::filesystem::path _open_file = "";
	std::filesystem::path _tCURRENT_PATH;
	std::filesystem::path _topen_file = "abc";
	bool _open_file_tick = true;

public:
	void topbar_init() override
	{
		can_scroll = true;
		is_menubar = true;
		topbar_add_shrink(chr_w);
	}

	void update_loop() override;
};

struct UIBOX_INFO_OPENED_FILES : public UIBOX_INFO_SCROLLABLE {
private:
	std::filesystem::path _topen_file = "abc";
	bool _open_file_tick = true;

public:
	std::filesystem::path _open_file = "";
	void topbar_init() override
	{
		can_scroll = true;
		is_menubar = true;
		topbar_add_shrink(chr_w);
	}

	void update_loop() override;
};

struct UIBOX_INFO_HOME : public UIBOX_INFO {
	void topbar_init() override
	{
		is_menubar = true;
		topbar_add_shrink(chr_w);
	}

	void update_loop() override;
};

extern UIBOX_INFO* uibox_add(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type);
extern UIBOX_INFO* uibox_add_tools(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type);
extern UIBOX_INFO* uibox_add_file_explorer(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type);
extern UIBOX_INFO* uibox_add_opened_files(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type);
extern UIBOX_INFO* uibox_add_home(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type);

extern bool UIBOX_IN_CLICK;
extern std::vector<std::unique_ptr<UIBOX_INFO>> UIBOXES;