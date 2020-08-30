#pragma once

template<typename T>
void uibox_element_add_button(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, uint16_t h, std::string text, std::string sel_text, T* input_var, T button_var)
{
	std::shared_ptr<UIBOX_ELEMENT_BUTTON<T>> _element = std::make_shared<UIBOX_ELEMENT_BUTTON<T>>();
	_element->x = x;
	_element->y = y;
	_element->text = text;
	_element->sel_text = sel_text;
	_element->w = (!w ? uibox->chr_w - 4 : text.size() + (w - 1));
	_element->h = h;
	_element->input_var = input_var;
	_element->button_var = button_var;
	_element->parent_uibox = uibox;
	_element->create(uibox);
	uibox->element_list.push_back(std::move(_element));
}

template<typename T>
void uibox_element_add_varbox(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, T* input_var, T button_var)
{
	std::shared_ptr<UIBOX_ELEMENT_VARBOX<T>> _element = std::make_shared<UIBOX_ELEMENT_VARBOX<T>>();
	_element->x = x;
	_element->y = y;
	_element->w = (!w ? uibox->chr_w - 4 : w);
	_element->h = 1;
	_element->input_var = input_var;
	_element->button_var = button_var;
	_element->parent_uibox = uibox;
	_element->create(uibox);
	uibox->element_list.push_back(std::move(_element));
}

template<typename T>
void uibox_element_add_input(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, uint16_t h, std::string text, std::string sel_text, T default_value, T* input_var)
{
	std::shared_ptr<UIBOX_ELEMENT_INPUT<T>> _element = std::make_shared<UIBOX_ELEMENT_INPUT<T>>();
	_element->x = x;
	_element->y = y;
	_element->text = text;
	_element->sel_text = sel_text;
	_element->w = (!w ? uibox->chr_w - 4 : text.size() + (w - 1));
	_element->h = h;
	_element->input_var = input_var;
	_element->default_value = default_value;
	_element->parent_uibox = uibox;
	_element->create(uibox);
	uibox->element_list.push_back(std::move(_element));
}