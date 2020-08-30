
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

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	while (!QUIT)
	{
		auto from = std::chrono::high_resolution_clock::now();
		SDL_GetWindowPosition(WINDOW_MAIN, &WINDOW_MAIN_X, &WINDOW_MAIN_Y);
		SDL_GetWindowSize(WINDOW_MAIN, &WINDOW_MAIN_W, &WINDOW_MAIN_H);
		SDL_SetRenderTarget(RENDERER_MAIN, nullptr);
		SDL_SetRenderDrawColor(RENDERER_MAIN, 0, 0, 0, 255);
		SDL_RenderClear(RENDERER_MAIN);

		//if (CURRENT_FILE_PTR != nullptr) CURRENT_FILE_PTR->brush_update = 0;

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