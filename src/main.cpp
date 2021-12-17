// #include "engine.h"
#include "hardware/vga.h"
#include "assert.h"
#include "stdint.h"
#include "stdlib.h"
#include "event.h"
#include "hardware/keyboard.h"
#include "hardware/timer.h"
#include "hardware/fpu.h"
#include "string.h"
#include "math.h"

namespace std
{
    double abs(double x)             { return fpu_abs(x);       }
    double atan(double x)            { return fpu_atan(x);      }
    double atan2(double y, double x) { return fpu_atan2(y, x);  }
    double ceil(double x)            { return fpu_ceil(x);      }
    double cos(double x)             { return fpu_cos(x);       }
    double floor(double x)           { return fpu_floor(x);     }
    double mod(double x, double y)   { return fpu_modulo(x, y); }
    double pow(double x, double y)   { return fpu_pow(x, y);    }
    double sin(double x)             { return fpu_sin(x);       }
    double sqrt(double x)            { return fpu_sqrt(x);      }
    double tan(double x)             { return fpu_tan(x);       }
    double trunc(double x)           { return fpu_trunc(x);     }
    
    template<typename T>
    inline const T& swap(const T& __a, const T& __b)
    { const T __t = __a; __b = __a; __a = __t; }

    template<typename T>
    inline const T& max(const T& __a, const T& __b)
    {
        if(__a < __b) return __b;
        return __a;
    }

    template<typename T>
    inline const T& min(const T& __a, const T& __b)
    {
        if(__a > __b) return __b;
        return __a;
    }

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


using namespace std;

int main()
{
    srand(0);

    printf("abs  (-3.14)     %lf\n", abs(-3.14));
    printf("atan (3.14)      %lf\n", atan(3.14));
    printf("atan2(3.14, 0.5) %lf\n", atan2(3.14, 0.5));
    printf("ceil (3.14)      %lf\n", ceil (3.14));
    printf("cos  (3.14)      %lf\n", cos  (3.14));
    printf("floor(3.14)      %lf\n", floor(3.14));
    printf("mod  (3.14, 0.5) %lf\n", mod  (3.14, 0.5));
    printf("pow  (100., 0.5) %lf\n", pow  (100., 0.5));
    printf("sin  (3.14)      %lf\n", sin  (100.));
    printf("sqrt (100.)      %lf\n", sqrt (3.14));
    printf("tan  (3.14)      %lf\n", tan  (3.14));
    printf("trunc(3.14)      %lf\n", trunc(3.14));

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
            print_event(&e);
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
        int period = 0;
        for(int x = 0; x < 320; x++)
        {
            double long_wave = sin((off*1.1f) + float(x) / 320.f) * 50;
            double short_wave = sin((off*2.0f) + float(x) / 320.f * PI * 2) * 50;
            if((short_wave - -49.) < 0.0000000001)
                period = x;

            Draw(x, 100 + short_wave, (col++ % 16));
            Draw(x, 100 + long_wave, 15);
        }

        for(int y = 0; y < 200; y++)
        {
            Draw(period, y, 2);
        }
        Swap();
    }

    return 0;
}
