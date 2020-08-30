#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

struct UNDO_ENTRY_PIXELS
{
	QUAD affected_region;
	uint16_t affected_layer;
	uint16_t affected_frame;
	//plf::stack<std::pair<uint32_t, COLOR>> affected_pixels;

	plf::stack<std::pair<uint32_t, COLOR>> undo_pixels;
	plf::stack<std::pair<uint32_t, COLOR>> redo_pixels;

	UNDO_ENTRY_PIXELS(QUAD _rect, uint16_t _layer, uint16_t _frame)
		: affected_region{ _rect }
		, affected_layer{ _layer }
		, affected_frame{ _frame }
		//, undo_pixels(1)
		//, redo_pixels(1)
	{
	}

	/*void set(uint16_t xx, uint16_t yy, COLOR prev_col, COLOR new_col)
	{
		int index = xx + yy * this->affected_region.width();
		this->undo_pixels[index] = prev_col;
		this->redo_pixels[index] = new_col;
	}*/
};

struct UNDO_STACK_PIXELS {
	std::vector<UNDO_ENTRY_PIXELS> entries;
	uint16_t position = 0;
	//QUAD update_region = QUAD::empty();
};