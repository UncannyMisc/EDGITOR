#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

#include "UNDO_STACK.h"
#include "plf_stack.h"

struct LAYER_INFO_PIXELS {
	SDL_Texture* texture = nullptr;
	std::unique_ptr<COLOR[]> pixels;
	int16_t x = 0;
	int16_t y = 0;
	uint8_t alpha = 255;
	int blendmode = SDL_BLENDMODE_BLEND; /* SDL_BlendMode */

	~LAYER_INFO_PIXELS() {
		if (!texture) SDL_DestroyTexture(texture);
	}
};

struct FRAME_INFO_PIXELS {
	int time = 1;
	std::vector<std::shared_ptr<LAYER_INFO_PIXELS>> layers;
};

struct FILE_INFO {
	FILE_TYPE type = FILE_TYPE::EMPTY;
	std::filesystem::path path = "";
	std::string filename = "";
	uint16_t undo_pos = 0;
	bool modified = false;

	bool update = true;

	virtual void INPUT_LOOP() {};
	virtual void UPDATE_LOOP() {};
	virtual void RENDER_LOOP() {};

	virtual void apply_undo_data(UNDO_ENTRY_PIXELS* undo_entry, bool is_undo) {};

	template <typename T, typename TT>
	void push_undo_entry(T& u, TT& undo_entry)
	{
		// if we're back a few steps in the undo reel, we clear all the above undo steps.
		if (u.position > 0) {
			auto const old_end = u.entries.end();
			auto const new_end = std::prev(old_end, u.position);
			u.entries.erase(new_end, old_end);
		}

		// add the new undo
		u.entries.push_back(std::move(undo_entry));
		u.position = 0;
	}

	template <typename T>
	void clear_undo_stack(T& u)
	{
		u.entries.clear();
		u.position = 0;
	}

	template <typename T> // e.g. UNDO_STACK_PIXELS
	void undo(T& u)
	{
		if (u.position >= u.entries.size()) {
			return;
		}
		auto const undo_entry = &u.entries[u.entries.size() - u.position - 1];
		apply_undo_data(undo_entry, true);
		u.position++;
	}

	template <typename T>
	void redo(T& u)
	{
		if (u.position == 0) {
			return;
		}
		u.position--;
		auto const undo_entry = &u.entries[u.entries.size() - u.position - 1];
		apply_undo_data(undo_entry, false);
	}
};

struct FILE_INFO_PIXELS : public FILE_INFO {

	UNDO_STACK_PIXELS undo_stack;

	std::vector<std::shared_ptr<FRAME_INFO_PIXELS>> frames;
	float canvas_x = 0;
	float canvas_y = 0;
	float canvas_pan_x = 0;
	float canvas_pan_y = 0;
	uint16_t canvas_w = 0;
	uint16_t canvas_h = 0;
	float canvas_zoom = 1;
	int16_t canvas_mouse_x = 0;
	int16_t canvas_mouse_y = 0;
	int16_t canvas_mouse_prevx = 0;
	int16_t canvas_mouse_prevy = 0;
	int16_t canvas_mouse_cellx = 0;
	int16_t canvas_mouse_celly = 0;
	uint32_t canvas_pitch = 0;
	bool canvas_clicked = 0;
	uint16_t current_layer = 0;
	std::shared_ptr<LAYER_INFO_PIXELS> current_layer_ptr = 0;
	uint16_t current_frame = 0;
	std::shared_ptr<FRAME_INFO_PIXELS> current_frame_ptr = 0;
	std::vector<COLOR> brush_color;
	uint16_t current_brush_color;
	bool view_tiled_x = false;
	bool view_tiled_y = false;
	uint16_t cell_w = 16;
	uint16_t cell_h = 16;

	bool can_draw_tick = false;

	bool brush_update = false;
	SDL_Texture* brush_texture = nullptr;
	QUAD brush_update_region = QUAD::empty();
	std::unique_ptr<COLOR[]> brush_pixels;

	uint8_t layer_update = 0;
	QUAD layer_update_region = QUAD::empty();

	std::vector<bool> brush_update_bool;
	std::vector<COLOR> brush_update_color;
	plf::stack<std::pair<uint32_t, COLOR>> brush_update_stack;

	SDL_Texture* BG_TEXTURE;

	inline void add_layer_pixels(std::shared_ptr<FRAME_INFO_PIXELS> frame, int16_t _x, int16_t _y, int16_t _a, /*SDL_BlendMode*/ int _b)
	{
		std::shared_ptr<LAYER_INFO_PIXELS> new_layer = std::make_shared<LAYER_INFO_PIXELS>();
		new_layer->texture = SDL_CreateTexture(RENDERER_MAIN, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, canvas_w, canvas_h);
		new_layer->pixels = std::make_unique<COLOR[]>(canvas_w * canvas_h);
		//new_layer->pixels = std::make_shared<std::vector<COLOR>>(canvas_w * canvas_h);

		/*for (int i = 0; i < canvas_w * canvas_h; i++)
		{
			_p[i] = COL_ACCENT;
		}*/
		
		new_layer->x = _x;
		new_layer->y = _y;
		new_layer->alpha = _a;
		new_layer->blendmode = _b;

		update = 1;
		current_layer = frame->layers.size();
		current_layer_ptr = new_layer;
		can_draw_tick = 0;

		frame->layers.push_back(std::move(new_layer));
	}

	inline void add_frame_pixels(int frame_time)
	{
		std::shared_ptr<FRAME_INFO_PIXELS> new_frame = std::make_shared<FRAME_INFO_PIXELS>();
		add_layer_pixels(new_frame, 0, 0, 255, SDL_BLENDMODE_BLEND);

		new_frame->time = frame_time;

		update = 1;
		current_frame = frames.size();
		current_frame_ptr = new_frame;
		can_draw_tick = 0;

		frames.push_back(std::move(new_frame));
	}

	inline void canvas_center()
	{
		// center canvas
		canvas_x = ((float)WINDOW_MAIN_W * 0.5f) - ((float)canvas_w * 0.5f * canvas_zoom);
		canvas_y = ((float)WINDOW_MAIN_H * 0.5f) - ((float)canvas_h * 0.5f * canvas_zoom);
	}

	void refresh_canvas()
	{
		canvas_pitch = (sizeof(COLOR) * (uint32_t)canvas_w);

		brush_update_color.clear();
		brush_update_bool.clear();

		brush_update_color = std::vector<COLOR>(canvas_w * canvas_h);
		brush_update_bool = std::vector<bool>(canvas_w * canvas_h);

		SDL_DestroyTexture(BG_TEXTURE);
		
		// BACKGROUND GRID TEXTURE
		int BG_GRID_W = ((int16_t)ceil((double)canvas_w / (double)cell_w));
		int BG_GRID_H = ((int16_t)ceil((double)canvas_h / (double)cell_h));
		auto BG_GRID_PIXELS = std::make_unique<COLOR[]>(BG_GRID_W * BG_GRID_H);
		BG_TEXTURE = SDL_CreateTexture(RENDERER_MAIN, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, BG_GRID_W, BG_GRID_H);
		for (int i = 0; i < BG_GRID_H; i++)
		{
			for (int j = 0; j < BG_GRID_W; j++)
			{
				const COLOR cell_colors[]{
					COLOR {0x1f, 0x1f, 0x1f, 0xff},
					COLOR {0x1a, 0x1a, 0x1a, 0xff},
				};

				BG_GRID_PIXELS[i * BG_GRID_W + j] = cell_colors[(i + j) % 2];
			}
		}
		SDL_SetTextureBlendMode(BG_TEXTURE, SDL_BLENDMODE_NONE);
		SDL_UpdateTexture(BG_TEXTURE, nullptr, BG_GRID_PIXELS.get(), BG_GRID_W * sizeof(COLOR));
	}

	FILE_INFO_PIXELS(std::filesystem::path _p, std::string _f, uint16_t _cw, uint16_t _ch)
	{
		CURRENT_TOOL = TOOL::BRUSH;
		type = FILE_TYPE::PIXELS;
		path = _p;
		filename = _f;
		canvas_w = _cw;
		canvas_h = _ch;
		type = type;
		update = 1;

		brush_color.push_back(COLOR{ 255,255,255,255 });
		current_brush_color = 0;

		BG_TEXTURE = SDL_CreateTexture(RENDERER_MAIN, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 1, 1);

		refresh_canvas();

		canvas_zoom = 1;
		//canvas_zoom = 10; // could scale to fit window ?

		canvas_center();

		add_frame_pixels(1);
		
		SDL_UpdateTexture(current_layer_ptr->texture, nullptr, current_layer_ptr->pixels.get(), canvas_pitch);
	}

	void set_pixel(const uint32_t x, const uint32_t y, const COLOR c, COLOR* p)
	{
		if (x >= canvas_w || y >= canvas_h) return;
		const uint32_t pos = y * (uint32_t)canvas_w + x;
		if (brush_update_bool[pos]) return;

		const COLOR dest = p[pos];
		
		p[pos] = blend_colors(c, dest);
		brush_update_bool[pos] = true;
		brush_update_stack.emplace(std::pair<uint32_t, COLOR>{pos, dest});

		brush_update_region = brush_update_region.include_point(x, y);
	}

	void set_pixel_erase(const uint32_t x, const uint32_t y, const COLOR c, COLOR* p)
	{
		if (x >= canvas_w || y >= canvas_h) return;
		const uint32_t pos = y * (uint32_t)canvas_w + x;
		if (brush_update_bool[pos]) return;

		const COLOR dest = p[pos];

		p[pos].a = (uint8_t)((float)dest.a * ((float)(0xff - c.a) / 255.0f));
		brush_update_bool[pos] = 1;
		brush_update_stack.emplace(std::pair<uint32_t, COLOR>{pos, dest});

		brush_update_region = brush_update_region.include_point(x, y);

	}

	void set_pixel_line(int32_t x0, int32_t y0, const int32_t x1, const int32_t y1, const COLOR c)
	{
		int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
		int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
		int32_t err = dx + dy, e2;
		auto _tc = (current_layer_ptr->pixels.get());
		for (;;) {

			if (CURRENT_TOOL == TOOL::ERASER)
			{
				set_pixel_erase(x0, y0, c, _tc);
			}
			else
			{
				set_pixel(x0, y0, c, _tc);
			}

			if (x0 == x1 && y0 == y1) break;
			e2 = err << 1;
			if (e2 >= dy) { err += dy; x0 += sx; }
			if (e2 <= dx) { err += dx; y0 += sy; }
		}
		layer_update = 2;

		/*PRINT(canvas_w);
		PRINT(canvas_h);
		PRINT((int)sizeof(COLOR));
		PRINT((int)canvas_pitch);*/
	}

	void INPUT_LOOP() override
	{
		canvas_mouse_prevx = canvas_mouse_x;
		canvas_mouse_prevy = canvas_mouse_y;
		double t_CANVAS_MOUSE_X = ((double)(((double)MOUSE_X / (double)canvas_zoom) - ((double)canvas_x / (double)canvas_zoom)));
		double t_CANVAS_MOUSE_Y = ((double)(((double)MOUSE_Y / (double)canvas_zoom) - ((double)canvas_y / (double)canvas_zoom)));

		canvas_mouse_x = (int16_t)floor(t_CANVAS_MOUSE_X);
		canvas_mouse_y = (int16_t)floor(t_CANVAS_MOUSE_Y);

		canvas_mouse_cellx = (int16_t)floor(t_CANVAS_MOUSE_X / (double)cell_w);
		canvas_mouse_celly = (int16_t)floor(t_CANVAS_MOUSE_Y / (double)cell_h);

		if (UIBOX_IN <= -1)
		{
			if (!KEYBOARD_PREVALT && KEYBOARD_ALT && KEYBOARD_PRESSED_ALT && CURRENT_TOOL == TOOL::BRUSH)
			{
				CURRENT_TOOL = TOOL::PICKER;
			}
			else
			if (KEYBOARD_PREVALT && !KEYBOARD_ALT && !KEYBOARD_PRESSED_ALT && CURRENT_TOOL == TOOL::PICKER)
			{
				CURRENT_TOOL = TOOL::BRUSH;
			}

			if (!KEYBOARD_PREVSPACE && KEYBOARD_SPACE && KEYBOARD_PRESSED_SPACE && (CURRENT_TOOL == TOOL::BRUSH || CURRENT_TOOL == TOOL::ERASER || CURRENT_TOOL == TOOL::PICKER || CURRENT_TOOL == TOOL::FILL))
			{
				TEMP_CURRENT_TOOL = CURRENT_TOOL;
				CURRENT_TOOL = TOOL::PAN;
			}
			else
			if (KEYBOARD_PREVSPACE && !KEYBOARD_SPACE && !KEYBOARD_PRESSED_SPACE && CURRENT_TOOL == TOOL::PAN)
			{
				CURRENT_TOOL = TEMP_CURRENT_TOOL;
			}

			if (MOUSEWHEEL_Y != 0)
			{
				float t_CANVAS_ZOOM = canvas_zoom;
				canvas_zoom = clamp(canvas_zoom + (((float)abs(MOUSEWHEEL_Y) +
					truncf(canvas_zoom / 10.0f) +
					truncf(canvas_zoom / 20.0f) +
					truncf(canvas_zoom / 30.0f) +
					truncf(canvas_zoom / 40.0f) +
					truncf(canvas_zoom / 50.0f)) * sign((float)MOUSEWHEEL_Y)), 1.0f, 100.0f);

				if (t_CANVAS_ZOOM != canvas_zoom)
				{
					float _mx = (((float)MOUSE_X - (float)canvas_x) / (float)t_CANVAS_ZOOM), _my = (((float)MOUSE_Y - (float)canvas_y) / (float)t_CANVAS_ZOOM);

					float _nmx = (((float)MOUSE_X - (float)canvas_x) / (float)canvas_zoom), _nmy = (((float)MOUSE_Y - (float)canvas_y) / (float)canvas_zoom);
					canvas_x += (float)((_nmx - _mx) * canvas_zoom);
					canvas_y += (float)((_nmy - _my) * canvas_zoom);
				}
			}
		}

		if (FUNCTION_UNDO)
		{
			undo<UNDO_STACK_PIXELS>(undo_stack);
		}
		else
		if (FUNCTION_REDO)
		{
			redo<UNDO_STACK_PIXELS>(undo_stack);
		}

		if (MOUSEBUTTON_LEFT && (canvas_clicked || UIBOX_IN <= -1))
		{
			if (UIBOX_IN <= -1 && MOUSEBUTTON_PRESSED_LEFT)
			{
				canvas_clicked = 1;
			}

			if (can_draw_tick)
			{
				switch (CURRENT_TOOL)
				{
				case TOOL::ERASER:
				case TOOL::BRUSH:
					set_pixel_line(canvas_mouse_prevx, canvas_mouse_prevy, canvas_mouse_x, canvas_mouse_y, brush_color[current_brush_color]);
					break;

				case TOOL::PICKER:
					brush_color[current_brush_color] = current_layer_ptr->pixels[canvas_mouse_y * canvas_w + canvas_mouse_x];// get_pixel_layer(CANVAS_MOUSE_X, CANVAS_MOUSE_Y, CURRENT_LAYER);
					break;

				case TOOL::FILL:
					break;

				case TOOL::PAN:
					if (UIBOX_IN <= -1 && MOUSEBUTTON_PRESSED_LEFT)
					{
						canvas_clicked = 1;
						canvas_pan_x = (float)MOUSE_X - canvas_x;
						canvas_pan_y = (float)MOUSE_Y - canvas_y;
					}
					canvas_x = (float)MOUSE_X - canvas_pan_x;
					canvas_y = (float)MOUSE_Y - canvas_pan_y;
					break;

				default:
					break;
				}
			}
			else
				can_draw_tick = 1;
		}
		else
		if (MOUSEBUTTON_MIDDLE && (canvas_clicked || UIBOX_IN <= -1))
		{
			if (UIBOX_IN <= -1 && MOUSEBUTTON_PRESSED_MIDDLE)
			{
				canvas_clicked = 1;
				canvas_pan_x = (float)MOUSE_X - canvas_x;
				canvas_pan_y = (float)MOUSE_Y - canvas_y;
			}
			canvas_x = (float)MOUSE_X - canvas_pan_x;
			canvas_y = (float)MOUSE_Y - canvas_pan_y;
		}
		else
		if (MOUSEBUTTON_RIGHT && (canvas_clicked || UIBOX_IN <= -1))
		{
			if (UIBOX_IN <= -1 && MOUSEBUTTON_PRESSED_RIGHT)
			{
				canvas_clicked = 1;
			}
			//
		}
		else canvas_clicked = 0;
	}

	void UPDATE_LOOP() override
	{
		if (layer_update == 2)
		{
			if (!current_layer_ptr->texture && !brush_update_region.is_empty())
			{
				SDL_Rect const dirty_rect = brush_update_region.to_sdl();
				int const dirty_region_start_index = brush_update_region.top * canvas_w + brush_update_region.left;
				SDL_UpdateTexture(current_layer_ptr->texture, &dirty_rect, &current_layer_ptr->pixels[dirty_region_start_index], canvas_pitch);

				QUAD const canvas_rect = QUAD::from_wh(canvas_w, canvas_h);
				QUAD const clipped_brush_update_region = brush_update_region.clip_to(canvas_rect);

				layer_update_region = layer_update_region.include_region(clipped_brush_update_region);

				brush_update_region = QUAD::empty();
			}
			layer_update = 1;
		}
		else if (layer_update == 1)
		{
			std::pair<uint64_t, COLOR> _bus;
			uint32_t _tpos;
			COLOR _tcol;

			if (!brush_update_stack.empty())
			{
				UNDO_ENTRY_PIXELS _u{ layer_update_region, current_layer, current_frame };

				plf::stack<std::pair<uint32_t, COLOR>> _ustack;
				plf::stack<std::pair<uint32_t, COLOR>> _rstack;
				_ustack = brush_update_stack;

				_u.undo_pixels = std::move(_ustack);

				while (!brush_update_stack.empty())
				{
					_bus = brush_update_stack.top();
					brush_update_stack.pop();
					_tpos = _bus.first;
					_tcol = _bus.second;

					brush_update_bool[_tpos] = false;

					_u.redo_pixels.emplace(std::pair<uint32_t, COLOR>{_tpos, current_layer_ptr->pixels[_tpos]});

					// send _tcol to update;
				}
				//PRINT((int)undo_stack.entries.size());
				//push_undo_entry<UNDO_STACK_PIXELS, UNDO_ENTRY_PIXELS>(undo_stack, _u);

				// if we're back a few steps in the undo reel, we clear all the above undo steps.
				if (undo_stack.position > 0) {
					auto const old_end = undo_stack.entries.end();
					auto const new_end = std::prev(old_end, undo_stack.position);
					undo_stack.entries.erase(new_end, old_end);
				}

				// add the new undo
				undo_stack.entries.push_back(std::move(_u));
				undo_stack.position = 0;

				//PRINT((int)undo_stack.entries.size());
			}

			// add the new undo
			//push_undo_entry(_u);
			layer_update_region = QUAD::empty();
			layer_update = 0;
		}
	}

	void DRAW_BG_GRID()
	{
		float _tcanvas_w = ((float)canvas_w * canvas_zoom);
		float _tcanvas_h = ((float)canvas_h * canvas_zoom);
		float _tcell_w = ((float)cell_w * canvas_zoom);
		float _tcell_h = ((float)cell_h * canvas_zoom);

		int bg_w = (int)floor(ceilf(_tcanvas_w / _tcell_w) * _tcell_h);
		int bg_h = (int)floor(ceilf(_tcanvas_h / _tcell_h) * _tcell_h);
		SDL_Rect _trect = { (int)canvas_x, (int)canvas_y, bg_w, bg_h };
		SDL_SetTextureBlendMode(BG_TEXTURE, SDL_BLENDMODE_NONE);
		SDL_RenderCopy(RENDERER_MAIN, BG_TEXTURE, nullptr, &_trect);

		// these 2 rects cover the overhang the background grid has beyond the canvas
		SDL_SetRenderDrawColor(RENDERER_MAIN, 0, 0, 0, 255);
		_trect = {
			(int)canvas_x, (int)(std::max(0.0f,canvas_y + _tcanvas_h)),
			(int)bg_w, (int)_tcell_h
		};
		SDL_RenderFillRect(RENDERER_MAIN, &_trect);
		_trect = {
			(int)(canvas_x + ((float)canvas_w * canvas_zoom)), (int)canvas_y,
			(int)_tcell_w, (int)bg_h
		};
		SDL_RenderFillRect(RENDERER_MAIN, &_trect);
	}

	void RENDER_LOOP() override
	{
		SDL_Rect _q;

		DRAW_BG_GRID();

		auto draw_layer = [&](int l, int x, int y)
		{
			_q = {
				(int)(canvas_x + (x * canvas_w)),
				(int)(canvas_y + (y * canvas_h)),
				(int)((float)canvas_w * canvas_zoom),
				(int)((float)canvas_h * canvas_zoom) };

			SDL_SetTextureBlendMode(current_frame_ptr->layers[l]->texture, (SDL_BlendMode)current_frame_ptr->layers[l]->blendmode);
			SDL_RenderCopy(RENDERER_MAIN, current_frame_ptr->layers[l]->texture, nullptr, &_q);
			if (l == current_layer)
			{
				if (brush_update)
				{
					SDL_SetTextureBlendMode(brush_texture, SDL_BLENDMODE_BLEND);
					SDL_RenderCopy(RENDERER_MAIN, brush_texture, nullptr, &_q);
				}
			}
		};

		for (int l = 0; l < current_frame_ptr->layers.size(); l++)
		{
			draw_layer(l, 0, 0);

			if (view_tiled_x)
			{
				draw_layer(l, 1, 0);
				draw_layer(l, -1, 0);

				if (view_tiled_y)
				{
					draw_layer(l, 1, 1);
					draw_layer(l, -1, -1);
					draw_layer(l, 1, -1);
					draw_layer(l, -1, 1);
				}
			}
			if (view_tiled_y)
			{
				draw_layer(l, 0, 1);
				draw_layer(l, 0, -1);
			}
		}

		SDL_SetRenderDrawColor(RENDERER_MAIN, 64, 64, 64, 255);
		_q = {
				(int)(canvas_x - 1),
				(int)(canvas_y - 1),
				(int)(((float)canvas_w * canvas_zoom) + 2),
				(int)(((float)canvas_h * canvas_zoom) + 2) };
		SDL_RenderDrawRect(RENDERER_MAIN, &_q);
	}

	void apply_undo_data(UNDO_ENTRY_PIXELS* undo_entry, bool is_undo) override
	{
		//auto const region = undo_entry->affected_region;
		auto& data = is_undo ? undo_entry->undo_pixels : undo_entry->redo_pixels;
		auto const layer = undo_entry->affected_layer;
		auto const frame = undo_entry->affected_frame;

		/*int index;
		for (auto r : region)
		{
			index = (r.first - region.left) + (r.second - region.top) * region.width();
			frames[frame]->layers[layer]->pixels[r.second * canvas_w + r.first] = data[index];
			//set_pixel_layer(x, y, data[index], file->frames[undo_entry->affected_frame]->layers[(undo_entry->affected_layer)]);
		}*/
		
		if (!data.empty())
		{
			auto _tdata = data;
			std::pair<uint32_t, COLOR> _bus;
			//uint32_t _tpos;
			//COLOR _tcol;
			while (!data.empty())
			{
				_bus = data.top();
				data.pop();
				//_tpos = _bus.first;
				//_tcol = _bus.second;

				frames[frame]->layers[layer]->pixels[_bus.first] = _bus.second;
			}

			data = std::move(_tdata);

			SDL_Rect const dirty_rect = undo_entry->affected_region.to_sdl();
			int const dirty_region_start_index = undo_entry->affected_region.top * canvas_w + undo_entry->affected_region.left;
			SDL_UpdateTexture(current_layer_ptr->texture, &dirty_rect, &frames[frame]->layers[layer]->pixels[dirty_region_start_index], canvas_pitch);
		}

		update = true;
		current_layer = undo_entry->affected_layer;
		current_frame = undo_entry->affected_frame;

		// change the pointers too

		//undo_stack.update_region = region;
	}
};

//extern void add_layer_pixels(std::shared_ptr<FILE_INFO_PIXELS> file, std::shared_ptr<FRAME_INFO_PIXELS> frame, int16_t _x, int16_t _y, int16_t _a, /*SDL_BlendMode*/ int _b);
//extern void add_frame_pixels(std::shared_ptr<FILE_INFO_PIXELS> file, int frame_time);
extern void add_file_pixels(FILE_TYPE type, std::filesystem::path path, std::string filename, uint16_t canvas_w, uint16_t canvas_h);
extern void open_file(std::filesystem::path path);

extern void UPDATE_PATH_FILES();

extern std::vector<std::shared_ptr<FILE_INFO>> OPEN_FILES;
extern int CURRENT_FILE;
extern FILE_TYPE CURRENT_FILE_PTR_TYPE;
extern std::shared_ptr<FILE_INFO_PIXELS> CURRENT_FILE_PTR_PIXELS;
//extern std::shared_ptr<FILE_INFO_PIXELS> CURRENT_FILE_PTR_MAP;