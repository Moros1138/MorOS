#include <utils.h>

extern "C" void _putchar(char character)
{
    MorOS::Terminal::activeTerminal->PrintChar(character);
}

namespace MorOS
{
    
    void itoa(char* buf, int base, int value)
    {
        unsigned int ui = (unsigned int)value;
        int divisor = 0;
        char* p = buf;
        
        switch(base)
        {
            case 2:
            case 'b':
            case 'B':
                divisor = 2;
                break;
            case 10:
            case 'd':
            case 'D':
                divisor = 10;
                if(value < 0)
                {
                    *p++ = '-';
                    ui = -value;
                }
                break;
            case 16:
            case 'x':
            case 'X':
                divisor = 16;
                break;
            case 'u':
            case 'U':
                divisor = 10;
                break;
        }
        
        do
        {
            int remainder = ui % divisor;
            *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
        } while (ui /= divisor);
        
        // terminate buffer
        *p = '\0';
        
        char* p1 = buf;
        char* p2 = p -1;
        
        while((size_t)p1 < (size_t)p2)
        {
            char tmp = *p1;
            *p1 = *p2;
            *p2 = tmp;
            p1++;
            p2--;
        }
    }

} // MorOS
