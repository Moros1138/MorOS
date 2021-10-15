#include "hardware/mouse.h"
#include "hardware/monitor.h"

namespace MorOS
{
    Mouse* Mouse::activeMouse = 0;

    Mouse::Mouse()
    {
        activeMouse = this;
        mon = Monitor::activeMonitor;

        register_interrupt_handler(IRQ12, &MorOS::Mouse::handler);
        
        while(inb(0x64) & 0x01)
        {
            inb(0x60);
        }

        outb(0x64, 0xa8);
        outb(0x64, 0x20);
        uint8_t status = inb(0x60) | 2;

        outb(0x64, 0x60);
        outb(0x60, status);
        
        outb(0x64, 0xd4);
        outb(0x60, 0xf4);

        inb(0x60);
    }
    
    void Mouse::UpdatePosition(int32_t x, int32_t y)
    {
        // accumulate x/y
        _x += x;
        _y += y;
        
        // clamp x/y to screen (in pixels or characters)
        if(_x < 0) _x = 0;
        if(_y < 0) _y = 0;
        if(_x > mon->Width()-1)  _x = mon->Width()-1;
        if(_y > mon->Height()-1) _y = mon->Height()-1;
        
        // TODO: hook into an event system??
    }
    
    void Mouse::UpdateFlags(uint8_t flags)
    {
        _flags = flags;
        // TODO: hook into an event system??
    }

    int Mouse::GetMouseX()
    {
        return _x;
    }
    
    int Mouse::GetMouseY()
    {
        return _y;
    }
    
    bool Mouse::GetButton(int b)
    {
        return (_flags >> b) & 0x1;
    }

    void Mouse::handler(registers_t regs)
    {
        static uint8_t offset = 2;
        static uint8_t buffer[3] = {0};

        uint8_t status = inb(0x64);
        
        // only process mouse packets
        if(!(status & 0x20))
            return;
        
        // read this byte into the buffer
        buffer[offset] = inb(0x60);
        
        // increment offset
        offset = (offset + 1) % 3;

        // if we have a complete packet, let's process it!
        if(offset == 0)
        {
            // Mouse Moved!
            if(buffer[1] != 0 && buffer[2] != 0)
                Mouse::activeMouse->UpdatePosition((int8_t)buffer[1], -((int8_t)buffer[2]));
            
            // Mouse Buttons!
            Mouse::activeMouse->UpdateFlags(buffer[0] & 0x07);
        }
    }

} // MorOS
