/*
    believe it or not, these are provided by the compiler
    even though we're not actually using any of the standard
    libraries. saves a lot of hassle and allows me to use
    some types I'm used to using!
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <Terminal.h>
#include <Randomizer.h>

using namespace MorOS;

#define delay(duration) for(int i = 0; i < duration; i++) { __asm__("nop"); }
#define outhex(value) \
    puthex(((uint32_t)value >> 24) & 0xff); \
    puthex(((uint32_t)value >> 16) & 0xff); \
    puthex(((uint32_t)value >>  8) & 0xff); \
    puthex(((uint32_t)value >>  0) & 0xff);

// RANDOMIZER
void srand(uint32_t seed)
{
    Randomizer::activeRandomizer->SetSeed(seed);
}

int rand()
{
    return Randomizer::activeRandomizer->GetInteger();
}

// TERMINAL
void clear_screen()
{
    Terminal::activeTerminal->ClearScreen();
}

void puts(const char* str)
{
    Terminal::activeTerminal->PrintString(str);
}

void putc(const char c)
{
    Terminal::activeTerminal->PrintChar(c);
}

void puthex(uint8_t c)
{
    char digits[] = "0123456789abcdef";

    Terminal::activeTerminal->PrintChar(digits[(c & 0xf0) >> 4]);
    Terminal::activeTerminal->PrintChar(digits[(c & 0x0f)]);
}

void putdec(int input)
{
    bool negative = (input < 0);
    char buf[256] = { 0 };
    char digits[] = "0123456789";

    int length = 0;
    int temp;

    // negate if negative value
    temp = negative ? -input : input;

    // determine number of digits
    while(temp /= 10)
        length++;
    
    // negate if negative value
    temp = negative ? -input : input;
    
    // prefix output with -
    if(negative)
        buf[0] = '-';
        
    for(int index = length; index >= 0; index--)
    {
        if(negative)
            buf[index+1] = digits[temp % 10];
        else
            buf[index] = digits[temp % 10];
        
        temp /= 10;
    }
    
    Terminal::activeTerminal->PrintString(buf);
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

void strcpy(char* dest, char* src)
{
    size_t index=0;
    while(src[index])
    {
        dest[index] = src[index];
        index++;
    }
    dest[index] = '\0';
}

/*
    this is labelled as extern "C" to prevent the C++ compiler from
    mangling the function name. this allows it to be called from
    the boot.s this doesn't stop us from using C++ features within
    this function, however. I'll have fun with this later on, I'm
    certain of it.
*/
extern "C" void kmain(void* multiboot)
{
    // our global terminal
    Terminal terminal{};

    // our global randomizer
    Randomizer randomizer{};

    // memory information provided by mutliboot specification
    size_t max_memory = *(uint32_t*)(((size_t)multiboot) + 8) * 1024;
    
    // 10 MiB pre-calculated
    size_t heap_size = 10 * 1024 * 1024;

    // basically, give me 10 MiB of ram, pretty please
    uint8_t* heap_memory = (uint8_t*)(max_memory - heap_size - 10*1024);

    puts("MorOS - Max  Memory: ");
    outhex(max_memory);
    puts(" - "); putdec(max_memory); puts("\n");

    puts("MorOS - Heap Memory: ");
    outhex(heap_memory);
    puts(" - "); putdec((int)heap_memory); puts("\n");

    puts("MorOS - Heap Size:   ");
    outhex(heap_size);
    puts(" - "); putdec((int)heap_size); puts("\n");

    char buf[] = "Hello, World!\n";
    strcpy((char*)heap_memory, buf);

    puts("\nThe following is contained within the heap memory:\n\n");
    puts((char*)heap_memory);
    puts("\n");

    // let's hang out here forever!
    while(1)
    { 
    }
}