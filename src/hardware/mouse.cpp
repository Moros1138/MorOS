#include "hardware/mouse.h"
#include "hardware/monitor.h"

namespace MorOS
{
    Mouse* Mouse::activeMouse = 0;
    
    uint8_t Mouse::offset = 0;
    uint8_t Mouse::buffer[3] = {0};

    Mouse::Mouse()
    {
        activeMouse = this;
        mon = Monitor::activeMonitor;

        uint8_t status;

        // enable mouse (auxilliary device)
        Wait(1);
        outb(0x64, 0xa8);

        // enable interrupts
        Wait(1);
        outb(0x64, 0x20);

        // read status
        Wait(0);
        status = inb(0x60) | 2;
        Wait(1);
        outb(0x64, 0x60);

        // update status
        Wait(1);
        outb(0x60, status);
        
        // tell mouse to use default settings        
        Write(0xf6);
        Read(); // ACK

        // enable mouse
        Write(0xf4);
        Read(); // ACK

        register_interrupt_handler(IRQ12, &MorOS::Mouse::handler);
    }
    
    uint8_t Mouse::Read()
    {
        Wait(0);
        return inb(0x60);
    }

    void Mouse::Write(uint8_t value)
    {
        // wait to be able to send a signal
        Wait(1);
        
        // tell mouse we're sending a command
        outb(0x64, 0xd4);

        // wait again
        Wait(1);

        outb(0x60, value);
    }

    void Mouse::Wait(uint8_t type)
    {
        uint16_t timeout = 0xffff;
        if(type == 0)
        {
            while(timeout--) // data
            {
                if((inb(0x64) & 1) == 1)
                    return;
            }
        }
        else
        {
            while(timeout--) // signal
            {
                if((inb(0x64) & 2) == 0)
                    return;
            }
        }
        
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
    
    void Mouse::ProcessPacket()
    {
        // toss packet if y overflow bit is set
        if((buffer[0] & 0x80) == 1)
            return;
        
        // toss packet if x overflow bit is set
        if((buffer[0] & 0x40) == 1)
            return;

        int32_t x = (int8_t)Mouse::buffer[1];
        int32_t y = (int8_t)Mouse::buffer[2];
        
        // handle negative y
        if((buffer[0] & 0x20) == 1)
        {
            y = y | 0xffffff00;
            y = -y;
        }
        
        // handle negative x
        if((buffer[0] & 0x10) == 1)
        {
            x = x | 0xffffff00;
            x = -x;
        }
        
        // update mouse position
        _x += (int32_t)x;
        _y += -(int32_t)y;

        // clamp x/y to screen (in pixels or characters)
        if(_x < 0) _x = 0;
        if(_y < 0) _y = 0;
        if(_x > mon->Width()-1)  _x = mon->Width()-1;
        if(_y > mon->Height()-1) _y = mon->Height()-1;

        // button states
        _flags = buffer[0] & 0x07;

    }

    void Mouse::handler(registers_t regs)
    {
        // read this byte into the buffer
        Mouse::buffer[Mouse::offset] = inb(0x60); // Mouse::activeMouse->Read();
        
        
        // increment offset
        Mouse::offset = (Mouse::offset + 1) % 3;

        // if we have a complete packet, let's process it!
        if(Mouse::offset == 0)
            Mouse::activeMouse->ProcessPacket();
    }

} // MorOS
