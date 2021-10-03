#include <Terminal.h>

namespace MorOS
{

    Terminal* Terminal::activeTerminal = 0;

    Terminal::Terminal()
    {
        Terminal::activeTerminal = this;

        // the video buffer 0xb8000
        buffer = (uint16_t*)0xb8000;
        
        // set color, gray foreground, black background
        SetColor(7, 0);
        
        // fill screen with spaces
        ClearScreen();

        // current position tracking
        cursor = 0;
        col = 0;
        row = 0;
        
        // size of the terminal
        width = VGA_WIDTH;
        height = VGA_HEIGHT;
    }

    Terminal::~Terminal()
    {

    }

    void Terminal::ClearLine(size_t line)
    {
        for(size_t i = 0; i < width; i++)
            buffer[line * width + i] = (attribute << 8) | ' ';
    }

    void Terminal::Scroll()
    {
        for(size_t y = 1; y < height; y++)
        {
            for(size_t x = 0; x < width; x++)
            {
                buffer[((y - 1) * width) + x] = buffer[(y * width) + x];
            }
        }
    }

    void Terminal::PrintChar(uint8_t c)
    {
        // handle new-line
        if(c == '\n')
        {
            col = 0;

            if(++row == height)
            {
                // scroll screen contents up
                Scroll();
                
                // reset row to last line
                row = height - 1;
                
                // clear the last line
                ClearLine(row);
            }

            // update cursor location
            cursor = width * row;

            return;
        } // c == '\n'

        // would we scroll?
        size_t y1 = cursor / width;
        size_t y2 = (cursor + 1) / width;
        
        if(y2 != y1)
        {
            if(row++ == height)
            {
                Scroll();

                row = height - 1;
                
                ClearLine(row);
            }

            cursor = width * row;
        }
        // y * width + x

        // x cursor % width
        // y cursor / width

        buffer[cursor++] = (attribute << 8) | c;
        
        col = cursor % width;
        row = cursor / width;
    }

    void Terminal::PrintString(const char* str)
    {
        for(size_t index = 0; str[index] != '\0'; index++)
            PrintChar(str[index]);
    }

    void Terminal::ClearScreen()
    {
        cursor = 0;
        size_t end = width * height;

        for(size_t index = 0; index < end; index++)
        {
            buffer[index] = (attribute << 8) | ' ';
        }
            
    }

    void Terminal::SetColor(uint8_t foreground, uint8_t background)
    {
        SetAttribute((background & 0xF0) << 8 | (foreground & 0x0F));
    }

    void Terminal::SetAttribute(uint8_t attr)
    {
        attribute = attr;
    }

    void clear_screen()
    {
        Terminal::activeTerminal->ClearScreen();
    }

    void putdec(int num, bool unsigned_value)
    {
        char buf[256];
        itoa(buf, (unsigned_value) ? 'u' : 'd', num);
        Terminal::activeTerminal->PrintString(buf);
    }

    void puthex(int num)
    {
        char buf[9] = { 0 };
        char* p = buf;

        for(int bits = 28; bits >= 0; bits -= 4)
        {
            *p = (num >> bits) & 0xf;
            *p = (*p < 10) ? *p + '0' : *p + 'a' - 10;
            ++p;
        }
        
        *p = '\0';

        Terminal::activeTerminal->PrintString(buf);
    }

    void puts(const char* str)
    {
        Terminal::activeTerminal->PrintString(str);
    }

    void putc(const char c)
    {
        Terminal::activeTerminal->PrintChar(c);
    }

} // MorOS


