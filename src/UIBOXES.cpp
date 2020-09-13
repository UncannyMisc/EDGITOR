#include "UIBOXES.h"
#include "UIBOX_ELEMENTS_FUNCTIONS.h"

UIBOX_INFO* UIBOX_PTR_HOME;
UIBOX_INFO* UIBOX_PTR_FILE_EXPLORER;
UIBOX_INFO* UIBOX_PTR_OPENED_FILES;

//-- uibox helper functions

void uibox_set_char(UIBOX_INFO* ui, uint16_t char_pos, uint8_t _chr, COLOR _col, bool update)
{
	if (_chr) ui->chr_chr[char_pos] = _chr; // set _chr to 0 to not update it

	if (_col != COL_EMPTY)
	{
		if (_col == COL_BGUPDATE) ui->chr_col[char_pos].a = 1; else
		ui->chr_col[char_pos] = _col;
	}

	if (!update || !ui->chr_update_bool[char_pos]) return;
	ui->chr_update_bool[char_pos] = 1;

	ui->update_stack.push(char_pos);
}

void uibox_set_string(UIBOX_INFO* uibox, std::string _charlist, uint16_t x, uint16_t y, COLOR col, bool update)
{
	const char* _chars = _charlist.c_str();
	for (uint16_t j = 0; j < _charlist.size(); j++)
	{
		uibox_set_char(uibox, j + (y * uibox->chr_w + x), _chars[j], col, update);
	}
}

UIBOX_INFO* uibox_add(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type)
{
	auto new_uibox = std::make_unique<UIBOX_INFO>();
	new_uibox->init(_x, _y, _w, _h, title, _file_type);
	// add variables for element_loop()
	UIBOXES.push_back(std::move(new_uibox));
	return UIBOXES.back().get();
}

//-- implimentations of uibox update functions

void UIBOX_INFO_TOOLS::update_loop()
{
	uibox_element_add_button<int>(this, 2, 2, 0, 1, "BRUSH", "> BRUSH", &CURRENT_TOOL, TOOL::BRUSH);
	uibox_element_add_button<int>(this, 2, 3, 0, 1, "ERASER", "> ERASER", &CURRENT_TOOL, TOOL::ERASER);
	uibox_element_add_button<int>(this, 2, 4, 0, 1, "PICKER", "> PICKER", &CURRENT_TOOL, TOOL::PICKER);
	uibox_element_add_button<int>(this, 2, 5, 0, 1, "FILL", "> FILL", &CURRENT_TOOL, TOOL::FILL);
	uibox_element_add_button<int>(this, 2, 6, 0, 1, "PAN", "> PAN", &CURRENT_TOOL, TOOL::PAN);
	uibox_element_add_button<int>(this, 2, 6, 0, 1, "SELECT", "> SELECT", &CURRENT_TOOL, TOOL::SELECT);
}
UIBOX_INFO* uibox_add_tools(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type)
{
	auto new_uibox = std::make_unique<UIBOX_INFO_TOOLS>();
	new_uibox->init(_x, _y, _w, _h, title, _file_type);
	// add variables for element_loop()
	UIBOXES.push_back(std::move(new_uibox));
	return UIBOXES.back().get();
}

void UIBOX_INFO_FILE_EXPLORER::update_loop()
{
	if (!this->element_list.empty())
	{
		int _tx, _ty;
		std::string _tchar;
		for (int i = 0; i < element_list.size(); i++)
		{
			_tx = element_list[i]->x;
			_ty = element_list[i]->y;
			_tchar = element_list[i]->text;
			for (uint16_t j = 0; j < _tchar.size(); j++)
			{
				// clearing the previous elements
				uibox_set_char(this, j + (_ty * chr_w + _tx), 32, COL_BLACK, 1);
			}
		}
	}

	/*if (!this->scroll_element_list.empty() && !this->element_list.empty())
	{
		int _offset = 0;
		for (int i = 0; i < this->scroll_element_list.size(); i++)
		{
			this->element_list.erase(this->element_list.begin() + this->scroll_element_list[i - _offset]);
			++_offset;
		}
	}
	scroll_element_list.clear();*/
	scroll_element_list.clear();
	element_list.clear();

	if (_tCURRENT_PATH != CURRENT_PATH)
	{
		scroll_y = 0;
		_tCURRENT_PATH = CURRENT_PATH;
	}

	if (_topen_file != _open_file)
	{
		if (!_open_file_tick)
		{
			open_file(_open_file);
		}
		else _open_file_tick = false;
		_topen_file = _open_file;
	}

	//{
		//element_list.clear();

		int16_t _tw = -1;
		std::filesystem::path _tpath;
		std::string _element_name;
		for (auto& _element : CURRENT_PATH)
		{
			_tpath /= _element;
			if (_tw <= -1)
			{
				_tw = 0;
				continue;
			}
			else
			{
				if (_tw <= 0)
				{
					_element_name = _tpath.string();
				}
				else
				{
					_element_name = _element.string() + "\\";
				}
			}
			uibox_element_add_button(this, 2 + _tw, 3, 1, 1, _element_name, _element_name, &CURRENT_PATH, _tpath);

			_tw += _element_name.size();
			auto _telement = element_list.back();
			_telement->update_uibox = true;
		}

		uibox_element_add_button(this, 2, 2, 1, 1, STR_ARWL STR_LINEH, STR_ARWL STR_LINEH, &CURRENT_PATH, CURRENT_PATH.parent_path());
		auto _telement = element_list.back();
		_telement->update_uibox = true;
		uibox_element_add_textbox(this, 2, 4, false, STR_LINEV);
		UPDATE_PATH_FILES();

		scroll_element_list_create = false;
	//}

	//PRINT(_tCURRENT_PATH.string());

	//scroll_element_list_size = (uint16_t)CURRENT_PATH_FILES.size();
	std::string _tfilename;
	for (int _file = 0; _file < CURRENT_PATH_FILES.size(); ++_file)
	{
		scroll_element_list.push_back(_file);
		if (_file - scroll_y < 0) continue; else if ((5 + _file) - scroll_y > chr_h - 2) continue;

		//if (CURRENT_PATH_FILES[_file].filename.size() > (chr_w - 6)) // shorten name if it's wider than uibox
		//{
			//CURRENT_PATH_FILES[_file].first.erase(this->chr_w - 10);
			//CURRENT_PATH_FILES[_file].first += "...";
		//}

		if (CURRENT_PATH_FILES[_file].is_directory)
		{
			_tfilename = (_file < (CURRENT_PATH_FILES.size() - 1) ? STR_LINEBL STR_ARWR " " : STR_LINEBL STR_ARWR " ") + CURRENT_PATH_FILES[_file].filename + "\\";

			uibox_element_add_button(this, 2, (5 + _file) - scroll_y, 1, 1, _tfilename, _tfilename, &CURRENT_PATH, CURRENT_PATH_FILES[_file].path);
			auto _telement = element_list.back();
			_telement->update_uibox = true;
		}
		else
		{
			_tfilename = (_file < (CURRENT_PATH_FILES.size() - 1) ? STR_LINEVR " " : STR_LINEBL " ") + CURRENT_PATH_FILES[_file].filename;

			uibox_element_add_button(this, 2, (5 + _file) - this->scroll_y, 1, 1, _tfilename, _tfilename, &_open_file, CURRENT_PATH_FILES[_file].path);
			auto _telement = element_list.back();
			_telement->update_uibox = true;

			//std::string _tstr = PATH_FILES[_file].first.substr(PATH_FILES[_file].first.size() - 3, 3);
			//if (_tstr == "png" || _tstr == "PNG")
			//{
			//uibox_add_element_button_files_load(this, 2, (4 + _file) - this->scroll_y, 0, 1, (_file < (PATH_FILES.size() - 1) ? "\xc5 " : "\xc1 ") + PATH_FILES[_file].first, CURRENT_PATH, PATH_FILES[_file].first);
			//}
			//else
				//uibox_add_element_textbox(this, 2, (4 + _file) - this->scroll_y, "\xb3 " + PATH_FILES[_file].first);
		}
	}
}
UIBOX_INFO* uibox_add_file_explorer(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type)
{
	auto new_uibox = std::make_unique<UIBOX_INFO_FILE_EXPLORER>();
	new_uibox->init(_x, _y, _w, _h, title, _file_type);
	// add variables for element_loop()
	new_uibox->align_y = -2;
	UIBOXES.push_back(std::move(new_uibox));
	return UIBOXES.back().get();
}

void UIBOX_INFO_OPENED_FILES::update_loop()
{
	if (!this->element_list.empty())
	{
		int _tx, _ty;
		std::string _tchar;
		for (int i = 0; i < element_list.size(); i++)
		{
			_tx = element_list[i]->x;
			_ty = element_list[i]->y;
			_tchar = element_list[i]->text;
			for (uint16_t j = 0; j < _tchar.size(); j++)
			{
				// clearing the previous elements
				uibox_set_char(this, j + (_ty * chr_w + _tx), 32, COL_BLACK, 1);
			}
		}
	}
	scroll_element_list.clear();
	element_list.clear();

	if (_topen_file != _open_file)
	{
		if (!_open_file_tick)
		{
			open_file(_open_file);
		}
		else _open_file_tick = false;
		_topen_file = _open_file;
	}

	std::string _tfilename;
	for (int _file = 0; _file < OPEN_FILES.size(); ++_file)
	{
		scroll_element_list.push_back(_file);
		if (_file - scroll_y < 0) continue; else if ((2 + _file) - scroll_y > chr_h - 2) continue;

		_tfilename = STR_ARWR " " + OPEN_FILES[_file]->filename;

		uibox_element_add_button(this, 2, (2 + _file) - this->scroll_y, 1, 1, _tfilename, _tfilename, &_open_file, OPEN_FILES[_file]->path);
		auto _telement = element_list.back();
		_telement->update_uibox = true;

	}
}
UIBOX_INFO* uibox_add_opened_files(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type)
{
	auto new_uibox = std::make_unique<UIBOX_INFO_OPENED_FILES>();
	new_uibox->init(_x, _y, _w, _h, title, _file_type);
	// add variables for element_loop()
	new_uibox->align_y = -2;
	UIBOXES.push_back(std::move(new_uibox));
	return UIBOXES.back().get();
}

void UIBOX_INFO_HOME::update_loop()
{
	// mac: open
	// linux: xdg-open
	//std::string op = std::string("start ").append("https://twitter.com/ENDESGA");
	//system(op.c_str());

	static bool create = true;
	static int main_elements_n = 0;

	int _txp1 = ((this->chr_w - 2) / 2);
	int _txp2 = (this->chr_w - ((this->chr_w - 8) / 2) + 1);

	if (create)
	{

		char EDG_LOGO[] = {
			0xdb,0xdf,0xdf,0xdf,0xdf,0xdf,0xdb,
			0xdb,0x00,0x00,0x00,0x00,0x00,0xdb,
			0xdb,0x00,0x00,0x00,0x00,0x00,0xdb,
			0xdb,0xdf,0xdf,0xdf,0xdb,0xdf,0xdb,
			0xdb,0x00,0x00,0x00,0xdb,0xdf,0xdb,
			0xdf,0xdf,0xdf,0xdf,0xdf,0xdf,0xdf
		};
		for (int i = 0; i < 42; i++)
		{
			uibox_set_char(this, (((i / 7) + 2) * this->chr_w + ((i % 7) + 2)), EDG_LOGO[i], COL_EMPTY, 0);
		}

		uibox_element_add_textbox(this, 12, 2, 1, "EDGITOR : v" + EDGITOR_VERSION);
		uibox_element_add_textbox(this, 12, 3, 0, "Open Source Omnieditor");
		uibox_element_add_textbox(this, 12, 4, 0, "- - - - - - -");
		uibox_element_add_textbox(this, 12, 6, 0, "by ENDESGA");

		uibox_element_add_textbox(this, 2, 10, 0, "RECENT:");

		std::string _thw = "";
		for (int i = 0; i < this->chr_w - 4; i++)
		{
			_thw += STR_LINEH;
		}
		uibox_element_add_textbox(this, 2, _txp1 + 1, 0, _thw);

		uibox_element_add_textbox(this, 2, _txp1 + 2, 1, "CREATE NEW FILE:");

		uibox_element_add_file_new(this, 2, _txp1 + 4, 12, 1, "\xdc\xdf - PIXEL CANVAS", "> \xdc\xdf - PIXEL CANVAS", "untitled.png", FILE_TYPE::PIXELS);
		uibox_element_add_file_new(this, 2, _txp1 + 6, 11, 1, "[] - MAP CONSTRUCT", "> [] - MAP CONSTRUCT", "untitled.map", FILE_TYPE::MAP);
		uibox_element_add_file_new(this, 2, _txp1 + 8, 15, 1, "Aa - TEXT FILE", "> Aa - TEXT FILE", "untitled.txt", FILE_TYPE::TEXT);

		for (int i = _txp1 + 2; i < (this->chr_h - 1); i++)
		{
			uibox_set_char(this, (i * this->chr_w + _txp2), CHAR_LINEV, COL_EMPTY, 0);
		}

		create = 0;
		main_elements_n = this->element_list.size();
	}

	if (NEW_FILE_TYPE != FILE_TYPE::EMPTY) {

		while (main_elements_n < this->element_list.size())
		{
			this->element_list.erase(this->element_list.end() - 1);
		}

		for (int i = 0; i < 18; i++)
			for (int j = 0; j < 16; j++)
			{
				uibox_set_char(this, ((_txp1 + 2 + j) * this->chr_w + (_txp2 + 1 + i)), CHAR_NBSP, COL_BLACK, 1);
			}

		switch (NEW_FILE_TYPE)
		{
		case FILE_TYPE::PIXELS:
			uibox_element_add_textbox(this, _txp2 + 2, _txp1 + 2, 1, "EDIT CANVAS:");

			uibox_element_add_textbox(this, _txp2 + 2, _txp1 + 4, 0, "CANVAS W:");
			uibox_element_add_input<uint16_t>(this, _txp2 + 2, _txp1 + 5, 14, 1, "> ", " > ", (uint16_t)16, &NEW_FILE_PIXELS_CANVAS_W);
			uibox_element_add_textbox(this, _txp2 + 2, _txp1 + 7, 0, "CANVAS H:");
			uibox_element_add_input<uint16_t>(this, _txp2 + 2, _txp1 + 8, 14, 1, "> ", " > ", (uint16_t)16, &NEW_FILE_PIXELS_CANVAS_H);
			uibox_element_add_textbox(this, _txp2 + 2, _txp1 + 10, 0, "BG COLOR:");
			uibox_element_add_textbox(this, _txp2 + 2, _txp1 + 13, 0, "BIT DEPTH:");
			uibox_element_add_button<int>(this, _txp2 + 2, _txp1 + 14, 1, 1, STR_NBSP "1" STR_NBSP, STR_NBSP  "1" STR_NBSP, &PIXEL_BIT_DEPTH, 1);
			uibox_element_add_button<int>(this, _txp2 + 5, _txp1 + 14, 1, 1, STR_NBSP "2" STR_NBSP, STR_NBSP  "2" STR_NBSP, &PIXEL_BIT_DEPTH, 2);
			uibox_element_add_button<int>(this, _txp2 + 8, _txp1 + 14, 1, 1, STR_NBSP "4" STR_NBSP, STR_NBSP  "4" STR_NBSP, &PIXEL_BIT_DEPTH, 4);
			uibox_element_add_button<int>(this, _txp2 + 11, _txp1 + 14, 1, 1, STR_NBSP "8" STR_NBSP, STR_NBSP  "8" STR_NBSP, &PIXEL_BIT_DEPTH, 8);
			uibox_element_add_textbox(this, _txp2 + 14, _txp1 + 14, 0, "bits");
			uibox_element_add_create_file(this, _txp2 + 1, _txp1 + 16, "> CREATE NEW FILE");
			break;

		case FILE_TYPE::MAP:
			uibox_element_add_textbox(this, _txp2 + 2, _txp1 + 2, 1, "EDIT MAP:");
			break;

		default:
			break;
		}
	}
}
UIBOX_INFO* uibox_add_home(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type)
{
	auto new_uibox = std::make_unique<UIBOX_INFO_HOME>();
	new_uibox->init(_x, _y, _w, _h, title, _file_type);
	// add variables for element_loop()
	new_uibox->align_x = -2;
	new_uibox->align_y = -2;
	UIBOXES.push_back(std::move(new_uibox));
	return UIBOXES.back().get();
}

void UIBOX_INFO_COLOR::draw_loop()
{
	SDL_Rect _trect = { FONT_CHRW * 2,FONT_CHRH * 2,256,256 };
	//SDL_RenderCopy(RENDERER_MAIN, gradient, nullptr, &_trect);
}
void UIBOX_INFO_COLOR::update_loop()
{
	COLOR _tcol;
	uint8_t _w = 42;
	uint8_t _h = 22;

	for (uint8_t j = 0; j < _h; j++)
	{
		for (uint8_t i = 0; i < _w; i++)
		{
			if ((j > 0 && j < _h - 1) && (i > 0 && i < _w - 1))
			{
				continue;
			}
			chr_chr[(j + 3) * chr_w + (i + 1)] = (j == 0) ? ((i == 0) ? CHAR_LINETL : ((i == _w - 1) ? CHAR_LINETR : CHAR_LINEH)) :
				((j == _h - 1) ? ((i == 0) ? CHAR_LINEBL : ((i == _w - 1) ? CHAR_LINEBR : CHAR_LINEH)) : CHAR_LINEV);
		}
	}

	_w = 40;
	_h = 20;

	new_col = hsv_to_rgb((float)box_xyz.x / (float)(_w - 1), 1.0f - ((float)box_xyz.y / (float)(_h - 1)), (float)box_xyz.z / (_w - 1));
	PRINT((int)new_col.r);
	PRINT((int)new_col.g);
	PRINT((int)new_col.b);
	PRINT((int)new_col.a);
	CURRENT_FILE_PTR_PIXELS->brush_color[CURRENT_FILE_PTR_PIXELS->current_brush_color] = new_col;

	element_list.clear();

	for (uint16_t x = 0; x < _w; x++)
	{
		for (uint16_t y = 0; y < _h; y++)
		{
			_tcol = hsv_to_rgb((float)x / (float)(_w - 1), 1.0f - ((float)y / (float)(_h - 1)), (float)box_z / (_w - 1));
			uibox_element_add_button<vec3>(this, x + 2, y + 4, 1, 1, STR_NBSP, STR_NBSP,
				&box_xyz, vec3{ (uint8_t)x, (uint8_t)y, box_z}, _tcol, 1);
			auto _telement = element_list.back();
			_telement->update_uibox = true;
		}

		uibox_element_add_button<uint8_t>(this, x + 2, 25, 1, 1, STR_NBSP, STR_NBSP, &box_z, (uint8_t)x, hsv_to_rgb(1.0f, 1.0f, (float)x / (float)(_w - 1)), 1);
		auto _telement = element_list.back();
		_telement->update_uibox = true;
	}

	/*switch (picker_type)
	{
	case 0:
		new_col = hsv_to_rgb((float)box_xyza.x / (float)(w - 1), 1.0f - ((float)box_xyza.y / (float)(h - 1)), (float)box_xyza.z / (w - 1));
		break;

	case 1:
		new_col = hsv_to_rgb((float)box_xyza.x / (float)(w - 1), (float)box_xyza.z / (w - 1), 1.0f - ((float)box_xyza.y / (float)(h - 1)));
		break;

	case 2:
		new_col = hsv_to_rgb((float)box_xyza.z / (w - 1), (float)box_xyza.x / (float)(w - 1), 1.0f - ((float)box_xyza.y / (float)(h - 1)));
		break;

	default:
		break;
	}

	new_col = hsv_to_rgb((float)box_xyza.x / (float)(w - 1), 1.0f - ((float)box_xyza.y / (float)(h - 1)), (float)box_xyza.z / (w - 1));

	CURRENT_FILE_PTR_PIXELS->brush_color[CURRENT_FILE_PTR_PIXELS->current_brush_color] = new_col;

	COLOR _tbrushcol = CURRENT_FILE_PTR_PIXELS->brush_color[CURRENT_FILE_PTR_PIXELS->current_brush_color];
	uint8_t _talpha = _tbrushcol.a;

	COLOR _tcol;
	for (uint16_t x = 0; x < w; x++)
	{
		for (uint16_t y = 0; y < h; y++)
		{
			switch (picker_type)
			{
			case 0:
				_tcol = hsv_to_rgb((float)x / (float)(w - 1), 1.0f - ((float)y / (float)(h - 1)), (float)box_z / (w - 1));
				uibox_element_add_button<vec4>(this, x + 2, y + 3, 1, 1, STR_NBSP, STR_NBSP,
					&box_xyza, vec4{(uint8_t)x, (uint8_t)y, box_z, _talpha}, _tcol, 1);
				break;

			case 1:
				_tcol = hsv_to_rgb((float)x / (float)(w - 1), (float)box_z / (w - 1), 1.0f - ((float)y / (float)(h - 1)));
				uibox_element_add_button<vec4>(this, x + 2, y + 3, 1, 1, STR_NBSP, STR_NBSP,
					&box_xyza, vec4{ (uint8_t)x, box_z, (uint8_t)y, _talpha }, _tcol, 1);
				break;

			case 2:
				_tcol = hsv_to_rgb((float)box_z / (w - 1), (float)x / (float)(w - 1), 1.0f - ((float)y / (float)(h - 1)));
				uibox_element_add_button<vec4>(this, x + 2, y + 3, 1, 1, STR_NBSP, STR_NBSP,
					&box_xyza, vec4{ box_z, (uint8_t)x, (uint8_t)y, _talpha }, _tcol, 1);
				break;

			default:
				break;
			}
		}

		//uibox_set_char(this, (23) * chr_w + (x + 2), CHAR_NBSP, hsv_to_rgb((float)x / (float)(w - 1), 1.0f, 1.0f), 1);

		switch (picker_type)
		{
		case 0:
			uibox_element_add_button<uint8_t>(this, x + 2, 24, 1, 1, STR_NBSP, STR_NBSP, &box_z, (uint8_t)x, hsv_to_rgb((float)x / (float)(w - 1), 1.0f, 1.0f), 1);
			break;

		case 1:
			uibox_element_add_button<uint8_t>(this, x + 2, 24, 1, 1, STR_NBSP, STR_NBSP, &box_z, (uint8_t)x, hsv_to_rgb(1.0f, (float)x / (float)(w - 1), 1.0f), 1);
			break;

		case 2:
			uibox_element_add_button<uint8_t>(this, x + 2, 24, 1, 1, STR_NBSP, STR_NBSP, &box_z, (uint8_t)x, hsv_to_rgb(1.0f, 1.0f, (float)x / (float)(w - 1)), 1);
			break;

		default:
			break;
		}

		//uibox_element_add_button<uint8_t>(this, x + 2, 24, 1, 1, STR_NBSP, STR_NBSP, &box_n, (uint8_t)x, hsv_to_rgb((float)x / (float)(w - 1), 1.0f, 1.0f), 1);
		auto _telement = element_list.back();
		_telement->update_uibox = true;
	}*/


}
UIBOX_INFO* uibox_add_color(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, std::string title, int _file_type)
{
	auto new_uibox = std::make_unique<UIBOX_INFO_COLOR>();
	new_uibox->init(_x, _y, _w, _h, title, _file_type);
	// add variables for element_loop()
	UIBOXES.push_back(std::move(new_uibox));
	return UIBOXES.back().get();
}

bool UIBOX_IN_CLICK = false;
std::vector<std::unique_ptr<UIBOX_INFO>> UIBOXES;
