﻿#pragma once

#include "MAIN_LIBRARIES.h"

extern bool QUIT;
extern std::string EDGITOR_VERSION;

extern SDL_Window* WINDOW_MAIN;
extern SDL_Renderer* RENDERER_MAIN;

struct vec2 {
	uint8_t x, y;

	friend bool operator==(vec2 a, vec2 b) {
		return a.x == b.x && a.y == b.y;
	}

	friend bool operator!=(vec2 a, vec2 b) {
		return !(a == b);
	}
};

struct vec3 {
	uint8_t x, y, z;

	friend bool operator==(vec3 a, vec3 b) {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	friend bool operator!=(vec3 a, vec3 b) {
		return !(a == b);
	}
};

struct vec4 {
	uint8_t x, y, z, w;

	friend bool operator==(vec4 a, vec4 b) {
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	friend bool operator!=(vec4 a, vec4 b) {
		return !(a == b);
	}
};

enum FILE_TYPE {
	EMPTY,
	ALL,
	PIXELS,
	MAP,
	TEXT,

	// New file types should always be added above UNSUPPORTED and below the Previously last FILE_TYPE

	UNSUPPORTED,
};

extern char PATH_SEP;

struct PATH_FILES_INFO {
	std::filesystem::path path;
	bool is_directory = false;
	std::string ext = "";
	std::string filename = "";
};

extern std::filesystem::path CURRENT_PATH;
extern std::vector<PATH_FILES_INFO> CURRENT_PATH_FILES;

//extern char KEY_TEXT[256];
extern std::string KEY_TEXT;
extern int KEY_TEXT_INT;

extern int UIBOX_ELEMENT_TYPE;
extern int8_t UIBOX_IN;

extern int* UIBOX_INPUT_PTR_I;
extern int UIBOX_INPUT_VALUE_I;
extern uint8_t* UIBOX_INPUT_PTR_U8;
extern uint8_t UIBOX_INPUT_VALUE_U8;
extern uint16_t* UIBOX_INPUT_PTR_U16;
extern uint16_t UIBOX_INPUT_VALUE_U16;

extern FILE_TYPE NEW_FILE_TYPE;
extern uint16_t NEW_FILE_PIXELS_CANVAS_W;
extern uint16_t NEW_FILE_PIXELS_CANVAS_H;
extern int PIXEL_BIT_DEPTH;

extern SDL_Texture* FONTMAP[256];
extern SDL_Texture* FONTMAP2;
extern TTF_Font* FONT;
extern uint16_t FONT_CHRW;
extern uint16_t FONT_CHRH;

extern int WINDOW_MAIN_X;
extern int WINDOW_MAIN_Y;
extern int WINDOW_MAIN_W;
extern int WINDOW_MAIN_H;

extern int16_t MOUSE_X;
extern int16_t MOUSE_Y;
extern int16_t MOUSE_PREVX;
extern int16_t MOUSE_PREVY;

extern bool MOUSEBUTTON_LEFT;
extern bool MOUSEBUTTON_PRESSED_LEFT;
extern bool MOUSEBUTTON_PREVLEFT;
extern bool MOUSEBUTTON_MIDDLE;
extern bool MOUSEBUTTON_PRESSED_MIDDLE;
extern bool MOUSEBUTTON_PREVMIDDLE;
extern bool MOUSEBUTTON_RIGHT;
extern bool MOUSEBUTTON_PRESSED_RIGHT;
extern bool MOUSEBUTTON_PREVRIGHT;
extern int16_t MOUSEWHEEL_X;
extern int16_t MOUSEWHEEL_PREVX;
extern int16_t MOUSEWHEEL_Y;
extern int16_t MOUSEWHEEL_PREVY;

extern bool KEYBOARD_CTRL;
extern bool KEYBOARD_PRESSED_CTRL;
extern bool KEYBOARD_PREVCTRL;
extern bool KEYBOARD_SHIFT;
extern bool KEYBOARD_PRESSED_SHIFT;
extern bool KEYBOARD_PREVSHIFT;
extern bool KEYBOARD_ALT;
extern bool KEYBOARD_PRESSED_ALT;
extern bool KEYBOARD_PREVALT;
extern bool KEYBOARD_SPACE;
extern bool KEYBOARD_PRESSED_SPACE;
extern bool KEYBOARD_PREVSPACE;
extern bool KEYBOARD_ENTER;
extern bool KEYBOARD_PRESSED_ENTER;
extern bool KEYBOARD_PREVENTER;
extern bool KEYBOARD_ESC;
extern bool KEYBOARD_PRESSED_ESC;
extern bool KEYBOARD_PREVESC;

extern bool KEYBOARD_ANY;
extern bool KEYBOARD_PRESSED_ANY;
extern bool KEYBOARD_PREVANY;

extern std::vector<bool> KEYSYM;

//todo, this is unused, there is also a version implimented for this in the pixels filetype
extern bool FUNCTION_UNDO;
extern bool FUNCTION_REDO;

extern int TEMP_CURRENT_TOOL;

//note, the open file variables are in FILES.h

const uint8_t CHAR_NBSP = 0xffu; // no-break space
#define STR_NBSP "\xff"

const uint8_t CHAR_BLOCK = 0xdb; // █
#define STR_BLOCK "\xdb"

const uint8_t CHAR_BOXTL = 0xc9;// u8"╔";
#define STR_BOXTL "\xc9"
const uint8_t CHAR_BOXTR = 0xbb;// u8"╗";
#define STR_BOXTR "\xbb"
const uint8_t CHAR_BOXBL = 0xc8;//u8"╚";
#define STR_BOXBL "\xc8"
const uint8_t CHAR_BOXBR = 0xbc;//u8"╝";
#define STR_BOXBR "\xbc"
const uint8_t CHAR_BOXH = 0xcd;//u8"═";
#define STR_BOXH "\xcd"
const uint8_t CHAR_BOXV = 0xba;//u8"║";
#define STR_BOXV "\xba"

const uint8_t CHAR_LINETL = 0xda;// ┌
#define STR_LINETL "\xda"
const uint8_t CHAR_LINETR = 0xbf; // ┐
#define STR_LINETR "\xbf"
const uint8_t CHAR_LINEBL = 0xc0;// └
#define STR_LINEBL "\xc0"
const uint8_t CHAR_LINEBR = 0xd9;// ┘
#define STR_LINEBR "\xd9"
const uint8_t CHAR_LINEH = 0xc4;// ─
#define STR_LINEH "\xc4"
const uint8_t CHAR_LINEV = 0xb3;// │
#define STR_LINEV "\xb3"

const uint8_t CHAR_LINEVL = 0xb4;// ┤
#define STR_LINEVL "\xb4"
const uint8_t CHAR_LINEVR = 0xc3;// ├
#define STR_LINEVR "\xc3"
const uint8_t CHAR_LINEHU = 0xc1;// ┴
#define STR_LINEHL "\xc1"
const uint8_t CHAR_LINEHD = 0xc2;// ┬
#define STR_LINEHR "\xc2"

const uint8_t CHAR_LINEVL2 = 0xb5;// ╡
#define STR_LINEVL2 "\xb5"
const uint8_t CHAR_LINEVR2 = 0xc6;// ╞
#define STR_LINEVR2 "\xc6"

const uint8_t CHAR_ARWU = 0x18;
#define STR_ARWU "\x18"
const uint8_t CHAR_ARWD = 0x19;
#define STR_ARWD "\x19"
const uint8_t CHAR_ARWL = 0x1b;
#define STR_ARWL "\x1b"
const uint8_t CHAR_ARWR = 0x1a;
#define STR_ARWR "\x1a"

/*

	*	_0	_1	_2	_3	_4	_5	_6	_7	_8	_9	_a	_b	_c	_d	_e	_f
	0		☺	☻	♥	♦	♣	♠	•	◘	○	◙	♂	♀	♪	♫	☼
	1	►	◄	↕	‼	¶	§	▬	↨	↑	↓	→	←	∟	↔	▲	▼
	2	sp	!	"	#	$	%	&	'	(	)	*	+	,	-	.	/
	3	0	1	2	3	4	5	6	7	8	9	:	;	<	=	>	?
	4	@	A	B	C	D	E	F	G	H	I	J	K	L	M	N	O
	5	P	Q	R	S	T	U	V	W	X	Y	Z	[	\	]	^	_
	6	`	a	b	c	d	e	f	g	h	i	j	k	l	m	n	o
	7	p	q	r	s	t	u	v	w	x	y	z	{	|	}	~	del
	8	Ç	ü	é	â	ä	à	å	ç	ê	ë	è	ï	î	ì	Ä	Å
	9	É	æ	Æ	ô	ö	ò	û	ù	ÿ	Ö	Ü	¢	£	¥	₧	ƒ
	a	á	í	ó	ú	ñ	Ñ	ª	º	¿	⌐	¬	½	¼	¡	«	»
	b	░	▒	▓	│	┤	╡	╢	╖	╕	╣	║	╗	╝	╜	╛	┐
	c	└	┴	┬	├	─	┼	╞	╟	╚	╔	╩	╦	╠	═	╬	╧
	d	╨	╤	╥	╙	╘	╒	╓	╫	╪	┘	┌	█	▄	▌	▐	▀
	e	α	ß	Γ	π	Σ	σ	µ	τ	Φ	Θ	Ω	δ	∞	φ	ε	∩
	f	≡	±	≥	≤	⌠	⌡	÷	≈	°	∙	·	√	ⁿ	²	■	nbsp

	*/