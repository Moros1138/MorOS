#include "stdint.h"
#include "hardware/vga.h"
#include "hardware/io.h"

#include "assert.h"
#include "stdlib.h"
#include "string.h"

//------------------------------------------------------------------
// VGA ports
//------------------------------------------------------------------
#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		    0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5

#define VGA_DAC_MASK        0x3C6
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9

#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF

#define VGA_CRTC_INDEX		0x3D4		// 0x3B4 for mono emulation
#define VGA_CRTC_DATA		0x3D5		// 0x3B5 for mono emulation
#define	VGA_INSTAT_READ		0x3DA

#define SZ(x) (int)(sizeof(x)/sizeof(x[0]))

//------------------------------------------------------------------
// VGA Common Variables
//------------------------------------------------------------------

// running status flag; default false
static bool vgaRunning = false;
// track current video mode; default TextMode
video_mode_t vgaMode;
// pointer to current video memory
static uint8_t* vgaRAM;

//------------------------------------------------------------------
// VGA Text Mode Variables
//------------------------------------------------------------------

// pointer to current text mode buffer
static uint8_t* vgaTextBuffer;
// current attribute byte value; default 0x07 (bg: black, fg: gray)
static uint8_t  vgaTextAttribute;
// track cursor location
static int32_t vgaTextColumn;
// track cursor location
static int32_t vgaTextRow;
// track current index into vgaTextBuffer/vgaRAM
static uint32_t vgaTextIndex;
// pointer to font from vgaFont.c
extern uint8_t vgaFont[4096];

//------------------------------------------------------------------
// VGA Graphics Mode Variables
//------------------------------------------------------------------

// pointer to current graphics mode buffer
static uint8_t* vgaGraphicsBuffer;

// pointer to palette in vgaPalette.c
extern uint32_t vgaPalette[256];
extern uint32_t vgaPaletteCount;

void vga_text_move_cursor(int col, int row)
{
    // TODO: move the blinking cursor
    uint16_t cursorLocation = vgaTextRow * 80 + vgaTextColumn;

    outb(VGA_CRTC_INDEX, 0x0e); // send cursor location high byte
    outb(VGA_CRTC_DATA, cursorLocation >> 8);
    outb(VGA_CRTC_INDEX, 0x0f); // send cursor location low byte
    outb(VGA_CRTC_DATA, cursorLocation);
    
    // set the vgaIndex to the proper location
    vgaTextIndex = ((row * 80) + col) * 2;
}

void vga_switch_to_mode03()
{
    const uint8_t crtcRegisters[] = {
        0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
        0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
        0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
        0xFF,
    };

    const uint8_t seqRegisters[] = {
        0x03, 0x00, 0x03, 0x00, 0x02,
    };
	
    const uint8_t gcRegisters[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
        0xFF,
    };
    
    const uint16_t acRegisters[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x0C, 0x00, 0x0F, 0x08, 0x00    
    };

    outb(VGA_MISC_WRITE, 0x67);
    
    // unlock registers
    outw(VGA_CRTC_INDEX, 0x0e11);

    for(int i = 0; i < SZ(crtcRegisters); i++)
    {
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, crtcRegisters[i]);
    }

    for(int i = 0; i < SZ(seqRegisters); i++)
    {
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, seqRegisters[i]);
    }
        
    for(int i = 0; i < SZ(gcRegisters); i++)
    {
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, gcRegisters[i]);
    }
        
    
    for(int i = 0; i < SZ(acRegisters); i++)
    {
        inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, acRegisters[i]);
    }

    // ega palette
    for(uint8_t i = 0; i < 16; i++)
    {
        inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, i);
    }
    
    inb(VGA_INSTAT_READ);
    outb(VGA_AC_WRITE, 0x20); // enable video

    //--------------------------------------------------------------
    // BEGIN: load custom font
    //--------------------------------------------------------------
    uint8_t* buf = vgaFont;
    uint8_t seq2, seq4, gc4, gc5, gc6;

    outb(VGA_SEQ_INDEX, 2);
    seq2 = inb(VGA_SEQ_DATA);

    outb(VGA_SEQ_INDEX, 4);
    seq4 = inb(VGA_SEQ_DATA);
    
    // turn off even-odd addressing (set flat addressing)
    // assume: chain-4 addressing already off
    outb(VGA_SEQ_DATA, seq4 | 0x04);

    outb(VGA_GC_INDEX, 4);
    gc4 = inb(VGA_GC_DATA);

    outb(VGA_GC_INDEX, 5);
    gc5 = inb(VGA_GC_DATA);
    outb(VGA_GC_DATA, gc5 & ~0x10);

    outb(VGA_GC_INDEX, 6);
    gc6 = inb(VGA_GC_DATA);
    outb(VGA_GC_DATA, gc6 & ~0x02);

    // write font to plane P4
    // set read plane
    outb(VGA_GC_INDEX, 0x04);
    outb(VGA_GC_DATA, 0x02);
    // set write plane
    outb(VGA_SEQ_INDEX, 2);
    outb(VGA_SEQ_DATA, 0x04);

    // do the writing of the damn font
    for(int i = 0; i < 256; i++)
    {
        memcpy((char *)(0xb8000 + (i * 32)), buf, 16);
        buf += 16;
    }    

    // restore registers
    outb(VGA_SEQ_INDEX, 2);
    outb(VGA_SEQ_DATA, seq2);
    outb(VGA_SEQ_INDEX, 4);
    outb(VGA_SEQ_DATA, seq4);
    outb(VGA_GC_INDEX, 4);
    outb(VGA_GC_DATA, gc4);
    outb(VGA_GC_INDEX, 5);
    outb(VGA_GC_DATA, gc5);
    outb(VGA_GC_INDEX, 6);
    outb(VGA_GC_DATA, gc6);
    
    //--------------------------------------------------------------
    // END: load custom font
    //--------------------------------------------------------------

    // set maximum scan line register to 15
    outb(VGA_CRTC_INDEX, 0x09);
    outb(VGA_CRTC_DATA, 15);
    
    // set the cursor end line to 15
    outb(VGA_CRTC_INDEX, 0x0b);
    outb(VGA_CRTC_DATA, 15);
    
    // set the cursor start line to 14 and enable cursor visibility
    outb(VGA_CRTC_INDEX, 0x0a);
    outb(VGA_CRTC_DATA, 14);
    
    vgaRAM = (uint8_t*)0xb8000;
    vgaMode = TextMode;
    vgaTextAttribute = 0x07;
    vga_swap();
}

void vga_switch_to_mode13()
{
    const uint16_t crtcRegisters[] = {
        0x5f00, 0x4f01, 0x5002, 0x8203, 0x5404, 0x8005, 0xbf06, 0x1f07,
        0x4109, 0x9c10, 0x8e11, 0x8f12, 0x2813, 0x9615, 0xb916
    };

    outb(VGA_MISC_WRITE, 0x63);
    outw(VGA_CRTC_INDEX, 0x0e11); // enable regs 0-7

    for(int i = 0; i < 15; i++) 
        outw(VGA_CRTC_INDEX, crtcRegisters[i]);

    outw(VGA_CRTC_INDEX, 0x0008); // vert.panning = 0

    outw(VGA_CRTC_INDEX, 0x4014);
    outw(VGA_CRTC_INDEX, 0xa317);
    outw(VGA_SEQ_INDEX, 0x0e04);

    outw(VGA_SEQ_INDEX, 0x0101);
    outw(VGA_SEQ_INDEX, 0x0f02); // enable writing to all planes
    outw(VGA_GC_INDEX, 0x4005); // 256color mode
    outw(VGA_GC_INDEX, 0x0506); // graph mode & A000-AFFF

    inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX,0x30); outb(VGA_AC_WRITE,0x41);
    outb(VGA_AC_INDEX,0x33); outb(VGA_AC_WRITE,0x00);
    
    // ega pal
    for(uint8_t i = 0; i < 16; i++)
    {
        outb(VGA_AC_INDEX, i); 
        outb(VGA_AC_WRITE, i); 
    } 
    
    inb(VGA_INSTAT_READ);
    outb(VGA_AC_WRITE, 0x20); // enable video

    // BEGIN: define palette
    outb(VGA_DAC_MASK, 0xFF);
    outb(VGA_DAC_WRITE_INDEX, 0);
    for(size_t i = 0; i < vgaPaletteCount; i++)
    {
        outb(VGA_DAC_DATA, ((vgaPalette[i] & 0x000000ff) >>  0) >> 2); // red
        outb(VGA_DAC_DATA, ((vgaPalette[i] & 0x0000ff00) >>  8) >> 2); // green
        outb(VGA_DAC_DATA, ((vgaPalette[i] & 0x00ff0000) >> 16) >> 2); // blue
    }
    // END: define palette

    vgaRAM = (uint8_t*)0xa0000;
    vgaMode = GraphicsMode;
    vga_swap();
}

void vga_graphics_swap()
{
    if(vgaMode != GraphicsMode)
        return;
    
    memcpy(vgaRAM, vgaGraphicsBuffer, 64000);
}

void vga_text_swap()
{
    if(vgaMode != TextMode)
        return;
    
    memcpy(vgaRAM, vgaTextBuffer, 80 * 25 * 2);
    vga_text_move_cursor(vgaTextColumn, vgaTextRow);
}

void vga_text_clear()
{
    // clear screen
    for(vgaTextIndex = 0; vgaTextIndex < (80 * 25 * 2); vgaTextIndex += 2)
    {
        vgaTextBuffer[vgaTextIndex    ] = ' ';
        vgaTextBuffer[vgaTextIndex + 1] = vgaTextAttribute;
    }
    
    vga_text_swap();

    vgaTextIndex = 0;
    vgaTextColumn = 0;
    vgaTextRow = 0;
}

void vga_text_scroll()
{
    uint8_t* end   = (vgaTextBuffer + (80 * 25 * 2));
    uint8_t* last_line = (end - 160);

    for(uint8_t* i = (vgaTextBuffer + 160); i != end; i++)
        *(i - 160) = *i;
    
    // blank last line
    for(uint8_t* i = last_line; i != end; i += 2)
    {
        *(i + 0) = ' ';
        *(i + 1) = vgaTextAttribute;
    }
    vga_text_swap();
}

void vga_text_putc(char ch)
{
    // handle backspace
    if(ch == 8)
    {
        if(vgaTextColumn - 1 < 0)
        {
            return;
        }
        
        vgaTextColumn--;
        
        vga_text_move_cursor(vgaTextColumn, vgaTextRow);
        
        return;
    }
    
    if(ch == '\t')
    {
        vgaTextColumn += 4 - (vgaTextColumn % 4);
        vga_text_move_cursor(vgaTextColumn, vgaTextRow);
        return;
    }

    // write character at cursor location
    if(ch == '\n')
    {
        // update cursor position
        vgaTextColumn = 0;
        vgaTextRow++;

        if(vgaTextRow == 25)
        {
            vga_text_scroll();
            vgaTextRow = 24;
        }
        
        // move index/cursor to next location
        vga_text_move_cursor(vgaTextColumn, vgaTextRow);

        return;
    }
    
    if(ch == '\r')
        return;
        
    vgaTextBuffer[vgaTextIndex  ] = ch;
    vgaTextBuffer[vgaTextIndex+1] = vgaTextAttribute;
    
    vgaTextColumn++;
    
    if(vgaTextColumn == 80)
    {
        vgaTextColumn = 0;
        vgaTextRow++;

        if(vgaTextRow == 25)
        {
            vga_text_scroll();
            vgaTextRow = 24;
        }
    }
    
    vga_text_swap();

    // move index/cursor to next location
    vga_text_move_cursor(vgaTextColumn, vgaTextRow);
}

void vga_set_mode(video_mode_t mode)
{
    if(mode == TextMode && vgaMode != TextMode)
        vga_switch_to_mode03();

    if(mode == GraphicsMode && vgaMode != GraphicsMode)
        vga_switch_to_mode13();
}

void vga_graphics_clear(uint8_t col)
{
    memset(vgaGraphicsBuffer, col, 64000);
}

void vga_graphics_draw(int x, int y, uint8_t col)
{
    if(x < 0 || y < 0 || x > 319 || y > 199)
        return;
    
    vgaGraphicsBuffer[y * 320 + x] = col;
}

void vga_text_set_attribute(uint8_t attr)
{
    vgaTextAttribute = attr;
}

void vga_swap()
{
    if(vgaMode == TextMode) vga_text_swap();
    if(vgaMode == GraphicsMode) vga_graphics_swap();
}

void vga_init()
{
    vgaTextBuffer = (uint8_t*)malloc(80 * 25 * 2);
    vgaGraphicsBuffer = (uint8_t*)malloc(320 * 200);
    
    vga_switch_to_mode03();
    vga_text_clear();
    vgaRunning = true;
}

bool vga_running()
{
    return vgaRunning;
}


