#ifndef CONSOLE_H_

#define CONSOLE_H_

#include "pch.h"

namespace YAFaCon
{
    typedef uint16_t Data;
    typedef Data Instruction;
    typedef uint8_t Address;

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
