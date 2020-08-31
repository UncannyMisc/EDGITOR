
#include "COLOR.h"

COLOR COL_ACCENT = { 0xff, 0x00, 0x40, 0xff };
COLOR COL_BGUPDATE = { 0x00, 0x00, 0x00, 0x01 };
COLOR COL_EMPTY = { 0x00, 0x00, 0x00, 0x00 };
COLOR COL_WHITE = { 0xff, 0xff, 0xff, 0xff };
COLOR COL_LTGRAY = { 0xc0, 0xc0, 0xc0, 0xff };
COLOR COL_GRAY = { 0x80, 0x80, 0x80, 0xff };
COLOR COL_DKGRAY = { 0x40, 0x40, 0x40, 0xff };
COLOR COL_BLACK = { 0x00, 0x00, 0x00, 0xff };

COLOR blend_colors(const COLOR src_color, const COLOR dst_color) {
	const float src_cola = (float)src_color.a / 255.0f;
	const float dest_cola = (float)dst_color.a / 255.0f * (1.0f - src_cola);
	const float new_cola = (src_cola + dest_cola);

	return COLOR{
		uint8_t(((float)src_color.r / 255.0f * src_cola + (float)dst_color.r / 255.0f * dest_cola) / new_cola * 255.0f),
		uint8_t(((float)src_color.g / 255.0f * src_cola + (float)dst_color.g / 255.0f * dest_cola) / new_cola * 255.0f),
		uint8_t(((float)src_color.b / 255.0f * src_cola + (float)dst_color.b / 255.0f * dest_cola) / new_cola * 255.0f),
		uint8_t(new_cola * 255.0f),
	};
}

COLOR hsv_to_rgb(float h, float s, float v)
{
	float r = 0.0f, g = 0.0f, b = 0.0f;

	float i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	switch (((int)i) % 6) {
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}

	return COLOR{ (uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), 255 };
}
