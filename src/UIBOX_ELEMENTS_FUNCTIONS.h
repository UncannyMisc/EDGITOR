#pragma once

#include "MAIN_LIBRARIES.h"
#include "GLOBAL_VARIABLES.h"

#include "UIBOX_ELEMENTS.h"

struct UIBOX_INFO;

extern void uibox_element_add_textbox(UIBOX_INFO* uibox, uint16_t x, uint16_t y, bool highlight, std::string text);
template<typename T>
extern void uibox_element_add_button(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, uint16_t h, std::string text, std::string sel_text, T* input_var, T button_var);
template<typename T>
extern void uibox_element_add_button(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, uint16_t h, std::string text, std::string sel_text, T* input_var, T button_var, COLOR col, bool hold);
template<typename T>
extern void uibox_element_add_varbox(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, T* input_var, T button_var);
extern void uibox_element_add_file_new(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, uint16_t h, std::string text, std::string sel_text, std::string filename, FILE_TYPE file_type);
template<typename T>
extern void uibox_element_add_input(UIBOX_INFO* uibox, uint16_t x, uint16_t y, uint16_t w, uint16_t h, std::string text, std::string sel_text, T default_value, T* input_var);
extern void uibox_element_add_create_file(UIBOX_INFO* uibox, uint16_t x, uint16_t y, std::string text);

#include "UIBOX_ELEMENTS_FUNCTIONS_TEMPLATES.inl"