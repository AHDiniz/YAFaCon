#ifndef CONSOLE_H_

#define CONSOLE_H_

#include <cstdint>
#include <cstdlib>
#include <vector>

namespace YAFaCon
{
    typedef uint16_t Data;
    typedef Data Instruction;
    typedef uint8_t Address;

    class MemoryBuffer
    {
    public:

        MemoryBuffer();
        ~MemoryBuffer();

        const Data ReadData(int contextID, Address addr) const;
        Data &GetData(int contextID, Address addr);

    private:
        Data m_Buffer[4096];
    };
    
    class Cartridge
    {
    public:

        Cartridge();
        ~Cartridge();

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

        Cartridge m_Cartridge;
        MemoryBuffer m_RandomAccess;

        void fetch();
    };
}

#endif
