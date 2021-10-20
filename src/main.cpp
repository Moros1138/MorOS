#include "multiboot.h"

#include "types.h"

#include "descriptors/gdt.h"
#include "descriptors/idt.h"
#include "hardware/monitor.h"
#include "hardware/timer.h"
#include "hardware/keyboard.h"
#include "hardware/mouse.h"
#include "memory.h"
#include "random.h"

using namespace MorOS;

namespace std
{
    template<class T>
    void swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
} // std

int abs(int x)
{
    return (x < 0) ? -x : x;
}

int GetDrawTargetWidth()
{
    return 320;
}

int GetDrawTargetHeight()
{
    return 200;
}

void Clear(uint8_t col)
{
    memset(Monitor::activeMonitor->GetBuffer(), col, 64000);
}

bool Draw(int x, int y, uint8_t col)
{
    if(x < 0 || y < 0 || x >= 320 || y >= 200)
        return false;

    Monitor::activeMonitor->GetBuffer()[y * 320 + x] = col;
    
    return true;
}

void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t p, uint32_t pattern = 0xFFFFFFFF)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1; dy = y2 - y1;

    auto rol = [&](void) { pattern = (pattern << 1) | (pattern >> 31); return pattern & 1; };

    // straight lines idea by gurkanctn
    if (dx == 0) // Line is vertical
    {
        if (y2 < y1) std::swap(y1, y2);
        for (y = y1; y <= y2; y++) if (rol()) Draw(x1, y, p);
        return;
    }

    if (dy == 0) // Line is horizontal
    {
        if (x2 < x1) std::swap(x1, x2);
        for (x = x1; x <= x2; x++) if (rol()) Draw(x, y1, p);
        return;
    }

    // Line is Funk-aye
    dx1 = abs(dx); dy1 = abs(dy);
    px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1; y = y1; xe = x2;
        }
        else
        {
            x = x2; y = y2; xe = x1;
        }

        if (rol()) Draw(x, y, p);

        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
                px = px + 2 * dy1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
                px = px + 2 * (dy1 - dx1);
            }
            if (rol()) Draw(x, y, p);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1; y = y1; ye = y2;
        }
        else
        {
            x = x2; y = y2; ye = y1;
        }

        if (rol()) Draw(x, y, p);

        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
                py = py + 2 * dx1;
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
                py = py + 2 * (dx1 - dy1);
            }
            if (rol()) Draw(x, y, p);
        }
    }
}

void DrawCircle(int32_t x, int32_t y, int32_t radius, uint8_t p, uint8_t mask = 0xff)
{ // Thanks to IanM-Matrix1 #PR121
    if (radius < 0 || x < -radius || y < -radius || x - GetDrawTargetWidth() > radius || y - GetDrawTargetHeight() > radius)
        return;

    if (radius > 0)
    {
        int x0 = 0;
        int y0 = radius;
        int d = 3 - 2 * radius;

        while (y0 >= x0) // only formulate 1/8 of circle
        {
            // Draw even octants
            if (mask & 0x01) Draw(x + x0, y - y0, p);// Q6 - upper right right
            if (mask & 0x04) Draw(x + y0, y + x0, p);// Q4 - lower lower right
            if (mask & 0x10) Draw(x - x0, y + y0, p);// Q2 - lower left left
            if (mask & 0x40) Draw(x - y0, y - x0, p);// Q0 - upper upper left
            if (x0 != 0 && x0 != y0)
            {
                if (mask & 0x02) Draw(x + y0, y - x0, p);// Q7 - upper upper right
                if (mask & 0x08) Draw(x + x0, y + y0, p);// Q5 - lower right right
                if (mask & 0x20) Draw(x - y0, y + x0, p);// Q3 - lower lower left
                if (mask & 0x80) Draw(x - x0, y - y0, p);// Q1 - upper left left
            }

            if (d < 0)
                d += 4 * x0++ + 6;
            else
                d += 4 * (x0++ - y0--) + 10;
        }
    }
    else
        Draw(x, y, p);
}


void FillCircle(int32_t x, int32_t y, int32_t radius, uint8_t p)
{ // Thanks to IanM-Matrix1 #PR121
    if (radius < 0 || x < -radius || y < -radius || x - GetDrawTargetWidth() > radius || y - GetDrawTargetHeight() > radius)
        return;

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


void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t p)
{
    DrawLine(x, y, x + w, y, p);
    DrawLine(x + w, y, x + w, y + h, p);
    DrawLine(x + w, y + h, x, y + h, p);
    DrawLine(x, y + h, x, y, p);
}

void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t p)
{
    int32_t x2 = x + w;
    int32_t y2 = y + h;

    if (x < 0) x = 0;
    if (x >= (int32_t)GetDrawTargetWidth()) x = (int32_t)GetDrawTargetWidth();
    if (y < 0) y = 0;
    if (y >= (int32_t)GetDrawTargetHeight()) y = (int32_t)GetDrawTargetHeight();

    if (x2 < 0) x2 = 0;
    if (x2 >= (int32_t)GetDrawTargetWidth()) x2 = (int32_t)GetDrawTargetWidth();
    if (y2 < 0) y2 = 0;
    if (y2 >= (int32_t)GetDrawTargetHeight()) y2 = (int32_t)GetDrawTargetHeight();

    for (int i = x; i < x2; i++)
        for (int j = y; j < y2; j++)
            Draw(i, j, p);
}

int ScreenWidth()
{
    return GetDrawTargetWidth();
}

int ScreenHeight()
{
    return GetDrawTargetHeight();
}

extern "C" void _main(multiboot_info_t* mbd, uint32_t)
{
    // global descriptor table
    GlobalDescriptorTable gdt{};
    
    // interrupts
    InterruptManager interruptManager{};

    // monitor (VGA mode 13h)
    Monitor monitor{};

    // ~1000 fps - ~1.00ms per tick -  1193
    Timer(1193);

    // memory
    MemoryManager memoryManager(mbd);

    // randomizer
    Random random{};

    // keyboard
    Keyboard keyboard{};
    
    // mouse
    Mouse mouse{};

    // monitor.SetMode(Monitor::Mode::Graphics);

    while(1)
    {
        // for(int y = 0; y < ScreenHeight(); y++)
        //     for(int x = 0; x < ScreenWidth(); x++)
        //         Draw(x, y, rand() % 256);
        
        // Clear(7);
        
        // uint8_t col = 5;

        // if(mouse.GetButton(0))
        //     col = 4;
            
        // if(mouse.GetButton(1))
        //     col = 3;
        
        // if(mouse.GetButton(2))
        //     col = 2;

        // FillCircle(mouse.GetMouseX(), mouse.GetMouseY(), 5, col);

        // printf("%d %d %d %d %d\n", mouse.GetMouseX(), mouse.GetMouseY(), mouse.GetButton(0), mouse.GetButton(1), mouse.GetButton(2));
        monitor.Swap();
    }



}
