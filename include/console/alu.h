#ifndef ALU_H_

#define ALU_H_

#include "pch.h"

namespace YAFaCon
{
    inline uint16_t Add(uint16_t a, uint16_t b)
    {
        return a + b;
    }

    inline uint16_t Subtract(uint16_t a, uint16_t b)
    {
        return a - b;
    }

    inline uint16_t Multiply(uint16_t a, uint16_t b)
    {
        return a * b;
    }

    inline uint16_t Divide(uint16_t a, uint16_t b)
    {
        return a / b;
    }

    inline uint16_t And(uint16_t a, uint16_t b)
    {
        return a & b;
    }

    inline uint16_t Or(uint16_t a, uint16_t b)
    {
        return a | b;
    }

    inline uint16_t Xor(uint16_t a, uint16_t b)
    {
        return a ^ b;
    }

    inline uint16_t Not(uint16_t a)
    {
        return ~a;
    }

    inline uint16_t ShiftRight(uint16_t a, uint16_t b)
    {
        return a >> b;
    }

    inline uint16_t ShiftLeft(uint16_t a, uint16_t b)
    {
        return a << b;
    }
}

#endif
