
#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

#include "EVENT_LOOP.h"
#include "UIBOX_LOOP.h"

#include <chrono>

int main(int, char* [])
{
	if (!INIT())
	{
		PRINT("COULD NOT START EDGITOR");
		while (1)
		{
			// hold to show error message
		}
	}

	int t = 1;

	// WINDOW_INIT();
	// RENDERER_INIT();
	UIBOX_INIT();

	PRINT("EDGITOR :: VERSION: " + EDGITOR_VERSION);

	//std::cout << (226 | (1 << 30)) << std::endl << std::endl; // 1073742050
	//std::cout << (1073742050 >> 30) << std::endl << std::endl; // 1073742050

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	while (!QUIT)
	{
		auto from = std::chrono::high_resolution_clock::now();
		SDL_GetWindowPosition(WINDOW_MAIN, &WINDOW_MAIN_X, &WINDOW_MAIN_Y);
		SDL_GetWindowSize(WINDOW_MAIN, &WINDOW_MAIN_W, &WINDOW_MAIN_H);
		SDL_SetRenderTarget(RENDERER_MAIN, nullptr);
		SDL_SetRenderDrawColor(RENDERER_MAIN, 0, 0, 0, 255);
		SDL_RenderClear(RENDERER_MAIN);

		KEYSYM.reserve(256);
		for (int i = 0; i < 256; i++)
		{
			KEYSYM.push_back(false);
		}

		EVENT_LOOP();
		/*INPUT_LOOP();
		UPDATE_LOOP();
		RENDER_LOOP();*/

		switch (CURRENT_FILE_PTR_TYPE)
		{
		case EMPTY:
			break;
		case PIXELS:
			CURRENT_FILE_PTR_PIXELS->INPUT_LOOP();
			CURRENT_FILE_PTR_PIXELS->UPDATE_LOOP();
			CURRENT_FILE_PTR_PIXELS->RENDER_LOOP();
			break;
		case MAP:
			break;
		case TEXT:
			break;
		default:
			break;
		}

		UIBOX_LOOP();

		SDL_SetRenderDrawColor(RENDERER_MAIN, 255, 255, 255, 255);
		static const auto blend_invert = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
		SDL_SetRenderDrawBlendMode(RENDERER_MAIN, blend_invert);
		//SDL_SetRenderDrawBlendMode(RENDERER_MAIN, SDL_BLENDMODE_ADD);

		SDL_RenderDrawLine(RENDERER_MAIN, MOUSE_X - 3, MOUSE_Y - 4, MOUSE_X + 4, MOUSE_Y + 3);
		SDL_RenderDrawLine(RENDERER_MAIN, MOUSE_X - 4, MOUSE_Y - 4, MOUSE_X + 4, MOUSE_Y + 4);
		SDL_RenderDrawLine(RENDERER_MAIN, MOUSE_X - 4, MOUSE_Y - 3, MOUSE_X + 3, MOUSE_Y + 4);
		SDL_RenderDrawLine(RENDERER_MAIN, MOUSE_X + 4, MOUSE_Y - 3, MOUSE_X - 3, MOUSE_Y + 4);
		SDL_RenderDrawLine(RENDERER_MAIN, MOUSE_X + 4, MOUSE_Y - 4, MOUSE_X - 4, MOUSE_Y + 4);
		SDL_RenderDrawLine(RENDERER_MAIN, MOUSE_X + 3, MOUSE_Y - 4, MOUSE_X - 4, MOUSE_Y + 3);



		/*SDL_SetRenderDrawColor(RENDERER_MAIN, 255, 255, 255, 255);
		static auto blend_mask = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
		SDL_SetRenderDrawBlendMode(RENDERER_MAIN, blend_mask);
		
		SDL_Rect _trect = { 0,0,WINDOW_MAIN_W,WINDOW_MAIN_H };
		SDL_RenderFillRect(RENDERER_MAIN, &_trect);

		static auto blend_mask2 = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
		SDL_SetRenderDrawBlendMode(RENDERER_MAIN, blend_mask2);

		_trect = { 400,500,700,600 };
		SDL_RenderFillRect(RENDERER_MAIN, &_trect);

		static auto blend_mask3 = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
		SDL_SetRenderDrawBlendMode(RENDERER_MAIN, blend_mask3);

		SDL_SetRenderDrawColor(RENDERER_MAIN, 255, 0, 64, 255);
		SDL_RenderDrawLine(RENDERER_MAIN, 0, MOUSE_Y, WINDOW_MAIN_W, MOUSE_Y);*/

		/*SDL_SetRenderDrawColor(RENDERER_MAIN, 255, 255, 255, 255);

		_trect = {400,500,700,600};
		SDL_RenderFillRect(RENDERER_MAIN, &_trect);

		SDL_SetRenderDrawColor(RENDERER_MAIN, 0, 0, 0, 0);

		_trect = { 404,504,700 - 8,600 -8 };
		SDL_RenderFillRect(RENDERER_MAIN, &_trect);*/

		/*static auto blend_mask2 = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
		SDL_SetRenderDrawBlendMode(RENDERER_MAIN, blend_mask2);
		SDL_SetRenderDrawColor(RENDERER_MAIN, 255, 0, 64, 255);

		SDL_RenderDrawLine(RENDERER_MAIN, 0, MOUSE_Y, WINDOW_MAIN_W, MOUSE_Y);*/

		SDL_SetRenderDrawBlendMode(RENDERER_MAIN, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(RENDERER_MAIN, 255, 255, 255, 255);



		SDL_RenderPresent(RENDERER_MAIN);
		auto to = std::chrono::high_resolution_clock::now();
		std::cout << (double)(std::chrono::duration_cast<std::chrono::duration<double>>(to - from).count() * 1000.0) << "ms    " << '\r';
	}

	SDL_DestroyRenderer(RENDERER_MAIN);
	SDL_DestroyWindow(WINDOW_MAIN);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}