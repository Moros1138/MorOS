#include "multiboot.h"

#include "types.h"
#include "monitor.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "memory.h"
#include "random.h"

using namespace MorOS;

constexpr uint32_t ScreenWidth = 320;
constexpr uint32_t ScreenHeight = 200;
uint8_t pixels[ScreenWidth * ScreenHeight];

void Draw(int32_t x, int32_t y, uint8_t col)
{
    if(x < 0 || y < 0 || x >= ScreenWidth || y >= ScreenHeight )
        return;
    
    pixels[y * ScreenWidth + x] = col;
}

void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t col)
{
    int32_t x2 = x + w;
    int32_t y2 = y + h;

    // if (x < 0) x = 0;
    // if (x >= (int32_t)GetDrawTargetWidth()) x = (int32_t)GetDrawTargetWidth();
    // if (y < 0) y = 0;
    // if (y >= (int32_t)GetDrawTargetHeight()) y = (int32_t)GetDrawTargetHeight();

    // if (x2 < 0) x2 = 0;
    // if (x2 >= (int32_t)GetDrawTargetWidth()) x2 = (int32_t)GetDrawTargetWidth();
    // if (y2 < 0) y2 = 0;
    // if (y2 >= (int32_t)GetDrawTargetHeight()) y2 = (int32_t)GetDrawTargetHeight();

    for (int i = x; i < x2; i++)
        for (int j = y; j < y2; j++)
            Draw(i, j, col);
}

void FillCircle(int32_t x, int32_t y, int32_t radius, uint8_t p)
{ // Thanks to IanM-Matrix1 #PR121
    // if (radius < 0 || x < -radius || y < -radius || x - GetDrawTargetWidth() > radius || y - GetDrawTargetHeight() > radius)
    //     return;

    if (radius > 0)
    {
        int x0 = 0;
        int y0 = radius;
        int d = 3 - 2 * radius;

        auto drawline = [&](int sx, int ex, int y)
        {
            for (int x = sx; x <= ex; x++)
                Draw(x, y, p);
        };

        while (y0 >= x0)
        {
            drawline(x - y0, x + y0, y - x0);
            if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

            if (d < 0)
                d += 4 * x0++ + 6;
            else
            {
                if (x0 != y0)
                {
                    drawline(x - x0, x + x0, y - y0);
                    drawline(x - x0, x + x0, y + y0);
                }
                d += 4 * (x0++ - y0--) + 10;
            }
        }
    }
    else
        Draw(x, y, p);
}


extern "C" void _main(multiboot_info_t* mbd, uint32_t)
{
    // global descriptor table
    MorOS::GlobalDescriptorTable gdt{};
    
    // interrupts
    MorOS::InterruptManager interruptManager{};

    // 33.33333333ms per tick
    MorOS::Timer(30);

    // memory
    MorOS::MemoryManager memoryManager(mbd);

    // randomizer
    MorOS::Random random{};

    // monitor (VGA mode 13h)
    MorOS::Monitor monitor{};

    uint32_t tp1, tp2;
    // initial time points
    tp1 = tp2 = MorOS::Timer::tick;
    
    bool flag = false;
    
    // just chill out forever!
    while(1)
    {
        // tp2 = MorOS::Timer::tick;
        // float fElapsedTime = ((float)(tp2 - tp1) * 33) / 1000;
        // tp1 = tp2;

        flag = !flag;

        for(int y = 0; y < ScreenHeight; y++)
            for(int x = 0; x < ScreenWidth; x++)
                Draw(x, y, (MorOS::rand() % 255) + 1);

        // FillRect(0, 0, ScreenWidth, ScreenHeight, 0);
        // FillRect(5, 5, ScreenWidth-10, ScreenHeight-10, 4);
        // FillRect(10, 10, ScreenWidth-20, ScreenHeight-20, 3);
        // FillCircle(ScreenWidth / 2, ScreenHeight / 2, 50, (flag) ? 4 : 14);

        MorOS::memcpy((uint8_t*)0xA0000, pixels, ScreenWidth * ScreenHeight);
    }
}