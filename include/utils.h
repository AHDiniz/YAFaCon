#ifndef UTILS_H_

#define UTILS_H_

#include "pch.h"

namespace Utils
{
    inline uint8_t ReadHighNibble(const uint8_t data)
    {
        return (data & 0xf0);
    }

    inline uint8_t ReadLowNibble(const uint8_t data)
    {
        return (data & 0xf0);
    }

    inline void WriteHighNibble(uint8_t &dest, const uint8_t src)
    {
        dest = (dest & 0x0F) | ((src & 0xF) << 4);
    }

    inline void WriteLowNibble(uint8_t &dest, const uint8_t src)
    {
        dest = (dest & 0xF0) | (src & 0xF);
    }
}

#endif
