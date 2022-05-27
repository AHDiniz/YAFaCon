#include "console/console.h"

namespace YAFaCon
{
    MemoryBuffer::MemoryBuffer()
    {
        memset(m_Buffer, 0, 4096 * sizeof(Data));
    }

    MemoryBuffer::MemoryBuffer(void *data)
    {
        memcpy(m_Buffer, data, 4096 * sizeof(Data));
    }

    void MemoryBuffer::SetBuffer(void *data)
    {
        memcpy(m_Buffer, data, 4096 * sizeof(Data));
    }

    const Data MemoryBuffer::ReadData(int contextID, Address addr) const
    {
        return m_Buffer[(contextID - 1) * 256 + addr];
    }

    Data &MemoryBuffer::GetData(int contextID, Address addr)
    {
        return m_Buffer[(contextID - 1) * 256 + addr];
    }

    Cartridge::Cartridge()
    {
        Data b[4096];
        memset(b, 0, 4096 * sizeof(Data));
        m_Instructions.SetBuffer(b);
        m_Data.SetBuffer(b);
        m_Background.SetBuffer(b);
        m_Foreground.SetBuffer(b);
    }

    Cartridge::Cartridge(char *fileName)
    {

    }

    Cartridge::~Cartridge()
    {

    }

    Processor::Processor(Cartridge c)
    {
        m_Cartridge = c;
        m_PC = 0;
        m_NextPC = 1;
        m_InstCtx = m_DataCtx = m_BackCtx = m_ForeCtx = 0;
    }

    Processor::~Processor()
    {

    }

    void Processor::Execute()
    {
        while (m_PC != 256)
        {
            const Instruction i = m_Cartridge.ReadInstruction(m_InstCtx, m_PC);
            runInstruction(i);

            m_PC = m_NextPC;

            if (m_NextPC == m_PC + 1)
                ++m_NextPC;
        }
    }

    void Processor::runInstruction(Instruction i)
    {
        
    }
}
