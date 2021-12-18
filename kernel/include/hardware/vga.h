#ifndef VGA_H
#define VGA_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

enum video_mode_enum {
    TextMode,
    GraphicsMode
};
typedef enum video_mode_enum video_mode_t;

void vga_init();
void vga_set_mode(video_mode_t mode);

void vga_text_clear();
void vga_text_putc(char ch);
void vga_text_set_attribute(uint8_t attr);

void vga_graphics_clear(uint8_t col); 
void vga_graphics_draw(int x, int y, uint8_t col);

void vga_swap();

bool vga_running();

#ifdef __cplusplus
}
#endif


#endif