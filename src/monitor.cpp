#include "monitor.h"

namespace MorOS
{
    Monitor* Monitor::activeMonitor = 0;

    Monitor::Monitor()
    {
        Monitor::activeMonitor = this;

        // video ram buffer
        buffer      = (uint16_t*)0xb8000;
        
        // init cursor
        outb(0x3D4, 0x09);   // set maximum scan line register to 15
        outb(0x3D5, 0x0F);

        outb(0x3D4, 0x0B);   // set the cursor end line to 15
        outb(0x3D5, 0x0F);

        outb(0x3D4, 0x0A);   // set the cursor start line to 14 and enable cursor visibility
        outb(0x3D5, 0x0E);

        // initialize trackers
        cursor_x    = 0;
        cursor_y    = 0;
        index       = 0;
        
        // default to (BG: black FG: grey)
        attribute   = 0x07;
        
        // clear the screen
        clear();
    }

    Monitor::~Monitor()
    { }

    void Monitor::putc(char ch)
    {
        if(ch == '\n')
        {
            cursor_x = 0;
            cursor_y++;
        }
        else
        {
            // print the character using the current attribute
            buffer[index] = ch | (attribute << 8);
            cursor_x++;
        }
        
        if(cursor_x >= VGA_WIDTH)
        {
            cursor_x = 0;
            cursor_y++;
        }

        if(cursor_y >= VGA_HEIGHT)
        {
            cursor_y = VGA_HEIGHT - 1;
            scroll();
        }

        // update index
        index = cursor_y * VGA_WIDTH + cursor_x;
        move_cursor();
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
    
    void Monitor::clear()
    {
        uint16_t blank = 0x20 | (attribute << 8);
        for(int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
            buffer[i] = blank;
    }
    
    void Monitor::show_cursor(bool state)
    {
        outb(VGA_COMMAND_PORT, VGA_SET_CURSOR_START);
        
        uint8_t data = inb(VGA_DATA_PORT);
        printf("%d %d\n", cursor_x, cursor_y);
        printf("Cursor Data: %u\n", data);
        outb(VGA_DATA_PORT, data);
    }

    void Monitor::move_cursor()
    {
        // Tell the VGA board we are setting the high cursor byte.
        outb(VGA_COMMAND_PORT, VGA_SET_CURSOR_HIGH_BYTE);

        // Send the high cursor byte.
        outb(VGA_DATA_PORT, index >> 8);
        
        // Tell the VGA board we are setting the low cursor byte.
        outb(VGA_COMMAND_PORT, VGA_SET_CURSOR_LOW_BYTE);
        
        // Send the low cursor byte.
        outb(VGA_DATA_PORT, index);
    }

    void Monitor::scroll()
    {
        int i;
        // shift text up by 1 line
        for(i = 0 * VGA_WIDTH; i < (VGA_WIDTH * (VGA_HEIGHT-1)); i++)
            buffer[i] = buffer[i + VGA_WIDTH];

        // clear the last line
        for (i = (VGA_HEIGHT-1)*VGA_WIDTH; i < VGA_HEIGHT*VGA_WIDTH; i++)
           buffer[i] = 0x0720;
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
