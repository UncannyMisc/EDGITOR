
#include "UIBOX_ELEMENTS_FUNCTIONS.h"
#include "UIBOX_ELEMENTS.h"

void uibox_element_add_textbox(UIBOX_INFO* uibox, uint16_t x, uint16_t y, bool highlight, std::string text)
{
	std::shared_ptr<UIBOX_ELEMENT_TEXTBOX> _element = std::make_shared<UIBOX_ELEMENT_TEXTBOX>();
	_element->x = x;
	_element->y = y;
	_element->text = text;
	_element->w = 1;// (fw ? uibox->chr_w - 4 : text.size());
	_element->h = 1;
	_element->highlight = highlight;
	_element->parent_uibox = uibox;
	_element->create(uibox);
	uibox->element_list.push_back(std::move(_element));
}

void uibox_element_add_file_new(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, uint16_t h, std::string text, std::string sel_text, std::string filename, FILE_TYPE file_type)
{
	std::shared_ptr<UIBOX_ELEMENT_FILE_NEW> _element = std::make_shared<UIBOX_ELEMENT_FILE_NEW>();
	_element->x = x;
	_element->y = y;
	_element->text = text;
	_element->sel_text = sel_text;
	_element->w = (!w ? uibox->chr_w - 4 : text.size() + (w - 1));
	_element->h = h;
	_element->file_type = file_type;
	_element->filename = filename;
	_element->parent_uibox = uibox;
	_element->create(uibox);
	uibox->element_list.push_back(std::move(_element));
}

void uibox_element_add_create_file(UIBOX_INFO* uibox, uint16_t x, uint16_t y, std::string text)
{
	std::shared_ptr<UIBOX_ELEMENT_CREATE_FILE> _element = std::make_shared<UIBOX_ELEMENT_CREATE_FILE>();
	_element->x = x;
	_element->y = y;
	_element->text = text;
	_element->w = text.size();
	_element->h = 1;
	_element->highlight = 1;
	_element->parent_uibox = uibox;
	_element->create(uibox);
	uibox->element_list.push_back(std::move(_element));
}



