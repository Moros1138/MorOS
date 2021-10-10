#ifndef _MOROS_GDT_H
#define _MOROS_GDT_H

#include "types.h"
#include "memory.h"

extern "C" void gdt_flush(uint32_t);

namespace MorOS
{
    class SegmentDescriptor
    {
    private:
        uint16_t limit_low;           // The lower 16 bits of the limit.
        uint16_t base_low;            // The lower 16 bits of the base.
        uint8_t  base_middle;         // The next 8 bits of the base.
        uint8_t  access;              // Access flags, determine what ring this segment can be used in.
        uint8_t  granularity;
        uint8_t  base_high;           // The last 8 bits of the base.
    public:
        SegmentDescriptor();
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t acc, uint8_t gran);
    } __attribute__((packed));

    class GlobalDescriptorTable
    {
    private:
        SegmentDescriptor nullSegment;
        SegmentDescriptor codeSegment;
        SegmentDescriptor dataSegment;
        SegmentDescriptor userCodeSegment;
        SegmentDescriptor userDataSegment;
    public:
        GlobalDescriptorTable();
    } __attribute__((packed));

} // MorOS

#endif // _MOROS_GDT_H
