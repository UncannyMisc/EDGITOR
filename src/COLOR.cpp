
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
