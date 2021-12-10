#include "hardware/io.h"
#include "hardware/mouse.h"
#include "hardware/interrupts.h"

#include "assert.h"
#include "event.h"
#include "string.h"

static uint8_t       packet[4];
static int           packetIndex;
static int           packetSize;

static bool          oldButtons[5] = {0};
static bool          newButtons[5] = {0};

static void mouse_interrupt_handler(registers_t regs)
{
    UNUSED(regs);
    
    // insert next byte into packet
    packet[packetIndex] = inb(0x60);
    
    // update packet index
    packetIndex = (packetIndex + 1) % packetSize;

    // quit here, if we don't have a complete packet
    if(packetIndex != 0)
        return;

    // attempt to resync
    if((packet[0] & 0x08) == 0)
    {
        packetIndex++;
        return;
    }

    // toss packet if y or x overflow bit is set
    if((packet[0] & 0x80) || (packet[0] & 0x40))
        return;
    
    motion_event_t motion;
    motion.type = Motion;
    motion.x = (int8_t)packet[1];
    motion.y = -(int8_t)packet[2];
    motion.z = 0;

    // update z axis only when it's not 0
    if((packet[3] & 0xf) != 0)
    {
        if((packet[3] & 0xf) == 1)
            motion.z = -1;
        else if((packet[3] & 0xf) == 15)
            motion.z = 1;
        else
            motion.z = 0;
    }
    
    // fire motion event
    if(motion.x || motion.y || motion.z)
        event_fire((event_t*)&motion);

    // update button states
    newButtons[0] = (packet[0] & (0x1 << 0)) ? true : false;
    newButtons[1] = (packet[0] & (0x1 << 1)) ? true : false;
    newButtons[2] = (packet[0] & (0x1 << 2)) ? true : false;
    newButtons[3] = (packet[3] & (0x1 << (4))) ? true : false;
    newButtons[4] = (packet[3] & (0x1 << (5))) ? true : false;

    button_event_t button;

    for(int i = 0; i < 5; i++)
    {
        if(!oldButtons[i] && newButtons[i]) // Press
        {
            button.type = ButtonPress;
            button.button = i;
            event_fire((event_t*)&button);
        }
        
        if(oldButtons[i] && !newButtons[i]) // Release
        {
            button.type = ButtonRelease;
            button.button = i;
            event_fire((event_t*)&button);
        }

        oldButtons[i] = newButtons[i];
    }
}

static const uint16_t CTRL_PORT = 0x64;
static const uint16_t DATA_PORT = 0x60;

void mouse_write(uint16_t port, uint8_t data)
{
    // https://wiki.osdev.org/Mouse_Input#Waiting_to_Send_Bytes_to_Port_0x60_and_0x64
    uint16_t timeout = 0xffff;
    while(timeout--)
    {
        if((inb(CTRL_PORT) & 0x2) == 0)
            break;
    }
    
    outb(port, data);
}

uint8_t mouse_read(uint16_t port)
{
    // https://wiki.osdev.org/Mouse_Input#Waiting_to_Send_Bytes_to_Port_0x60_and_0x64
    if(port == DATA_PORT)
    {
        uint16_t timeout = 0xffff;
        while(timeout--)
        {
            if((inb(CTRL_PORT) & 0x1) == 1)
                break;
        }
    }
    
    return inb(port);
}

uint8_t mouse_set_sample_rate(int rate)
{
    uint8_t ack;

    outb(0x64, 0xd4);
    outb(0x60, 0xf3);
    while(!(inb(0x64) & 1))
        __asm__ __volatile__("nop;");
    ack = inb(0x60);
    
    outb(0x64, 0xd4);
    outb(0x60, rate);

    while(!(inb(0x64) & 1))
        __asm__ __volatile__("nop;");
    ack = inb(0x60);
    return ack;
}

uint8_t mouse_identify()
{
    outb(0x64, 0xd4);
    outb(0x60, 0xf2);

    while(!(inb(0x64) & 1))
        __asm__ __volatile__("nop;");
    assert(inb(0x60) == 0xfa);
    
    return inb(0x60);
}

void mouse_init()
{
    // initilaize variables
    memset(&packet, 0, sizeof(uint8_t) * 4);
    packetIndex = 0;
    packetSize  = 3; // default, will be 4 later

    // Enable Auxiliar Device Command
    outb(CTRL_PORT, 0xa8);

    //--------------------------------------------------------------
    // BEGIN compaq status
    //--------------------------------------------------------------

    // get compaq status byte command
    mouse_write(CTRL_PORT, 0x20);
    
    // read status byte
    uint8_t status = mouse_read(DATA_PORT) | 2;
    
    // set compaq status command
    mouse_write(CTRL_PORT, 0x60);

    // update status
    mouse_write(DATA_PORT, status);
    
    //--------------------------------------------------------------
    // END compaq status
    //--------------------------------------------------------------

    //--------------------------------------------------------------
    // START intellimouse
    //--------------------------------------------------------------
    
    mouse_set_sample_rate(200);
    mouse_set_sample_rate(100);
    mouse_set_sample_rate(80);
    
    if(mouse_identify() == 3)
    {
        mouse_set_sample_rate(200);
        mouse_set_sample_rate(200);
        mouse_set_sample_rate(80);
    }

    if(mouse_identify() == 4)
    {
        packetSize = 4;
    }

    //--------------------------------------------------------------
    // END intellimouse
    //--------------------------------------------------------------

    // enable mouse
    mouse_write(CTRL_PORT, 0xd4);
    mouse_write(DATA_PORT, 0xf4);
    mouse_read(DATA_PORT); // ACK
    
    register_interrupt_handler(IRQ12, &mouse_interrupt_handler);
}
