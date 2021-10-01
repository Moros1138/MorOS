/*
    believe it or not, these are provided by the compiler
    even though we're not actually using any of the standard
    libraries. saves a lot of hassle and allows me to use
    some types I'm used to using!
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
    this is labelled as extern "C" to prevent the C++ compiler from
    mangling the function name. this allows it to be called from
    the boot.s this doesn't stop us from using C++ features within
    this function, however. I'll have fun with this later on, I'm
    certain of it.
*/
extern "C" void kmain(void)
{
    
    /*
        at this point of the multiboot specs, we are in
        the text mode 80x25, 16 color mode.
    
        the screen buffer is located at 0xb8000

        each screen entry contains the following:

            BYTE color
                high nibble = background color
                low nibble = foreground color
            
            BYTE displayed_character
                the character displayed on the screen
    
    */
    uint16_t* screen_buffer = (uint16_t*) 0xb8000;

    // helper size variables
    const size_t screen_width = 80;
    const size_t screen_height = 25;

    // fill the screen with spaces
    for(size_t index = 0; index < (screen_width * screen_height); index++)
        screen_buffer[index] = 0x0f00 | ' ';

    // keep track of our cursor location (primitive, will do better later)
    size_t cursor = 0;

    // print 'H' to the screen with a black background and a white foreground
    screen_buffer[cursor++] = 0x0f00 | 'H';

    screen_buffer[cursor++] = 0x0f00 | 'e';
    screen_buffer[cursor++] = 0x0f00 | 'l';
    screen_buffer[cursor++] = 0x0f00 | 'l';
    screen_buffer[cursor++] = 0x0f00 | 'o';
    screen_buffer[cursor++] = 0x0f00 | ',';
    screen_buffer[cursor++] = 0x0f00 | ' ';
    screen_buffer[cursor++] = 0x0f00 | 'W';
    screen_buffer[cursor++] = 0x0f00 | 'o';
    screen_buffer[cursor++] = 0x0f00 | 'r';
    screen_buffer[cursor++] = 0x0f00 | 'l';
    screen_buffer[cursor++] = 0x0f00 | 'd';
    screen_buffer[cursor++] = 0x0f00 | '!';


    // let's hang out here forever!
    while(1)
    { }
}