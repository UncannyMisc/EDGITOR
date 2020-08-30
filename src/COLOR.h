#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

struct COLOR {
	uint8_t r, g, b, a;

	friend bool operator==(COLOR a, COLOR b) {
		return a.r == b.r && a.g == b.g
			&& a.b == b.b && a.a == b.a;
	}

	friend bool operator!=(COLOR a, COLOR b) {
		return !(a == b);
	}
};


// PRESET COLORS
extern COLOR COL_ACCENT;
extern COLOR COL_BGUPDATE;
extern COLOR COL_EMPTY;
extern COLOR COL_WHITE;
extern COLOR COL_LTGRAY;
extern COLOR COL_GRAY;
extern COLOR COL_DKGRAY;
extern COLOR COL_BLACK;

extern COLOR blend_colors(const COLOR src_color, const COLOR dst_color);