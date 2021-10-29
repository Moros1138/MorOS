#ifndef _MOROS_MOUSE_H
#define _MOROS_MOUSE_H

#include "types.h"
#include "isr.h"
#include "io.h"

#include "hardware/monitor.h"

namespace MorOS
{
    class Mouse
    {
    public:
        Mouse();
        
        // globally accessible instance of this class
        static Mouse* activeMouse;
        


        // the interrupt handler for our mouse
        static void handler(registers_t regs);
        
        static uint8_t offset;
        static uint8_t buffer[3];
        static uint8_t packetSize;

        void Wait(uint8_t type);
        uint8_t Read();
        void Write(uint8_t value);

    public: // Update functions    
        
        void ProcessPacket();
    
    public: // Position and Button Getters
        
        // get current mouse x coordinate
        int GetMouseX();
        // get current mouse y coordinate
        int GetMouseY();
        // get if the provided mouse button is held
        bool GetButton(int b);

    private:
        // position: x coordinate in characters or pixels, depends on screen mode
        int32_t _x;
        // position: y coordinate in characters or pixels, depends on screen mode
        int32_t _y;
        // button states
        uint8_t _flags;

        // reference to the global monitor
        Monitor* mon;
    };
    
} // MorOS

#endif // _MOROS_MOUSE_H
