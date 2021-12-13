// #include "engine.h"
#include "hardware/vga.h"
#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "event.h"
#include "hardware/keyboard.h"
#include "hardware/timer.h"
#include "string.h"
#include "math.h"

namespace std
{


} // std

void Swap()
{
    vga_swap();
}

void Clear(uint8_t col)
{
    vga_graphics_clear(col);
}

void Draw(int x, int y, uint8_t col)
{
    if(x < 0 || y < 0 || x > 319 || y > 199)
        return;

    vga_graphics_draw(x, y, col);
}

void Rect(int x, int y, int w, int h, uint8_t col)
{
    for(int j = y; j < y + h; j++)
        for(int i = x; i < x + w; i++)
            Draw(i, j, col);
}

int main()
{
    srand(0);
    bool graphicsMode = false;
    bool graphicsRunning = true;

    uint32_t tp1, tp2;
    tp1 = tp2 = timer_get_time_point();
    float off = 0.0f;

    float fpsTracker = 0.0f;
    int FPS = 0;
    int col = 0;

    while(1)
    {
        tp2 = timer_get_time_point();
        float elapsed = (float)(tp2 - tp1) / 1000;
        tp1 = tp2;

        fpsTracker += elapsed;
        FPS++;
        if(fpsTracker > 1.0f)
        {
            printf("FPS: %d\n", FPS);
            FPS = 0;
            fpsTracker -= 1.0f;
            col = rand() % 256;
        }
        
        event_t e;
        while(event_pending())
        {
            event_next(&e);
            // print_event(&e);
            if(e.type == KeyPress && e.key.keycode == KEY_SPACE)
            {
                graphicsMode = !graphicsMode;
                vga_set_mode((video_mode_t)graphicsMode);
                printf("Switch Mode (%d)\n", graphicsMode);
            }

            if(e.type == KeyPress && e.key.keycode == KEY_ENTER)
            {
                graphicsRunning = !graphicsRunning;
                printf("Graphics Running (%d)\n", graphicsRunning);
            }
        }
        vga_text_set_attribute((col % 15) + 1);        
        if(!graphicsRunning)
            continue;

        off += elapsed;
        

        Clear(0);
        
        float radius = 50.0f;
        Rect(160 + (cos(off) * radius), 100 + (sin(off) * radius), 5, 5, (col++ % 16));
        Draw(160, 100, 3);

        for(int x = 0; x < 320; x++)
        {
            Draw(x, 100 + sin((off*2.0f) + float(x) / 320.f * PI * 2) * 50, (col++ % 16));
            Draw(x, 100 + sin((off*1.1f) + float(x) / 320.f) * 50, 15);
        }

        Swap();
    }


    
    return 0;
}
