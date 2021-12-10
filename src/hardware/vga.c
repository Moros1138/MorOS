#include "stdint.h"
#include "hardware/vga.h"
#include "hardware/io.h"
#include "hardware/serial.h"

#include "string.h"

//------------------------------------------------------------------
// VGA Common Variables
//------------------------------------------------------------------

// running status flag; default false
static bool vgaRunning = false;
// track current video mode; default TextMode
video_mode_t vgaMode = TextMode;
// pointer to current video memory; default (uint8_t*)0xb8000
static uint8_t* vgaRAM;

//------------------------------------------------------------------
// VGA Text Mode Variables
//------------------------------------------------------------------

// current attribute byte value; default 0x07 (bg: black, fg: gray)
static uint8_t  vgaAttribute;
// track current index into vgaRAM
static uint32_t vgaIndex;
// track cursor location
static int32_t vgaColumn;
// track cursor location
static int32_t vgaRow;

//------------------------------------------------------------------
// VGA Graphics Mode Variables
//------------------------------------------------------------------
uint8_t g_320x200x256[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
//                COLOR emulation       MONO emulation
#define VGA_CRTC_INDEX		0x3D4		// 0x3B4
#define VGA_CRTC_DATA		0x3D5		// 0x3B5
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

void vga_write_registers(uint8_t* registers)
{
    // MISCELLANEOUS register
    outb(VGA_MISC_WRITE, *registers);
    registers++;
    
    // BEGIN: SEQUENCERS registers
    for(int i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, *registers);
        registers++;
    }
    // END: SEQUENCERS registers
    
    // BEGIN: unlock CRTC registers
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
    
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);

    // ensure registers remain unlocked after write
    registers[0x03] |= 0x80;
    registers[0x11] &= ~0x80;
    // END: unlock CRTC registers

    // BEGIN: CRTC registers
    for(int i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, *registers);
        registers++;
    }
    // END: CRTC registers
    
    // BEGIN: GRAPHICS CONTROLLER registers
    for(int i = 0; i < VGA_NUM_GC_REGS; i++)
    {
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, *registers);
        registers++;
    }
    // END: GRAPHICS CONTROLLER registers

    // BEGIN: ATTRIBUTE CONTROLLER registers
    for(int i = 0; i < VGA_NUM_AC_REGS; i++)
    {
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, *registers);
        registers++;
    }
    // END: ATTRIBUTE CONTROLLER registers

    // lock 16-color palette and unblank display
    inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, 0x20);
}

void vga_mode13()
{
    vga_write_registers(g_320x200x256);
    memset((uint8_t*)0xa0000, 0, 320 * 200);
}

void vga_init()
{
    vgaRAM = (uint8_t*)0xb8000;
    vgaAttribute = 0x07;

    // set maximum scan line register to 15
    outb(0x03D4, 0x09); outb(0x03D5, 15);
    
    // set the cursor end line to 15
    outb(0x03D4, 0x0b); outb(0x03D5, 15);
    // set the cursor start line to 14 and enable cursor visibility
    outb(0x03D4, 0x0a); outb(0x03D5, 14);
    
    vga_clear();
    vgaRunning = true;
}

void vga_clear()
{
    // clear screen
    for(vgaIndex = 0; vgaIndex < (80 * 25 * 2); vgaIndex += 2)
    {
        vgaRAM[vgaIndex  ] = ' ';
        vgaRAM[vgaIndex+1] = vgaAttribute;
    }

    vgaIndex = 0;
    vgaColumn = 0;
    vgaRow = 0;
}

void vga_move_cursor(int col, int row)
{
    // TODO: move the blinking cursor
    uint16_t cursorLocation = vgaRow * 80 + vgaColumn;

    outb(0x3d4, 0x0e); // send cursor location high byte
    outb(0x3d5, cursorLocation >> 8);
    outb(0x3d4, 0x0f); // send cursor location low byte
    outb(0x3d5, cursorLocation);
    
    // set the vgaIndex to the proper location
    vgaIndex = ((row * 80) + col) * 2;
}

void vga_scroll()
{
    uint8_t* end   = (vgaRAM + (80 * 25 * 2));
    uint8_t* last_line = (end - 160);

    for(uint8_t* i = (vgaRAM + 160); i != end; i++)
    {
        *(i - 160) = *i;
    }
    
    // blank last line
    for(uint8_t* i = last_line; i != end; i += 2)
    {
        *(i + 0) = ' ';
        *(i + 1) = vgaAttribute;
    }
}

void vga_putc(char ch)
{
    // handle backspace
    if(ch == 8)
    {
        if(vgaColumn - 1 < 0)
        {
            return;
        }
        
        vgaColumn--;
        
        vga_move_cursor(vgaColumn, vgaRow);
        
        return;
    }
    
    if(ch == '\t')
    {
        vgaColumn += 4 - (vgaColumn % 4);
        vga_move_cursor(vgaColumn, vgaRow);
        return;
    }

    // write character at cursor location
    if(ch == '\n')
    {
        // update cursor position
        vgaColumn = 0;
        vgaRow++;

        if(vgaRow == 25)
        {
            vga_scroll();
            vgaRow = 24;
        }
        
        // move index/cursor to next location
        vga_move_cursor(vgaColumn, vgaRow);

        return;
    }
    
    if(ch == '\r')
        return;
        
    vgaRAM[vgaIndex  ] = ch;
    vgaRAM[vgaIndex+1] = vgaAttribute;
    
    vgaColumn++;
    
    if(vgaColumn == 80)
    {
        vgaColumn = 0;
        vgaRow++;

        if(vgaRow == 25)
        {
            vga_scroll();
            vgaRow = 24;
        }
    }
    
    // move index/cursor to next location
    vga_move_cursor(vgaColumn, vgaRow);
}

bool vga_running()
{
    return vgaRunning;
}
