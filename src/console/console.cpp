#include "console/console.h"
#include "console/alu.h"
#include "utils.h"

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

    typedef struct dataSplit
    {
        uint8_t high;
        uint8_t low;
    } DataSplit;

    uint8_t ExecuteALUOperation(uint8_t a, uint8_t b, Address op)
    {
        switch (op)
        {
            case ADD:
                return Add(a, b);
            case SUB:
                return Subtract(a, b);
            case MUL:
                return Multiply(a, b);
            case DIV:
                return Divide(a, b);
            case AND:
                return And(a, b);
            case OR:
                return Or(a, b);
            case XOR:
                return Xor(a, b);
            case NOT:
                return Not(a);
            case SHL:
                return ShiftLeft(a, b);
            case SHR:
                return ShiftRight(a, b);
            default:
                break;
        }
        return 0;
    }

    void Processor::runInstruction(Instruction i)
    {
        DataSplit *split = (DataSplit *)&i;

        uint8_t instID = 0;
        Utils::WriteHighNibble(instID, split->high);

        switch (instID)
        {
            case ALU:
            {
                Address regA = 0, regB = 0;
                Utils::WriteLowNibble(regA, split->high);
                regA >>= 4; 
                Utils::WriteHighNibble(regB, split->low);
                
                Address op = 0;
                Utils::WriteLowNibble(op, split->low);

                m_Registers[ACC] = ExecuteALUOperation(m_Registers[regA], m_Registers[regB], op);
            } break;
            case LDA:
            {
                Address reg = 0;
                Utils::WriteLowNibble(reg, split->high);
                reg >>= 4;
                
                Address addr = split->low;
                m_Registers[reg] = m_Cartridge.ReadData(m_DataCtx, addr);
            } break;
            case STA:
            {
                Address reg = 0;
                Utils::WriteLowNibble(reg, split->high);
                reg >>= 4;
                Data src = m_Registers[reg];

                Address addr = split->low;
                Data &dest = m_Cartridge.GetData(m_DataCtx, addr);

                dest = src; 
            } break;
            case WRL:
            {
                Address reg = 0;
                Utils::WriteLowNibble(reg, split->high);
                reg >>= 4;

                DataSplit *regData = (DataSplit*)&(m_Registers[reg]);
                regData->low = split->low;
            } break;
            case WRH:
            {
                Address reg = 0;
                Utils::WriteLowNibble(reg, split->high);
                reg >>= 4;

                DataSplit *regData = (DataSplit*)&(m_Registers[reg]);
                regData->high = split->low;
            } break;
            case CPY:
            {
                Address regA = 0, regB = 0;
                Utils::WriteLowNibble(regA, split->high);
                regA >>= 4;

                Utils::WriteHighNibble(regB, split->low);
                
                Data src = m_Registers[regB];
                Data &dest = m_Registers[regA];

                dest = src;
            } break;
            case LDM:
            {
                
            } break;
            case STM:
            {} break;
            case BTG:
            {} break;
            case BRC:
            {} break;
            case JMP:
            {} break;
            case SYS:
            {} break;
            case CTX:
            {} break;
            default: break;
        }
    }
}
