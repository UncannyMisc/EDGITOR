
#include "GLOBAL_VARIABLES.h"

bool QUIT = false;
std::string EDGITOR_VERSION = "0.1";

SDL_Window* WINDOW_MAIN;
SDL_Renderer* RENDERER_MAIN;

char PATH_SEP = static_cast<char>(std::filesystem::path::preferred_separator);

std::filesystem::path CURRENT_PATH = std::filesystem::current_path();
std::vector<PATH_FILES_INFO> CURRENT_PATH_FILES;

//std::filesystem::

std::string KEY_TEXT = "";
int KEY_TEXT_INT = 0;

int UIBOX_ELEMENT_TYPE = -1;
int8_t UIBOX_IN = -1;

int* UIBOX_INPUT_PTR_I = nullptr;
int UIBOX_INPUT_VALUE_I = 0;
uint8_t* UIBOX_INPUT_PTR_U8 = nullptr;
uint8_t UIBOX_INPUT_VALUE_U8 = 0;
uint16_t* UIBOX_INPUT_PTR_U16 = nullptr;
uint16_t UIBOX_INPUT_VALUE_U16 = 0;

FILE_TYPE NEW_FILE_TYPE = FILE_TYPE::EMPTY;
uint16_t NEW_FILE_PIXELS_CANVAS_W = 0;
uint16_t NEW_FILE_PIXELS_CANVAS_H = 0;
int PIXEL_BIT_DEPTH = 4; // 1 = 1-bit, 2 = 2-bit, 3 = 4-bit, 4 = 8-bit

SDL_Texture* FONTMAP[256];
SDL_Texture* FONTMAP2;
TTF_Font* FONT;
uint16_t FONT_CHRW;
uint16_t FONT_CHRH;

int WINDOW_MAIN_X = 0;
int WINDOW_MAIN_Y = 0;
int WINDOW_MAIN_W = 0;
int WINDOW_MAIN_H = 0;

int16_t MOUSE_X = 0;
int16_t MOUSE_Y = 0;
int16_t MOUSE_PREVX = 0;
int16_t MOUSE_PREVY = 0;

bool MOUSEBUTTON_LEFT = false;
bool MOUSEBUTTON_PRESSED_LEFT = false;
bool MOUSEBUTTON_MIDDLE = false;
bool MOUSEBUTTON_PRESSED_MIDDLE = false;
bool MOUSEBUTTON_RIGHT = false;
bool MOUSEBUTTON_PRESSED_RIGHT = false;
int16_t MOUSEWHEEL_X = 0;
int16_t MOUSEWHEEL_Y = 0;

bool KEYBOARD_CTRL = false;
bool KEYBOARD_PRESSED_CTRL = false;
bool KEYBOARD_SHIFT = false;
bool KEYBOARD_PRESSED_SHIFT = false;
bool KEYBOARD_ALT = false;
bool KEYBOARD_PRESSED_ALT = false;
bool KEYBOARD_SPACE = false;
bool KEYBOARD_PRESSED_SPACE = false;
bool KEYBOARD_ENTER = false;
bool KEYBOARD_PRESSED_ENTER = false;
bool KEYBOARD_ESC = false;
bool KEYBOARD_PRESSED_ESC = false;

bool MOUSEBUTTON_PREVLEFT = false;
bool MOUSEBUTTON_PREVMIDDLE = false;
bool MOUSEBUTTON_PREVRIGHT = false;
int16_t MOUSEWHEEL_PREVX = 0;
int16_t MOUSEWHEEL_PREVY = 0;
bool KEYBOARD_PREVCTRL = false;
bool KEYBOARD_PREVSHIFT = false;
bool KEYBOARD_PREVALT = false;
bool KEYBOARD_PREVSPACE = false;
bool KEYBOARD_PREVENTER = false;
bool KEYBOARD_PREVESC = false;

bool FUNCTION_UNDO = false;
bool FUNCTION_REDO = false;

int CURRENT_TOOL = 0;
int TEMP_CURRENT_TOOL = 0;