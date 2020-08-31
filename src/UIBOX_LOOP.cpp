#include "UIBOX_LOOP.h"
#include "UIBOX_ELEMENTS.h"

void UIBOX_INIT()
{
	// SYSTEM / ALL
	//uibox_add_file_new(0, 0, 512, 512, "NEW FILE", FILE_TYPE::ALL);
	UIBOX_PTR_HOME = uibox_add_home(0, 0, 408, 680, "HOME", FILE_TYPE::ALL);
	
	UIBOX_PTR_FILE_EXPLORER = uibox_add_file_explorer(408 + FONT_CHRW, 0, 432, 680, "FILE EXPLORER", FILE_TYPE::ALL);
	UIBOX_PTR_OPENED_FILES = uibox_add_opened_files(408 + FONT_CHRW + FONT_CHRW + 432, 0, 864 / 2, 680, "CURRENT OPEN FILES", FILE_TYPE::ALL);
	
	// PIXELS
	uibox_add_tools(0, 0, 160, 500, "TOOLS", FILE_TYPE::PIXELS);
	uibox_add_color(0, 0, 512, 512, "COLOR", FILE_TYPE::PIXELS);
}

void UIBOX_LOOP()
{
	int8_t _tUIBOX_IN = UIBOX_IN;
	if (!UIBOX_IN_CLICK) UIBOX_IN = -1;
	uint8_t _tuibox_size = (uint8_t)UIBOXES.size() - 1;
	UIBOX_INFO* uibox;
	SDL_Rect rect;

	for (int8_t u = _tuibox_size; u >= 0; u--)
	{
		uibox = UIBOXES[u].get();

		if (uibox->file_type >= FILE_TYPE::UNSUPPORTED || (uibox->file_type != FILE_TYPE::ALL && uibox->file_type != CURRENT_FILE_PTR_TYPE)) continue;

		switch (uibox->align_x)
		{
		case 0: // no align
			break;

		case -1:
		case -2: // left
			uibox->x = 0;
			break;

		case 1: // center
			uibox->x = (WINDOW_MAIN_W / 2) - (uibox->w / 2);
			break;

		case 2:
		case 3: // right
			uibox->x = WINDOW_MAIN_W - uibox->w;
			break;
		default:
			break;
		}

		switch (uibox->align_y)
		{
		case 0: // no align
			break;

		case -1:
		case -2: // top
			uibox->y = 0;
			break;

		case 1: // middle
			uibox->y = (WINDOW_MAIN_H / 2) - (uibox->h / 2);
			break;

		case 2:
		case 3: // bottom
			uibox->y = WINDOW_MAIN_H - uibox->h;
			break;
		default:
			break;
		}
		
		if (!UIBOX_IN_CLICK && (point_in_rect(MOUSE_X, MOUSE_Y, uibox->x, uibox->y, uibox->w, uibox->h) || point_in_rect(MOUSE_PREVX, MOUSE_PREVY, uibox->x, uibox->y, uibox->w, uibox->h)))
		{
			UIBOX_IN = u;

			if (UIBOX_IN <= _tUIBOX_IN)
			{
				if (uibox->can_scroll && ((UIBOX_INFO_SCROLLABLE*)uibox)->scroll_element_list.size() > 0 && (MOUSEWHEEL_X != 0 || MOUSEWHEEL_Y != 0))
				{
					int16_t _tscrolly = ((UIBOX_INFO_SCROLLABLE*)uibox)->scroll_y;
					((UIBOX_INFO_SCROLLABLE*)uibox)->scroll_y = clamp(((UIBOX_INFO_SCROLLABLE*)uibox)->scroll_y - MOUSEWHEEL_Y, 0, ((UIBOX_INFO_SCROLLABLE*)uibox)->scroll_element_list.size() - 1);
					PRINT((int)_tscrolly);
					PRINT((int)((UIBOX_INFO_SCROLLABLE*)uibox)->scroll_y);
					PRINT(" ");
					if (_tscrolly != ((UIBOX_INFO_SCROLLABLE*)uibox)->scroll_y)
					{
						uibox->update = true;
					}
				}
			}
		}

		// TOPBAR ELEMENTS (grab, shrink, close, etc)
		if (!uibox->topbar_elements.empty())
		{
			bool _tbool_in = false;
			bool* _ttopbar_in = nullptr;
			bool _ttopbar_previn = false;
			bool _tcanclick = false;
			uint8_t _prev_w = 0;
			for (uint8_t i = 0; i < uibox->topbar_elements.size(); i++)
			{
				_tcanclick = 0;
				_ttopbar_in = &uibox->topbar_elements[i].second.second;
				_ttopbar_previn = *_ttopbar_in;
				_tbool_in = point_in_rect(MOUSE_X, MOUSE_Y, uibox->x + ((i + _prev_w) * FONT_CHRW), uibox->y, (uibox->topbar_elements[i].second.first * FONT_CHRW), FONT_CHRH) ||
					point_in_rect(MOUSE_PREVX, MOUSE_PREVY, uibox->x + ((i + _prev_w) * FONT_CHRW), uibox->y, (uibox->topbar_elements[i].second.first * FONT_CHRW), FONT_CHRH);

				if (!MOUSEBUTTON_LEFT && !MOUSEBUTTON_PRESSED_LEFT)
				{
					if ((UIBOX_IN <= _tUIBOX_IN) && _tbool_in && !(*_ttopbar_in) && !UIBOX_IN_CLICK) // only highlight if there's nothing above the UIBOX
					{
						for (uint8_t ic = 0; ic < uibox->topbar_elements[i].second.first; ic++)
						{
							uibox_set_char(uibox, i + ic + _prev_w, 0, COL_ACCENT, 1);
						}
						*_ttopbar_in = 1;
						_tcanclick = 1;
					}
					else
					if ((UIBOX_IN > _tUIBOX_IN || !_tbool_in) && *_ttopbar_in) // if it's below another UIBOX, clear the highlight
					{
						for (uint8_t ic = 0; ic < uibox->topbar_elements[i].second.first; ic++)
						{
							uibox_set_char(uibox, i + ic + _prev_w, 0, COL_BLACK, 1);
						}
						*_ttopbar_in = 0;
					}
				}

				if (UIBOX_IN <= _tUIBOX_IN && (_tbool_in || UIBOX_IN_CLICK) && (*_ttopbar_in || _ttopbar_previn) && MOUSEBUTTON_LEFT)
				{
					switch (uibox->topbar_elements[i].first)
					{
					case uibox->TOPBAR_ELEMENT::GRAB:
						if (!uibox->pinned)
						{
							if (MOUSEBUTTON_PRESSED_LEFT)
							{
								uibox->pan_x = (uint16_t)(MOUSE_X - uibox->x);
								uibox->pan_y = (uint16_t)(MOUSE_Y - uibox->y);
							}
							uibox->x = (uint16_t)clamp(MOUSE_X - uibox->pan_x, 0, WINDOW_MAIN_W - uibox->w);
							uibox->y = (uint16_t)clamp(MOUSE_Y - uibox->pan_y, 0, WINDOW_MAIN_H - uibox->h);
						}
						break;

					case uibox->TOPBAR_ELEMENT::SHRINK:
						if (MOUSEBUTTON_PRESSED_LEFT)
						{
							uibox->uibox_shrink();
						}
						break;

					case uibox->TOPBAR_ELEMENT::PIN:
						if (MOUSEBUTTON_PRESSED_LEFT)
						{
							uibox->uibox_pin();
						}
						break;

					case uibox->TOPBAR_ELEMENT::CLOSE:

						break;
					default:
						break;
					}
				}

				_prev_w += uibox->topbar_elements[i].second.first - 1;
			}
		}

		if (!uibox->is_menubar)
		{
			uibox->x = (uint16_t)clamp(uibox->x, 0, WINDOW_MAIN_W - uibox->w);
			uibox->y = (uint16_t)clamp(uibox->y, FONT_CHRH, WINDOW_MAIN_H - uibox->h);
		}

		if (!uibox->shrink)
		{
			if (!uibox->element_list.empty())
			{
				UIBOX_ELEMENT_MAIN* _element;
				bool _t_over = false;
				bool _t_is_sel = false;
				bool _t_update = false;

				for (uint16_t e = 0; e < uibox->element_list.size(); e++)
				{
					_t_update = 0;
					_element = uibox->element_list[e].get();

					if (_element->const_update)
					{
						_element->update(uibox);
						continue;
					}

					if (_element->highlight)
					{
						_t_over = (point_in_rect(MOUSE_X, MOUSE_Y,
							uibox->x + (FONT_CHRW * _element->x), uibox->y + (FONT_CHRH * _element->y),
							FONT_CHRW * _element->w, FONT_CHRH * _element->h));

						if (_t_over)
						{
							if (MOUSEBUTTON_PRESSED_LEFT || (_element->hold && MOUSEBUTTON_LEFT))
							{
								//UIBOX_ELEMENT_CLICK = _element;
								//std::cout << "          " << std::endl << UIBOX_ELEMENT_CLICK << std::endl;
								_element->set();
								//std::cout << "          " << std::endl << UIBOX_ELEMENT_CLICK << std::endl;
							}
						}
						else
						if (_element->is_input && _element == UIBOX_ELEMENT_CLICK && UIBOX_ELEMENT_TYPE >= 0)
						{
							if (MOUSEBUTTON_PRESSED_LEFT)
							{
								//confirm_input();
								_element->set();
							}
						}

						if (_element->prev_over != _t_over)
						{
							_element->prev_over = _t_over;
							_t_update = 1;
						}
					}

					_t_is_sel = _element->is_sel();

					if (_element->prev_sel != _t_is_sel)
					{
						_element->prev_sel = _t_is_sel;
						_t_update = 1;
					}

					//if (uibox->element_update) _t_update = 1;

					if (_t_update)
					{
						//if (!_element->tick_update) _element->tick_update = ELEMENT_UPDATE_TICK; else { --_element->tick_update; continue; }
						_element->update(uibox);
					}
				}
			}

			if (uibox->update)
			{
				uibox->update = 0;
				uibox->update_loop();
			}
		}

		SDL_SetTextureBlendMode(uibox->texture, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(RENDERER_MAIN, uibox->texture);

		uint8_t _tchr_w = uibox->chr_w;
		uint8_t _tchr_h = uibox->chr_h;
		uint16_t j;
		SDL_Texture** _tchr;
		COLOR* _tcol;

		while (!uibox->update_stack.empty())
		{
			j = uibox->update_stack.top();
			uibox->update_stack.pop();
			_tchr = &FONTMAP[uibox->chr_chr[j]];
			_tcol = &uibox->chr_col[j];

			rect = { (j % _tchr_w) * FONT_CHRW, (j / _tchr_w) * FONT_CHRH, FONT_CHRW, FONT_CHRH };

			if (_tcol->a)
			{
				SDL_SetTextureColorMod(FONTMAP[0xdb], _tcol->r, _tcol->g, _tcol->b);
				SDL_RenderCopy(RENDERER_MAIN, FONTMAP[0xdb], NULL, &rect);
				_tcol->a = 0;
			}

			SDL_SetTextureColorMod(*_tchr, 255, 255, 255);
			SDL_RenderCopy(RENDERER_MAIN, *_tchr, NULL, &rect);
		}

		uibox->draw_loop();

		SDL_SetRenderTarget(RENDERER_MAIN, nullptr);

		if (uibox->shrink)
		{
			SDL_Rect shrink_rect = { 0,0,uibox->w,FONT_CHRH };
			rect = { uibox->x, uibox->y, uibox->w, FONT_CHRH };
			SDL_RenderCopy(RENDERER_MAIN, uibox->texture, &shrink_rect, &rect);
		}
		else
		{
			rect = { uibox->x, uibox->y, uibox->w, uibox->h };
			SDL_RenderCopy(RENDERER_MAIN, uibox->texture, nullptr, &rect);
		}
	}

	if (UIBOX_IN >= 0)
	{
		if (MOUSEBUTTON_PRESSED_LEFT)
		{
			std::rotate(UIBOXES.begin(), UIBOXES.begin() + UIBOX_IN, UIBOXES.begin() + UIBOX_IN + 1);

			UIBOX_IN = 0;
			UIBOX_IN_CLICK = 1;
		}
		else
		if (UIBOX_IN_CLICK && !MOUSEBUTTON_LEFT) {
			UIBOX_IN_CLICK = 0;
		}

		// ELEMENTS
	}
}
