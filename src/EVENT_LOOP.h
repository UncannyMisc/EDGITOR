#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"
#include "FILES.h"

#include "UIBOX_ELEMENTS.h"
#include "CLIPBOARD.h"
/*
			key and mouse var reset
			UIBox input update
			Drag and Drop File input
			key and mouse var set
			Undo and Redo
			Paste
			Save File
*/
inline void EVENT_LOOP()
{
	// move last values
	MOUSEBUTTON_PREVLEFT = MOUSEBUTTON_LEFT;
	MOUSEBUTTON_PREVMIDDLE = MOUSEBUTTON_MIDDLE;
	MOUSEBUTTON_PREVRIGHT = MOUSEBUTTON_RIGHT;
	MOUSEWHEEL_PREVX = MOUSEWHEEL_X;
	MOUSEWHEEL_PREVY = MOUSEWHEEL_Y;

	KEYBOARD_PREVCTRL = KEYBOARD_CTRL;
	KEYBOARD_PREVSHIFT = KEYBOARD_SHIFT;
	KEYBOARD_PREVALT = KEYBOARD_ALT;
	KEYBOARD_PREVSPACE = KEYBOARD_SPACE;
	KEYBOARD_PREVENTER = KEYBOARD_ENTER;
	KEYBOARD_PREVESC = KEYBOARD_ESC;

	KEYBOARD_PREVANY = KEYBOARD_ANY;

	// reset current values
	MOUSEBUTTON_PRESSED_LEFT = 0;
	MOUSEBUTTON_PRESSED_MIDDLE = 0;
	MOUSEBUTTON_PRESSED_RIGHT = 0;
	KEYBOARD_PRESSED_ALT = 0;
	KEYBOARD_PRESSED_CTRL = 0;
	KEYBOARD_PRESSED_SHIFT = 0;
	KEYBOARD_PRESSED_SPACE = 0;
	KEYBOARD_PRESSED_ENTER = 0;
	KEYBOARD_PRESSED_ESC = 0;

	KEYBOARD_PRESSED_ANY = 0;

	FUNCTION_UNDO = false;
	FUNCTION_REDO = false;

	MOUSEWHEEL_X = 0;
	MOUSEWHEEL_Y = 0;

	MOUSE_PREVX = MOUSE_X;
	MOUSE_PREVY = MOUSE_Y;

	int DISPLAY_MOUSE_X, DISPLAY_MOUSE_Y;
	SDL_GetGlobalMouseState(&DISPLAY_MOUSE_X, &DISPLAY_MOUSE_Y);
	MOUSE_X = (DISPLAY_MOUSE_X - WINDOW_MAIN_X);
	MOUSE_Y = (DISPLAY_MOUSE_Y - WINDOW_MAIN_Y);

	// update UIBOX element input from keyboard
	auto KEY_TEXT_UPDATE = []
	{
		KEY_TEXT_INT = ctoi(KEY_TEXT.c_str());

		UIBOX_INPUT_UPDATE();
	};

	SDL_Event EVENT{};

	// convert between SDL events and useful global values
	while (SDL_PollEvent(&EVENT))
	{
		switch (EVENT.type)
		{

		case SDL_DROPFILE: {
			char* dropped_filedir = EVENT.drop.file;
			open_file(dropped_filedir);
			PRINT("OPEN EXTERNAL FILE");
			SDL_free(dropped_filedir);
		} break;

		case SDL_TEXTINPUT:
			if (SDL_strlen(EVENT.text.text) == 0 || EVENT.text.text[0] == '\n') break;
			KEY_TEXT += EVENT.text.text;
			
			KEY_TEXT_UPDATE();
			break;

		case SDL_WINDOWEVENT:
			switch (EVENT.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				QUIT = 1;
				break;

			/*case SDL_WINDOWEVENT_RESIZED:
				if (SDL_GetWindowMaxim)
				break;*/
			}
			break;

			// user inputs
		case SDL_MOUSEBUTTONUP:
			switch (EVENT.button.button)
			{
			case SDL_BUTTON_LEFT:
				MOUSEBUTTON_LEFT = 0;
				MOUSEBUTTON_PRESSED_LEFT = 0;
				break;

			case SDL_BUTTON_MIDDLE:
				MOUSEBUTTON_MIDDLE = 0;
				MOUSEBUTTON_PRESSED_MIDDLE = 0;
				break;

			case SDL_BUTTON_RIGHT:
				MOUSEBUTTON_RIGHT = 0;
				MOUSEBUTTON_PRESSED_RIGHT = 0;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (EVENT.button.button)
			{
			case SDL_BUTTON_LEFT:
				MOUSEBUTTON_LEFT = 1;
				MOUSEBUTTON_PRESSED_LEFT = 1;
				break;

			case SDL_BUTTON_MIDDLE:
				MOUSEBUTTON_MIDDLE = 1;
				MOUSEBUTTON_PRESSED_MIDDLE = 1;
				break;

			case SDL_BUTTON_RIGHT:
				MOUSEBUTTON_RIGHT = 1;
				MOUSEBUTTON_PRESSED_RIGHT = 1;
				break;
			}
			break;

		case SDL_MOUSEWHEEL:
			MOUSEWHEEL_X = (int16_t)EVENT.wheel.x;
			MOUSEWHEEL_Y = (int16_t)EVENT.wheel.y;
			break;

		case SDL_KEYDOWN: {
			const auto keysym = EVENT.key.keysym;

			KEYBOARD_ANY = 1;
			KEYBOARD_PRESSED_ANY = 1;

			if (keysym.scancode < 256) KEYSYM[keysym.scancode] = true;
			//KEYSYM.push_back(keysym.sym);

			if (keysym.sym == SDLK_LALT)
			{
				KEYBOARD_ALT = 1;
				KEYBOARD_PRESSED_ALT = 1;
			}
			else
			if (keysym.sym == SDLK_LCTRL)
			{
				KEYBOARD_CTRL = 1;
				KEYBOARD_PRESSED_CTRL = 1;
			}
			else
			if (keysym.sym == SDLK_LSHIFT)
			{
				KEYBOARD_SHIFT = 1;
				KEYBOARD_PRESSED_SHIFT = 1;
			}
			else
			if (keysym.sym == SDLK_SPACE)
			{
				KEYBOARD_SPACE = 1;
				KEYBOARD_PRESSED_SPACE = 1;
			}
			else
			if (keysym.sym == SDLK_ESCAPE)
			{
				KEYBOARD_ESC = 1;
				KEYBOARD_PRESSED_ESC = 1;
			}
			else
			if (keysym.sym == SDLK_BACKSPACE)
			{
				if (KEY_TEXT.size() && !KEY_TEXT.empty())
				{
					KEY_TEXT.pop_back();
					KEY_TEXT_UPDATE();
				}
			}

			// Undo, Redo, Paste and Save
			if (keysym.mod & KMOD_CTRL) {
				switch (keysym.sym) {
				case SDLK_z: {
					if (keysym.mod & KMOD_SHIFT) {
						// because it's the superior 'redo' shortcut :)
						//redo();
						FUNCTION_REDO = true;
					}
					else {
						//undo();
						FUNCTION_UNDO = true;
					}
					break;
				}
				case SDLK_y: FUNCTION_REDO = true;  break;

				case SDLK_v:
				{
					if (keysym.mod & KMOD_CTRL) {
						std::string file = "clipboard";
						clipboard_to_file(file);
						open_file(file);
					}
					else {
						// paste as selection in current file
					}
					break;
				}
				case SDLK_s: {
					SDL_Surface* _tsurf = SDL_CreateRGBSurfaceWithFormat(0, CURRENT_FILE_PTR_PIXELS->canvas_w, CURRENT_FILE_PTR_PIXELS->canvas_h, 32, SDL_PIXELFORMAT_RGBA32);

					auto layer = CURRENT_FILE_PTR_PIXELS->current_layer_ptr;// CURRENT_FRAME_PTR->layers[0];
					for (int i = 0; i < CURRENT_FILE_PTR_PIXELS->canvas_w * CURRENT_FILE_PTR_PIXELS->canvas_h; i++)
					{
						((COLOR*)_tsurf->pixels)[i] = layer->pixels[i]; // THERE ISN'T MULTI-LAYER BLENDING YET
					}
					std::string _tpath = CURRENT_FILE_PTR_PIXELS->filename;// (CURRENT_PATH + CURRENT_FILE);
					IMG_SavePNG(_tsurf, _tpath.c_str());
					SDL_FreeSurface(_tsurf);
					break;
				}
				default: break;
				}
			}
		} break;

		case SDL_KEYUP: {
			const auto keysym = EVENT.key.keysym;

			KEYBOARD_ANY = 0;
			KEYBOARD_PRESSED_ANY = 0;

			if (keysym.scancode < 256) KEYSYM[keysym.scancode] = false;

			if (keysym.sym == SDLK_LALT)
			{
				KEYBOARD_ALT = 0;
				KEYBOARD_PRESSED_ALT = 0;
			}
			else
			if (keysym.sym == SDLK_LCTRL)
			{
				KEYBOARD_CTRL = 0;
				KEYBOARD_PRESSED_CTRL = 0;
			}
			else
			if (keysym.sym == SDLK_LSHIFT)
			{
				KEYBOARD_SHIFT = 0;
				KEYBOARD_PRESSED_SHIFT = 0;
			}
			else
			if (keysym.sym == SDLK_SPACE)
			{
				KEYBOARD_SPACE = 0;
				KEYBOARD_PRESSED_SPACE = 0;
			}
			else
			if (keysym.sym == SDLK_ESCAPE)
			{
				KEYBOARD_ESC = 0;
				KEYBOARD_PRESSED_ESC = 0;
			}
			break;
		}
		}
	}

	/*if (MOUSEBUTTON_PRESSED_RIGHT)
	{
	//	add_file_pixels(FILE_TYPE::PIXELS, "", "untitled.png", NEW_FILE_PIXELS_CANVAS_W, NEW_FILE_PIXELS_CANVAS_H);
		//SDL_UpdateTexture(CURRENT_FILE_PTR_PIXELS->current_layer_ptr->texture, NULL, &CURRENT_FILE_PTR_PIXELS->current_layer_ptr->pixels[0], CURRENT_FILE_PTR_PIXELS->canvas_pitch);

		std::string file = "clipboard";

		clipboard_to_file(file);
	
		open_file(file);
		PRINT("OPEN EXTERNAL FILE");
	}*/
}