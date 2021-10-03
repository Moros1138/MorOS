#ifndef _MOROS_TERMINAL_H
#define _MOROS_TERMINAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <printf.h>
#include <utils.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  25

namespace MorOS
{
    
    class Terminal
    {
    public:
        Terminal();
        ~Terminal();
        
        static Terminal* activeTerminal;

    public:

        // output
        void PrintChar(uint8_t c);
        void PrintString(const char* str);

        // clearing
        void ClearLine(size_t line);
        void ClearScreen();
        
        // scrolling
        void Scroll();
        

        // attribute functions
        void SetColor(uint8_t foreground, uint8_t background);
        void SetAttribute(uint8_t attr);

    private:
        // the video buffer 0xb8000
        uint16_t* buffer;
        
        // current attribute byte
        uint8_t attribute;

        // current position tracking
        size_t cursor;
        size_t col;
        size_t row;
        
        // size of the terminal
        size_t width;
        size_t height;

    };
    
    void clear_screen();
    void putdec(int num, bool unsigned_value = false);
    void puthex(int num);
    void puts(const char* str);
    void putc(const char c);

} // MorOS



#endif // _MOROS_TERMINAL_H
