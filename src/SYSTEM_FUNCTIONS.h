#pragma once

extern void PRINT(std::string print);
extern void PRINT(const char* print);
extern void PRINT(int print);
extern void PRINT(double print);

extern int clamp(int i, int hi, int lo);
extern float clamp(float i, float hi, float lo);

extern int16_t sign(const int16_t x);
extern float sign(const float x);

extern int ctoi(const char* s);

extern bool point_in_rect(const uint16_t px, const uint16_t py, const uint16_t rx, const uint16_t ry, const uint16_t rw, const uint16_t rh);

extern void binaryfile_write(std::string const& fileName, std::string const& data);
extern void binaryfile_read(std::string const& fileName, std::string& data);

struct UIBOX_ELEMENT_MAIN;
extern UIBOX_ELEMENT_MAIN* UIBOX_ELEMENT_CLICK;
extern void confirm_input();

//

extern void INIT_FONT();

extern bool INIT();