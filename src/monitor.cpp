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

} // MorOS