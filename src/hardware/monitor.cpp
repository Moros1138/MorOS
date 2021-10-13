#include "hardware/monitor.h"
#include "hardware/timer.h"

namespace MorOS
{
    Monitor* Monitor::activeMonitor = 0;

    Monitor::Monitor()
    {
        Monitor::activeMonitor = this;
        
        // video ram buffer
        buffer      = (uint16_t*)0xb8000;
        
        // init cursor
        outb(VGA_COMM_PORT, VGA_SET_CRSR_MAX_SCAN);   // set maximum scan line register to 15
        outb(VGA_DATA_PORT , 15);

        outb(VGA_COMM_PORT, VGA_SET_CRSR_END);   // set the cursor end line to 15
        outb(VGA_DATA_PORT , 15);

        outb(VGA_COMM_PORT, VGA_SET_CRSR_START);   // set the cursor start line to 14 and enable cursor visibility
        outb(VGA_DATA_PORT , 14);

        // initialize trackers
        cursor_x    = 0;
        cursor_y    = 0;
        index       = 0;
        
        // default to (BG: black FG: grey)
        attribute   = 0x07;
        
        // clear the screen
        clear();
        move_cursor();
    }

    Monitor::~Monitor()
    { }
    
    void Monitor::switchTo13h()
    {
        // a large amount of data is written to port 0x03d4, let's batch it all together
        uint16_t words0x03d4[] = {
            0x0e11, // disable write protection on registers 0-7 of 0x3d4
            0x5f00, 0x4f01, 0x5002, 0x8203, 0x5404, 0x8005, 0x2813, // horizontal registers
            0xbf06, 0x1f07, 0x4109, 0x9c10, 0x8e11, 0x8f12, 0x9615, 0xb916, // vertical registers
            0x0008, // vert.panning = 0
            0x4014, 0xa317, // chain 4
        };

        uint16_t words0x03c4[] = {
            0x0e04, // Set Memory Mode
            0x0101, // Set Clock Mode
            0x0f02  // Map Mask Register -> enable writing to all planes
        };
        
        // disable interrupts
        asm("cli;");

        // MISCELLANEOUS OUTPUT REGISTER (0x03c2)
        // 
        //  /--------------------- VSYNC Polarity, 0 = positive retrace sync pulse
        //  | 
        //  | /------------------- HSYNC Polarity, 1 = negative retrace sync pulse
        //  | |
        //  | | /----------------- Odd/Even Page Select, 1 selects the high page
        //  | | |
        //  | | |   /--+---------- Clock Select 00 = select 25 Mhz clock
        //  | | |   | /            25 Mhz is used for 320/640 pixel wide modes.
        //  | | |   | | 
        //  | | |   | | /--------- RAM Enable, 1 = enable address decode for the
        //  | | |   | | |          display buffer system.
        //  | | |   | | |         
        //  | | |   | | | /------- I/O Address Select, 1 = compatible with color
        //  | | | x | | | |        graphics adapters.
        //  | | | | | | | |
        //  0 1 1 0 0 0 1 1 --> 0x63
        outb(0x3c2,0x63);
        
        // write words to port 0x03d4
        for(int a = 0; a < sizeof(words0x03d4); ++a)
            outw(0x3d4, words0x03d4[a]);
        
        // write words to port 0x03c4
        for(int a = 0; a < sizeof(words0x03c4); ++a)
            outw(0x03c4, words0x03c4[a]);
        

        // GRAPHICS MODE REGISTER (0x03ce) index 0x05
        // 
        //    /------------------- 256-Color Shift Mode, 1 = enable 256 color mode
        //    |  
        //    | /----------------- Shift Register Interleave Mode
        //    | |  
        //    | | /--------------- Host Odd/Even Memory Read Addressing Enable
        //    | | |                
        //    | | | /------------- Read Mode, 0b = use REad Mode 0
        //    | | | |              
        //    | | | |   /--+------ Write Mode, 00b = use Write Mode 0
        //  x | | | | x | /
        //  | | | | | | | |
        //  0 1 0 0 0 0 0 0 -> 0x40
        outw(0x3ce,0x4005);

        // MISCELLANEOUS GRAPHICS REGISTER (0x03ce) index 0x06
        // 
        //          /---------+--- Memory Map Select
        //          |         |
        //          | /-------/    01b = A0000h-AFFFFh (64K region)
        //          | | 
        //          | | /--------- Chain Odd/Even Enable
        //          | | |         
        //  x x x x | | | /------- Alphanumeric Mode Disable, 1 = graphics mode
        //  | | | | | | | |
        //  0 0 0 0 0 1 0 1 -> 0x05
        outw(0x3ce,0x0506);

        // Reading from port 0x03da puts port 0x03c0 into indexing mode.
        inb(0x3da);

        // index = 0x30  --  data = 0x41
        outb(0x3c0,0x30); outb(0x3c0,0x41);
        
        // index = 0x33  --  data = 0x00
        outb(0x3c0,0x33); outb(0x3c0,0x00);

        // ega pal
        for(int a = 0; a < 16; a++)
        {
            // index = a          --  data = a
            outb(0x3c0,(int8_t)a); outb(0x3c0,(int8_t)a); 
        }

        // configure palette with 8-bit RRRGGGBB color
        outb(0x3C6, 0xFF);
        outb(0x3C8, 0);
        for (uint8_t i = 0; i < 255; i++) {
            outb(0x3C9, (((i >> 5) & 0x7) * (256 / 8)) / 4);
            outb(0x3C9, (((i >> 2) & 0x7) * (256 / 8)) / 4);
            outb(0x3C9, (((i >> 0) & 0x3) * (256 / 4)) / 4);
        }

        // set color 255 = white
        outb(0x3C9, 0x3F);
        outb(0x3C9, 0x3F);
        outb(0x3C9, 0x3F);

        outb(0x3c0, 0x20); // enable video

        // enable interrupts
        asm("sti;");
        
        // clear the screen
        MorOS::memset((uint8_t*)0xA0000, 0, 64000);
    }

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
        outb(VGA_COMM_PORT, VGA_SET_CRSR_START);
        outb(VGA_DATA_PORT, (state) ? 0x0d : 0x2d);
    }
    
    void Monitor::set_color(uint8_t fg, uint8_t bg)
    {
        attribute = fg | (bg << 8);
    }
    
    void Monitor::move_cursor()
    {
        // Tell the VGA board we are setting the high cursor byte.
        outb(VGA_COMM_PORT, VGA_SET_CRSR_HIGH_BYTE);

        // Send the high cursor byte.
        outb(VGA_DATA_PORT, index >> 8);
        
        // Tell the VGA board we are setting the low cursor byte.
        outb(VGA_COMM_PORT, VGA_SET_CRSR_LOW_BYTE);
        
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
