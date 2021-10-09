#include "monitor.h"

namespace MorOS
{
    Monitor* Monitor::activeMonitor = 0;

    Monitor::Monitor()
    {
        Monitor::activeMonitor = this;

        // video ram buffer
        buffer      = (uint16_t*)0xb8000;
        
        // initialize trackers
        cursor_x    = 0;
        cursor_y    = 0;
        index       = 0;
        
        // default to (BG: black FG: grey)
        attribute   = 0x07;
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
            
        char buf[32] = { 0 };
        char buf2[32] = { 0 };
        
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
        for(i = 0 * VGA_WIDTH; i < (VGA_WIDTH * (VGA_HEIGHT-1)); i++)
        {
            buffer[i] = buffer[i + VGA_WIDTH];
        }

       for (i = (VGA_HEIGHT-1)*VGA_WIDTH; i < VGA_HEIGHT*VGA_WIDTH; i++)
       {
           buffer[i] = 0x0720;
       }
    }

    void printf(char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        size_t pos = 0;

        while(fmt[pos] != 0)
        {
            if(fmt[pos] == '%')
            {
                if(fmt[pos+1] == 'd')
                {
                    int32_t value = va_arg(args, int32_t);
                    Monitor::activeMonitor->putdec(value);
                    pos += 2;
                    continue;
                }

                if(fmt[pos+1] == 'u')
                {
                    uint32_t value = va_arg(args, uint32_t);
                    Monitor::activeMonitor->putdec(value, true);
                    pos += 2;
                    continue;
                }
                if(fmt[pos+1] == 'x')
                {
                    uint32_t value = va_arg(args, uint32_t);
                    Monitor::activeMonitor->puthex(value);
                    pos += 2;
                    continue;
                }
            }
            Monitor::activeMonitor->putc(fmt[pos++]);
        }
        
        

        va_end(args);
    }

} // MorOS