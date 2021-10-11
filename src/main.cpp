#include "multiboot.h"

#include "types.h"
#include "monitor.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "memory.h"


using namespace MorOS;




uint8_t hor_regs [] = { 0x0,  0x1,  0x2,  0x3,  0x4, 0x5,  0x13 };

uint8_t width_256[] = { 0x5f, 0x3f, 0x40, 0x82, 0x4a, 0x9a, 0x20 };
uint8_t width_320[] = { 0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0x28 };
uint8_t width_360[] = { 0x6b, 0x59, 0x5a, 0x8e, 0x5e, 0x8a, 0x2d };
uint8_t width_376[] = { 0x6e, 0x5d, 0x5e, 0x91, 0x62, 0x8f, 0x2f };
uint8_t width_400[] = { 0x70, 0x63, 0x64, 0x92, 0x65, 0x82, 0x32 };

uint8_t ver_regs  [] = { 0x6,  0x7,  0x9,  0x10, 0x11, 0x12, 0x15, 0x16 };

uint8_t height_200[] = { 0xbf, 0x1f, 0x41, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
uint8_t height_224[] = { 0x0b, 0x3e, 0x41, 0xda, 0x9c, 0xbf, 0xc7, 0x04 };
uint8_t height_240[] = { 0x0d, 0x3e, 0x41, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
uint8_t height_256[] = { 0x23, 0xb2, 0x61, 0x0a, 0xac, 0xff, 0x07, 0x1a };
uint8_t height_270[] = { 0x30, 0xf0, 0x61, 0x20, 0xa9, 0x1b, 0x1f, 0x2f };
uint8_t height_300[] = { 0x70, 0xf0, 0x61, 0x5b, 0x8c, 0x57, 0x58, 0x70 };
uint8_t height_360[] = { 0xbf, 0x1f, 0x40, 0x88, 0x85, 0x67, 0x6d, 0xba };
uint8_t height_400[] = { 0xbf, 0x1f, 0x40, 0x9c, 0x8e, 0x8f, 0x96, 0xb9 };
uint8_t height_480[] = { 0x0d, 0x3e, 0x40, 0xea, 0xac, 0xdf, 0xe7, 0x06 };
uint8_t height_564[] = { 0x62, 0xf0, 0x60, 0x37, 0x89, 0x33, 0x3c, 0x5c };
uint8_t height_600[] = { 0x70, 0xf0, 0x60, 0x5b, 0x8c, 0x57, 0x58, 0x70 };

// the chain4 parameter should be 1 for normal 13h-type mode, but //
// only allows 320x200 256x200, 256x240 and 256x256 because you
// can only access the first 64kb

// if chain4 is 0, then plane mode is used (tweaked modes), and
// you'll need to switch planes to access the whole screen but
// that allows you using any resolution, up to 400x600
// returns 1=ok, 0=fail
int init_graph_vga(int width, int height,int chain4) 
{
    const uint8_t *w,*h;
    uint8_t val = 0x63;
    int a;

    switch(width) {
        case 256: w=width_256; val += 0x00; break;
        case 320: w=width_320; val += 0x00; break;
        case 360: w=width_360; val += 0x04; break;
        case 376: w=width_376; val += 0x04; break;
        case 400: w=width_400; val += 0x04; break;
        default: return 0; // fail
    }

    switch(height) {
        case 200: h=height_200; val |= 0x00; break;
        case 224: h=height_224; val |= 0x80; break;
        case 240: h=height_240; val |= 0x80; break;
        case 256: h=height_256; val |= 0x80; break;
        case 270: h=height_270; val |= 0x80; break;
        case 300: h=height_300; val |= 0x80; break;
        case 360: h=height_360; val |= 0x80; break;
        case 400: h=height_400; val |= 0x80; break;
        case 480: h=height_480; val |= 0x80; break;
        case 564: h=height_564; val |= 0x80; break;
        case 600: h=height_600; val |= 0x80; break;
        default: return 0; // fail
    }

   // chain4 not available if mode takes over 64k

   if(chain4 && (long)width*(long)height>65536L) return 0; 

   // here goes the actual modeswitch

   outb(0x3c2,val);
   outw(0x3d4,0x0e11); // enable regs 0-7

   for(a = 0; a < sizeof(hor_regs); ++a)
        outw(0x3d4, (uint16_t)((w[a]<<8)+hor_regs[a]));

   for(a = 0;a < sizeof(ver_regs); ++a)
        outw(0x3d4,(uint16_t)((h[a]<<8)+ver_regs[a]));

   outw(0x3d4,0x0008); // vert.panning = 0

    if(chain4)
    {
        outw(0x3d4,0x4014);
        outw(0x3d4,0xa317);
        outw(0x3c4,0x0e04);
    }
    else
    {
        outw(0x3d4,0x0014);
        outw(0x3d4,0xe317);
        outw(0x3c4,0x0604);
    }

   outw(0x3c4,0x0101);
   outw(0x3c4,0x0f02); // enable writing to all planes
   outw(0x3ce,0x4005); // 256color mode
   outw(0x3ce,0x0506); // graph mode & A000-AFFF

   inb(0x3da);
   outb(0x3c0,0x30); outb(0x3c0,0x41);
   outb(0x3c0,0x33); outb(0x3c0,0x00);

   for(a=0;a<16;a++) {    // ega pal
      outb(0x3c0,(int8_t)a); 
      outb(0x3c0,(int8_t)a); 
   } 
   
   outb(0x3c0, 0x20); // enable video

   return 1;
}

constexpr uint32_t ScreenWidth = 256;
constexpr uint32_t ScreenHeight = 240;



void Draw(int32_t x, int32_t y, uint8_t col)
{
    if(x < 0 || y < 0 || x >= ScreenWidth || y >= ScreenHeight )
        return;
    
    uint8_t* pixels = (uint8_t*)0x000a0000;
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
    // our global monitor
    MorOS::Monitor monitor{};

    MorOS::GlobalDescriptorTable gdt{};
    MorOS::InterruptManager interruptManager{};

    multiboot_memory_map_t* mmmt = 0;
    
    // cycle through the memory map in search of the memory chunk at 1MiB
    for(uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        mmmt = (multiboot_memory_map_t*)(mbd->mmap_addr + i);

        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmmt->addr_high == 0 &&
            mmmt->addr_low  == 0x100000)
        {
            break;
        }
    }

    // if we have a memory chunk at 1MiB, let's totally use it!
    if(mmmt != 0)
        MorOS::MemoryManager memoryManager(mmmt->addr_low, mmmt->len_low);
    
 
 
    init_graph_vga(ScreenWidth, ScreenHeight, 1);

    FillRect(0, 0, ScreenWidth, ScreenHeight, 0);
    FillRect(5, 5, ScreenWidth-10, ScreenHeight-10, 4);
    FillRect(10, 10, ScreenWidth-20, ScreenHeight-20, 3);
    FillCircle(ScreenWidth / 2, ScreenHeight / 2, 50, 4);

    // MorOS::Timer(100);
    
    
    // just chill out forever!
    while(1);
}