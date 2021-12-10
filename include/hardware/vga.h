#ifndef VGA_H
#define VGA_H

#ifdef __cplusplus
extern "C" {
#endif

enum video_mode_enum {
    TextMode,
    GraphicsMode
};
typedef enum video_mode_enum video_mode_t;

void vga_init();
void vga_clear();

void vga_move_cursor(int col, int row);
void vga_putc(char ch);

void vga_mode13();
bool vga_running();

#ifdef __cplusplus
}
#endif


#endif