#include "gdt.h"

namespace MorOS
{

    SegmentDescriptor::SegmentDescriptor()
    {
        base_low    = 0;
        base_middle = 0;
        base_high   = 0;
        limit_low   = 0;
        granularity = 0;
        access      = 0;
    }

    SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t acc, uint8_t gran)
    {
        base_low    = (base & 0xFFFF);
        base_middle = (base >> 16) & 0xFF;
        base_high   = (base >> 24) & 0xFF;
        limit_low   = (limit & 0xFFFF);
        granularity = (limit >> 16) & 0x0F;
        granularity |= gran & 0xF0;
        access      = acc;
    }

    GlobalDescriptorTable::GlobalDescriptorTable()
    :   nullSegment(0, 0, 0, 0),
        codeSegment(0, 0xFFFFFFFF, 0x9A, 0xCF),
        dataSegment(0, 0xFFFFFFFF, 0x92, 0xCF),
        userCodeSegment(0, 0xFFFFFFFF, 0xFA, 0xCF),
        userDataSegment(0, 0xFFFFFFFF, 0xF2, 0xCF)
    {
        uint16_t gptr[3];
        
        gptr[0] = sizeof(GlobalDescriptorTable) - 1;
        gptr[2] = ((uint32_t)this >> 16) & 0x0000ffff;
        gptr[1] = ((uint32_t)this) & 0x0000ffff;
        
        gdt_flush((uint32_t)gptr);
    }

} // MorOS
