
#include "MAIN_LIBRARIES.h"

void PRINT(std::string print)
{
	std::cout << "          " << std::endl << print << std::endl;
}

void PRINT(const char* print)
{
	std::cout << "          " << std::endl << print << std::endl;
}

void PRINT(int print)
{
	std::cout << "          " << std::endl << print << std::endl;
}

void PRINT(double print)
{
	std::cout << "          " << std::endl << print << std::endl;
}

int clamp(int i, int lo, int hi)
{
	return (i <= lo ? lo : (i >= hi ? hi : i));
}

float clamp(float i, float lo, float hi)
{
	return (i <= lo ? lo : (i >= hi ? hi : i));
}

int16_t sign(const int16_t x) {
	return (x > 0) - (x < 0);
}

float sign(const float x) {
	return (x > 0) - (x < 0);
}

/*turns const char* to int while skipping letters*/
int ctoi(const char* s)
{
	// credit to nervetoucher on Twitch
	int ret = 0;
	for (; *s; s++)
	{
		if (isdigit(*s))
		{
			ret *= 10;
			ret += *s - '0';
		}
	}
	return ret;
}

bool point_in_rect(const uint16_t px, const uint16_t py, const uint16_t rx, const uint16_t ry, const uint16_t rw, const uint16_t rh)
{
	return (px >= rx && py >= ry && px < (rx + rw) && py < (ry + rh));
}

void confirm_input()
{
	UIBOX_ELEMENT_CLICK = nullptr;
	if (UIBOX_ELEMENT_TYPE >= 0)
	{
		switch (UIBOX_ELEMENT_TYPE)
		{
		case 0: // int?
			*UIBOX_INPUT_PTR_I = UIBOX_INPUT_VALUE_I;
			UIBOX_INPUT_PTR_I = nullptr;
			UIBOX_INPUT_VALUE_I = 0;
			break;

		case 1: // uint8_t
			*UIBOX_INPUT_PTR_U8 = UIBOX_INPUT_VALUE_U8;
			UIBOX_INPUT_PTR_U8 = nullptr;
			UIBOX_INPUT_VALUE_U8 = 0;
			break;

		case 2: // uint16_t
			*UIBOX_INPUT_PTR_U16 = UIBOX_INPUT_VALUE_U16;
			UIBOX_INPUT_PTR_U16 = nullptr;
			UIBOX_INPUT_VALUE_U16 = 0;
			break;

		default:
			break;
		}
	}
	UIBOX_ELEMENT_TYPE = -1;
}

//

void INIT_FONT() {
	
	FONT = TTF_OpenFont("resources/FONT.ttf", 16);
	if (FONT == NULL)
	{
		std::cout << std::endl << "COULDN'T FIND THE RESOURCES FOLDER" << std::endl << std::endl;
		SDL_SetWindowOpacity(WINDOW_MAIN, 0.0f);
		while (1)
		{
			//
		}
	}
	int _tfw, _tfh;
	TTF_SizeText(FONT, "A", &_tfw, &_tfh);
	FONT_CHRW = (uint16_t)_tfw;
	FONT_CHRH = (uint16_t)_tfh;

	const char* char_map[256] = {
		u8" ", u8"☺", u8"☻", u8"♥", u8"♦", u8"♣", u8"♠", u8"•", u8"◘", u8"○", u8"◙", u8"♂", u8"♀", u8"♪", u8"♫", u8"☼",
		u8"►", u8"◄", u8"↕", u8"‼", u8"¶", u8"§", u8"▬", u8"↨", u8"↑", u8"↓", u8"→", u8"←", u8"∟", u8"↔", u8"▲", u8"▼",
		u8" ", u8"!", u8"\"",u8"#", u8"$", u8"%", u8"&", u8"'", u8"(", u8")", u8"*", u8"+", u8",", u8"-", u8".", u8"/",
		u8"0", u8"1", u8"2", u8"3", u8"4", u8"5", u8"6", u8"7", u8"8", u8"9", u8":", u8";", u8"<", u8"=", u8">", u8"?",
		u8"@", u8"A", u8"B", u8"C", u8"D", u8"E", u8"F", u8"G", u8"H", u8"I", u8"J", u8"K", u8"L", u8"M", u8"N", u8"O",
		u8"P", u8"Q", u8"R", u8"S", u8"T", u8"U", u8"V", u8"W", u8"X", u8"Y", u8"Z", u8"[", u8"\\",u8"]", u8"^", u8"_",
		u8"`", u8"a", u8"b", u8"c", u8"d", u8"e", u8"f", u8"g", u8"h", u8"i", u8"j", u8"k", u8"l", u8"m", u8"n", u8"o",
		u8"p", u8"q", u8"r", u8"s", u8"t", u8"u", u8"v", u8"w", u8"x", u8"y", u8"z", u8"{", u8"|", u8"}", u8"~", u8" ",
		u8"Ç", u8"ü", u8"é", u8"â", u8"ä", u8"à", u8"å", u8"ç", u8"ê", u8"ë", u8"è", u8"ï", u8"î", u8"ì", u8"Ä", u8"Å",
		u8"É", u8"æ", u8"Æ", u8"ô", u8"ö", u8"ò", u8"û", u8"ù", u8"ÿ", u8"Ö", u8"Ü", u8"¢", u8"£", u8"¥", u8"₧", u8"ƒ",
		u8"á", u8"í", u8"ó", u8"ú", u8"ñ", u8"Ñ", u8"ª", u8"º", u8"¿", u8"⌐", u8"¬", u8"½", u8"¼", u8"¡", u8"«", u8"»",
		u8"░", u8"▒", u8"▓", u8"│", u8"┤", u8"╡", u8"╢", u8"╖", u8"╕", u8"╣", u8"║", u8"╗", u8"╝", u8"╜", u8"╛", u8"┐",
		u8"└", u8"┴", u8"┬", u8"├", u8"─", u8"┼", u8"╞", u8"╟", u8"╚", u8"╔", u8"╩", u8"╦", u8"╠", u8"═", u8"╬", u8"╧",
		u8"╨", u8"╤", u8"╥", u8"╙", u8"╘", u8"╒", u8"╓", u8"╫", u8"╪", u8"┘", u8"┌", u8"█", u8"▄", u8"▌", u8"▐", u8"▀",
		u8"α", u8"ß", u8"Γ", u8"π", u8"Σ", u8"σ", u8"µ", u8"τ", u8"Φ", u8"Θ", u8"Ω", u8"δ", u8"∞", u8"φ", u8"ε", u8"∩",
		u8"≡", u8"±", u8"≥", u8"≤", u8"⌠", u8"⌡", u8"÷", u8"≈", u8"°", u8"∙", u8"·", u8"√", u8"ⁿ", u8"²", u8"■", u8" " };


	SDL_Surface* sur;

	for (int i = 0; i < 256; i++)
	{
		sur = TTF_RenderUTF8_Solid(FONT, char_map[i], SDL_Color{ 255, 255, 255, 255 });
		FONTMAP[i] = SDL_CreateTextureFromSurface(RENDERER_MAIN, sur);
		SDL_SetTextureBlendMode(FONTMAP[i], SDL_BLENDMODE_BLEND);
		SDL_FreeSurface(sur);
	}
}

bool INIT() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		PRINT("SDL COULD NOT INITIALISE");
		PRINT(SDL_GetError());
		return 0;
	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) PRINT("WARNING: SDL_HINT_RENDER_SCALE_QUALITY NOT SET");
		if (!SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1")) PRINT("WARNING: SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING NOT SET");
		if (!SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "opengl")) PRINT("WARNING: SDL_HINT_FRAMEBUFFER_ACCELERATION NOT SET");
		if (!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl")) PRINT("WARNING: SDL_HINT_RENDER_DRIVER NOT SET");
		if (!SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, "1")) PRINT("WARNING: SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT NOT SET");
		if (!SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "1")) PRINT("WARNING: SDL_HINT_VIDEO_DOUBLE_BUFFER NOT SET");
		if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) PRINT("WARNING: SDL_HINT_RENDER_VSYNC NOT SET");
		//if (!SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1")) PRINT("WARNING: SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH NOT SET");

		//Create window
		WINDOW_MAIN = SDL_CreateWindow("EDGITOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_MAIN_W, WINDOW_MAIN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

		SDL_GetWindowSize(WINDOW_MAIN, &WINDOW_MAIN_W, &WINDOW_MAIN_H);
		SDL_SetWindowMinimumSize(WINDOW_MAIN, 1280, 720 - 32);

		if (WINDOW_MAIN == NULL) {
			PRINT("WINDOW NOT CREATED");
			PRINT(SDL_GetError());
			return 0;
		}
		else {
			//Creates vsynced renderer for window
			RENDERER_MAIN = SDL_CreateRenderer(WINDOW_MAIN, -1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (RENDERER_MAIN == NULL) {
				PRINT("RENDERER NOT CREATED");
				PRINT(SDL_GetError());
				return 0;
			}
			else {
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					PRINT("SDL_IMAGE COULD NOT INITIALISE");
					PRINT(SDL_GetError());
					return 0;
				}
				else {
					if (TTF_Init() < 0) {
						PRINT("SDL_TTF COULD NOT INITIALISE");
						PRINT(SDL_GetError());
						return 0;
					}
					else {
						INIT_FONT();
					}
				}
			}
		}
	}

	return 1;
}