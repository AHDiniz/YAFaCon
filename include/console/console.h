#ifndef CONSOLE_H_

#define CONSOLE_H_

#include "pch.h"

namespace YAFaCon
{
    typedef uint16_t Data;
    typedef Data Instruction;
    typedef uint8_t Address;

    enum Instructions
    {
        ALU = 0x00,
        LDA = 0x10,
        STA = 0x20,
        WRL = 0x30,
        WRH = 0x40,
        CPY = 0x50,
        LDM = 0x60,
        STM = 0x70,
        BTG = 0x80,
        BRC = 0x90,
        JMP = 0xa0,
        SYS = 0xb0,
        CTX = 0xc0
    };

    enum BranchOperations
    {
        BEQ = 0x00,
        BNE = 0x01,
        BGR = 0x02,
        BGE = 0x03,
        BLS = 0x04,
        BLE = 0x05
    };

    enum ALUOperations
    {
        ADD  = 0x00,
        SUB  = 0x01,
        MUL  = 0x02,
        DIV  = 0x03,
        AND  = 0x04,
        OR   = 0x05,
        XOR  = 0x06,
        NOT  = 0x07,
        SHL  = 0x08,
        SHR  = 0x09,
        ADDF = 0x0a,
        SUBF = 0x0b,
        MULF = 0x0c,
        DIVF = 0x0d
    };

    enum Registers
    {
        ZERO,   // Always has the zero value
        ACC,    // Stores the results of ALU operations
        PARAM1, // Used as parameter for functions or system calls
        PARAM2, // Used as parameter for functions or system calls
        PARAM3, // Used as parameter for functions or system calls
        PARAM4, // Used as parameter for functions or system calls
        RET,    // Used as pointer to return value(s)
        REG0,   // Can be used however the programmer wants
        REG1,   // Can be used however the programmer wants
        REG2,   // Can be used however the programmer wants
        REG3,   // Can be used however the programmer wants
        REG4,   // Can be used however the programmer wants
        REG5,   // Can be used however the programmer wants
        REG6,   // Can be used however the programmer wants
        REG7,   // Can be used however the programmer wants
        REG8    // Can be used however the programmer wants
    };

    class MemoryBuffer
    {
    public:

        MemoryBuffer();
        MemoryBuffer(void *data);
        ~MemoryBuffer();

        void SetBuffer(void *data);

        const Data ReadData(int contextID, Address addr) const;
        Data &GetData(int contextID, Address addr);

    private:
        Data m_Buffer[4096];
    };
    
    class Cartridge
    {
    public:

        Cartridge();
        Cartridge(char *fileName);
        ~Cartridge();

        inline const Instruction ReadInstruction(int ctx, Address addr) const { return m_Instructions.ReadData(ctx, addr); }
        inline Instruction &GetInstruction(int ctx, Address addr) { return m_Instructions.GetData(ctx, addr); }

        inline const Data ReadData(int ctx, Address addr) const { return m_Data.ReadData(ctx, addr); }
        inline Data &GetData(int ctx, Address addr) { return m_Data.GetData(ctx, addr); }

        inline const Data ReadBackground(int ctx, Address addr) const { return m_Background.ReadData(ctx, addr); }
        inline Data &GetBackground(int ctx, Address addr) { return m_Background.GetData(ctx, addr); }

        inline const Data ReadForeground(int ctx, Address addr) const { return m_Foreground.ReadData(ctx, addr); }
        inline Data &GetForeground(int ctx, Address addr) { return m_Foreground.GetData(ctx, addr); }

    private:

        MemoryBuffer m_Instructions;
        MemoryBuffer m_Data;
        MemoryBuffer m_Background;
        MemoryBuffer m_Foreground;
    };

    class Processor
    {
    public:

        Processor(Cartridge c);
        ~Processor();

        void Execute();

    private:

        Data m_Registers[16];
        
        Address m_PC;
        Address m_NextPC;
        Address m_BranchTarget;

        Address m_InstCtx;
        Address m_DataCtx;
        Address m_BackCtx;
        Address m_ForeCtx;

        Cartridge m_Cartridge;
        MemoryBuffer m_RandomAccess;

        void runInstruction(Instruction i);
    };
}

#endif
