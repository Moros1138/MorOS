#include "hardware/monitor.h"
#include "hardware/timer.h"

namespace MorOS
{
    Monitor* Monitor::activeMonitor = 0;

    // Attribute Controller Registers
    constexpr uint16_t VGA_AC_INDEX         = 0x03c0;
    constexpr uint16_t VGA_AC_READ          = 0x03c1;
    constexpr uint16_t VGA_AC_WRITE         = 0x03c0;

    // Miscellaneous Output
    constexpr uint16_t VGA_MISC_READ        = 0x03cc;
    constexpr uint16_t VGA_MISC_WRITE       = 0x03c2;

    // Sequencer Registers
    constexpr uint16_t VGA_SEQ_INDEX        = 0x03C4;
    constexpr uint16_t VGA_SEQ_DATA         = 0x03C5;

    // VGA Color Palette Registers
    constexpr uint16_t VGA_DAC_READ_INDEX   = 0x03C7;
    constexpr uint16_t VGA_DAC_WRITE_INDEX  = 0x03C8;
    constexpr uint16_t VGA_DAC_DATA         = 0x03C9;

    // Graphics Controller Registers
    constexpr uint16_t VGA_GC_INDEX         = 0x03CE;
    constexpr uint16_t VGA_GC_DATA          = 0x03CF;

    // CRT Controller Registers
    constexpr uint16_t VGA_CRTC_INDEX       = 0x03D4;
    constexpr uint16_t VGA_CRTC_DATA        = 0x03D5;

    // General Control and Status Registers
    constexpr uint16_t VGA_INSTAT_READ      = 0x03DA;

    Monitor::Monitor()
    {
        Monitor::activeMonitor = this;
        
        // video ram buffer
        for(size_t i = 0; i < 2000; i++)
            pTextBuffer[i] = 0x0720;
        
        memset(pGraphicsBufffer, 0, 64000);

        // set maximum scan line register to 15
        outb(VGA_CRTC_INDEX, 0x09); outb(VGA_CRTC_DATA, 15);
        
        // set the cursor end line to 15
        outb(VGA_CRTC_INDEX, 0x0b); outb(VGA_CRTC_DATA, 15);
        // set the cursor start line to 14 and enable cursor visibility
        outb(VGA_CRTC_INDEX, 0x0a); outb(VGA_CRTC_DATA, 14);

        // initialize trackers
        cursor_x    = 0;
        cursor_y    = 0;
        index       = 0;
        
        // default to (BG: black FG: grey)
        attribute   = 0x07;
        
        // set the mode for our tracking purposes
        SetMode(Mode::Text);

        // clear the screen
        clear();
        move_cursor();
        
        retrace = 0;
        prevRetrace = 0;

        // swap the buffer to the screen
        Swap();
    }

    Monitor::~Monitor()
    { }
    
    void Monitor::SetMode(Monitor::Mode m)
    {
        mode = m;
        if(mode == Monitor::Mode::Text)
        {
            width = 80;
            height = 25;
        }

        if(mode == Monitor::Mode::Graphics)
        {
            width = 320;
            height = 200;
            switchTo13h();
        }
    }

    void Monitor::Swap()
    {
        prevRetrace = retrace;
        retrace = inb(VGA_INSTAT_READ) & 0x8;
        
        bool flag = !prevRetrace && retrace;

        if(mode == Monitor::Mode::Text && flag)
        {
            uint16_t* pTemp = (uint16_t*)0xB8000;
            for(size_t i = 0; i < 2000; i++)
                pTemp[i] = pTextBuffer[i];
        }
            
        if(mode == Monitor::Mode::Graphics && flag)
        {
            uint8_t* pTemp = (uint8_t*)0xA0000;
            for(size_t i = 0; i < 64000; i++)
                pTemp[i] = pGraphicsBufffer[i];
        }
    }

    int32_t Monitor::Width()
    {
        return (mode == Monitor::Mode::Text) ? 80 : 320;
    }

    int32_t Monitor::Height()
    {
        return (mode == Monitor::Mode::Text) ? 25 : 200;
    }

    void Monitor::switchTo13h()
    {
        uint8_t seq_data[5]     = { 0x03, 0x01, 0x0F, 0x00, 0x0E };
        uint8_t crtc_data[25]   = { 0x5F, 0x4F, 0x50, 0x82,  0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 0xFF };
        uint8_t gc_data[9]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF };
        uint8_t ac_data[21]     = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00, 0x00 };

        // set misc register
        outb(VGA_MISC_WRITE, 0x63);

        // set sequence data
        for(uint8_t index = 0; index < 5; index++)
        {
            // select index
            outb(VGA_SEQ_INDEX, index);
            // write data at that index
            outb(VGA_SEQ_DATA, seq_data[index]);
        }

        // write crtc data
        for(uint8_t index = 0; index < 25; index++)
        {
            outb(VGA_CRTC_INDEX, index);
            outb(VGA_CRTC_DATA, crtc_data[index]);
        }        
  
        // write graphics controller data
        for(uint8_t index = 0; index < 9; index++)
        {
            outb(VGA_GC_INDEX, index);
            outb(VGA_GC_DATA, gc_data[index]);
        }

        // write ac data
        for(uint8_t index = 0; index < 21; index++)
        {
            outb(VGA_AC_INDEX, index);
            outb(VGA_AC_WRITE, ac_data[index]);
        }
        
        uint8_t d = inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, d | 0x20);
    }

    uint8_t* Monitor::GetBuffer()
    {
        return pGraphicsBufffer;
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
            pTextBuffer[index] = ch | (attribute << 8);
            cursor_x++;
        }
        
        if(cursor_x >= 80)
        {
            cursor_x = 0;
            cursor_y++;
        }

        if(cursor_y >= 25)
        {
            cursor_y = 24;
            scroll();
        }

        // update index
        index = cursor_y * 80 + cursor_x;
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
    
    void Monitor::clear()
    {
        if(mode == Monitor::Mode::Text)
        {
            for(size_t i = 0; i < 2000; i++)
                pTextBuffer[i] = 0x20 | (attribute << 8);
        }

        if(mode == Monitor::Mode::Graphics)
        {
            memset(pGraphicsBufffer, 0, 64000);
        }
    }
    
    void Monitor::show_cursor(bool state)
    {
        // read cursor start register
        outb(VGA_CRTC_INDEX, 0x0a);
        uint8_t d = inb(VGA_CRTC_DATA);

        // update cursor disable bit
        outb(VGA_CRTC_INDEX, 0x0a);
        if(state)
            outb(VGA_CRTC_DATA, d | 0x20);
        else
            outb(VGA_CRTC_DATA, d & 0xdf);
    }
    
    void Monitor::set_color(uint8_t fg, uint8_t bg)
    {
        attribute = fg | (bg << 8);
    }
    
    void Monitor::move_cursor()
    {
        if(mode != Monitor::Mode::Text)
            return;

        // Tell the VGA board we are setting the high cursor byte.
        outb(0x03d4, 0x0e); outb(0x03d5, index >> 8);
        
        // Tell the VGA board we are setting the low cursor byte.
        outb(0x03d4, 0x0f); outb(0x03d5, index);
    }

    void Monitor::scroll()
    {
        size_t i;
        // shift text up by 1 line
        for(i = 0 * 80; i < 1920; i++)
            pTextBuffer[i] = pTextBuffer[i + 80];

        // clear the last line
        for (i = 1920; i < 2000; i++)
           pTextBuffer[i] = 0x0720;
    }
    
    void printf(char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        size_t pos = 0;
        
        char buf1[32];
        char buf2[32];

        while(fmt[pos] != 0)
        {
            if(fmt[pos] == '%')
            {
                // zero out the buffers before we use them
                memset(buf1, 0, 32);
                memset(buf2, 0, 32);

                // handle "%c"
                if(fmt[pos+1] == 'c')
                {
                    int value = va_arg(args, int);
                    Monitor::activeMonitor->putc(value);
                    pos += 2;
                    continue;
                }

                // handle "%d" or "%u"
                if(fmt[pos+1] == 'd' || fmt[pos+1] == 'u')
                {
                    bool is_signed = (fmt[pos+1] == 'd') ? true : false;
                    uint32_t value = va_arg(args, uint32_t);

                    if(value == 0)
                    {
                        Monitor::activeMonitor->puts("0");
                        pos += 2;
                        continue;
                    }

                    // buffer indexes
                    size_t i = 0;
                    size_t j = 0;

                    uint32_t acc = value;

                    // if we have a signed, negative value, invert it
                    if(is_signed && (int32_t)value < 0)
                        acc = (uint32_t)(-((int32_t)value));

                    // convert each digit to an ascii char of the appropriate value
                    while(acc > 0)
                    {
                        buf1[i++] = '0' + (acc % 10);
                        acc /= 10;
                    }
                    
                    // if we have a signed, negative value, add minus sign
                    if(is_signed && (int32_t)value < 0)
                        buf1[i++] = '-';

                    // null terminated string!
                    buf1[i] = 0;

                    // reverse the buffer
                    while(i > 0)
                    {
                        buf2[j++] = buf1[--i];
                    }
                    
                    // output value to screen
                    Monitor::activeMonitor->puts(buf2);

                    pos += 2;
                    continue;
                }

                // handle "%x"
                if(fmt[pos+1] == 'x')
                {
                    uint32_t value = va_arg(args, uint32_t);
                    
                    // stop here if we're dealing with a 0
                    if(value == 0)
                    {
                        Monitor::activeMonitor->puts("00000000");
                        pos += 2;
                        continue;
                    }

                    int index = 0;
                    for(int i = 28; i >= 0; i -= 4)
                    {
                        uint8_t temp = (value >> i) & 0x0f;
                        buf1[index++] = (temp > 9) ? temp + 'a' - 10 : temp + '0';
                    }
                    
                    Monitor::activeMonitor->puts(buf1);
                    
                    pos += 2;
                    continue;
                }
                
            }
            
            Monitor::activeMonitor->putc(fmt[pos++]);
        }
        
        va_end(args);
    }

} // MorOS
