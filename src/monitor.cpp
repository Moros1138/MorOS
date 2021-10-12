#include "monitor.h"

namespace MorOS
{
    Monitor* Monitor::activeMonitor = 0;

    Monitor::Monitor()
    {
        Monitor::activeMonitor = this;

        // a large amount of data is written to port 0x03d4, let's batch it all together
        uint16_t words[] = {
            0x0e11, // enable regs 0-7
            0x5f00, 0x4f01, 0x5002, 0x8203, 0x5404, 0x8005, 0x2813, // horizontal registers
            0xbf06, 0x1f07, 0x4109, 0x9c10, 0x8e11, 0x8f12, 0x9615, 0xb916, // vertical registers
            0x0008, // vert.panning = 0
            0x4014, 0xa317, // chain 4
        };
        
        // start actually changing the video mode
        outb(0x3c2,0x63);
        
        // write words to port 0x03d4
        for(int a = 0; a < sizeof(words); ++a)
            outw(0x3d4, words[a]);

        outw(0x3c4,0x0e04); outw(0x3c4,0x0101); outw(0x3c4,0x0f02); // enable writing to all planes

        outw(0x3ce,0x4005); // 256color mode
        outw(0x3ce,0x0506); // graph mode & A000-AFFF

        inb(0x3da);
        outb(0x3c0,0x30); outb(0x3c0,0x41); outb(0x3c0,0x33); outb(0x3c0,0x00);

        // ega pal
        for(int a = 0; a < 16; a++)
        {
            outb(0x3c0,(int8_t)a); outb(0x3c0,(int8_t)a); 
        }

        // clear to black
        memset((uint8_t*)0xA0000, 0, 64000);
    
        outb(0x3c0, 0x20); // enable video
    }

    Monitor::~Monitor()
    { }

    void Monitor::putc(char ch)
    {
        // if(ch == '\n')
        // {
        //     cursor_x = 0;
        //     cursor_y++;
        // }
        // else
        // {
        //     // print the character using the current attribute
        //     buffer[index] = ch | (attribute << 8);
        //     cursor_x++;
        // }
        
        // if(cursor_x >= VGA_WIDTH)
        // {
        //     cursor_x = 0;
        //     cursor_y++;
        // }

        // if(cursor_y >= VGA_HEIGHT)
        // {
        //     cursor_y = VGA_HEIGHT - 1;
        //     scroll();
        // }

        // // update index
        // index = cursor_y * VGA_WIDTH + cursor_x;
        // move_cursor();
    }

    void Monitor::puts(char* str)
    {
        size_t len = 0;
        while(str[len] != 0)
        {
            putc(str[len++]);
        }
    }
    
    void Monitor::putdec(uint32_t num, bool bSigned)
    {
        if(num == 0)
        {
            puts("0");
            return;
        }
        
        // buffers are large enough to hold a signed int32_t
        char buf[12] = { 0 };
        char buf2[12] = { 0 };
        
        uint32_t acc = num;

        int i = 0;

        while(acc > 0)
        {
            buf[i] = '0' + acc % 10;
            acc /= 10; i++;
        }
        
        buf[i] = 0;
        
        int j = 0;
        while(i >= 0)
        {
            buf2[j++] = buf[--i];
        }

        puts(buf2);
    }
    
    void Monitor::puthex(uint32_t num)
    {
        if(num == 0)
        {
            puts("00000000");
            return;
        }

        char buf[9] = { 0 };

        int index = 0;
        for(int i = 28; i >= 0; i -= 4)
        {
            uint8_t temp = (num >> i) & 0x0f;
            buf[index++] = (temp > 9) ? temp + 'a' - 10 : temp + '0';
        }
        
        puts(buf);
    }
    
    
    void printf(char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        #define process_fmt(p, c, func, t) if(fmt[p+1] == c) { t value = va_arg(args, t); Monitor::activeMonitor->func(value); p += 2; continue; }
        
        size_t pos = 0;

        while(fmt[pos] != 0)
        {
            if(fmt[pos] == '%')
            {
                process_fmt(pos, 'c', putc, int);
                process_fmt(pos, 'd', putdec, uint32_t);
                process_fmt(pos, 'u', putdec, uint32_t);
                process_fmt(pos, 'x', puthex, uint32_t);
            }
            
            Monitor::activeMonitor->putc(fmt[pos++]);
        }
        
        #undef process_fmt

        va_end(args);
    }

} // MorOS
