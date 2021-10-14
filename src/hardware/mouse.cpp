#include "hardware/mouse.h"
#include "hardware/monitor.h"

namespace MorOS
{
    Mouse* Mouse::activeMouse = 0;
    uint8_t Mouse::offset = 0;
    uint8_t Mouse::buttons = 0;
    int32_t Mouse::cursor_x;
    int32_t Mouse::cursor_y;
    uint8_t Mouse::buffer[3] = { 0 };

    Mouse::Mouse()
    {
        activeMouse = this;
        
        register_interrupt_handler(IRQ12, &MorOS::Mouse::handler);
    }

    void Mouse::handler(registers_t regs)
    {
        uint8_t status = inb(0x64);
        
        if(!(status & 0x20))
            return;
        
        buffer[offset] = inb(0x60);
        
        offset = (offset + 1) % 3;

        if(offset == 0)
        {
            if(buffer[1] != 0 && buffer[2] != 0)
            {
                // moved!
                cursor_x += (int32_t)buffer[1];
                cursor_y += -((int32_t)buffer[2]);

                // clamp cursor to visible screen                
                if(cursor_x < 0) cursor_x = 0;
                if(cursor_y < 0) cursor_y = 0;
                if(cursor_x > Monitor::activeMonitor->Width()) cursor_x = Monitor::activeMonitor->Width();
                if(cursor_y > Monitor::activeMonitor->Height()) cursor_y = Monitor::activeMonitor->Height();

                printf(" --- MOUSE COORDS:  %d - %d ---\n");
            }
        }
    }

} // MorOS
