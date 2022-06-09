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

    bool BranchCompare(Data a, Data b, Address op)
    {
        switch (op)
        {
            case BEQ:
                return a == b;
            case BNE:
                return a != b;
            case BGR:
                return a > b;
            case BGE:
                return a >= b;
            case BLS:
                return a < b;
            case BLE:
                return a <= b;
            default: break;
        }
        return false;
    }

    void Processor::runInstruction(Instruction i)
    {
        DataSplit *split = (DataSplit *)&i;

        uint8_t instID = 0;
        instID = (Utils::ReadHighNibble(split->high)) >> 4;

        switch (instID)
        {
            case ALU:
            {
                Address regA = Utils::ReadLowNibble(split->high);
                Address regB = Utils::ReadHighNibble(split->low) >> 4;
                Address op = Utils::ReadLowNibble(split->low);

                m_Registers[ACC] = ExecuteALUOperation(m_Registers[regA], m_Registers[regB], op);
            } break;
            case LDA:
            {
                Address reg = Utils::ReadLowNibble(split->high);

                const Data loadData = m_RandomAccess.ReadData(m_RamCtx, split->low);

                m_Registers[reg] = loadData;
            } break;
            case STA:
            {
                Address reg = Utils::ReadLowNibble(split->high);

                Data &storeData = m_RandomAccess.GetData(m_RamCtx, split->low);

                storeData = m_Registers[reg];
            } break;
            case WRL:
            {
                Address reg = Utils::ReadLowNibble(split->high);

                DataSplit *regData = (DataSplit*)&(m_Registers[reg]);

                regData->low = split->low;                
            } break;
            case WRH:
            {
                Address reg = Utils::ReadLowNibble(split->high);

                DataSplit *regData = (DataSplit*)&(m_Registers[reg]);

                regData->high = split->low;
            } break;
            case CPY:
            {
                Address regA = Utils::ReadLowNibble(split->high);
                Address regB = Utils::ReadHighNibble(split->low) >> 4;

                m_Registers[regA] = m_Registers[regB];
            } break;
            case LDM:
            {
                Address reg = Utils::ReadLowNibble(split->high);

                const Data loadData = m_Cartridge.ReadData(m_DataCtx, split->low);

                m_Registers[reg] = loadData;
            } break;
            case STM:
            {
                Address reg = Utils::ReadLowNibble(split->high);

                Data &storeData = m_Cartridge.GetData(m_DataCtx, split->low);

                storeData = m_Registers[reg];
            } break;
            case BTG:
            {
                Utils::WriteHighNibble(m_BranchTarget, Utils::ReadLowNibble(split->high));
                Utils::WriteLowNibble(m_BranchTarget, Utils::ReadHighNibble(split->low) >> 4);
            } break;
            case BRC:
            {
                Address regA = Utils::ReadLowNibble(split->high);
                Address regB = Utils::ReadHighNibble(split->low) >> 4;
                Address op = Utils::ReadLowNibble(split->low);

                if (BranchCompare(m_Registers[regA], m_Registers[regB], op))
                {
                    m_NextPC = m_BranchTarget;
                }
            } break;
            case JMP:
            {
                Utils::WriteHighNibble(m_NextPC, Utils::ReadLowNibble(split->high));
                Utils::WriteHighNibble(m_NextPC, Utils::ReadHighNibble(split->low) >> 4);
            } break;
            case SYS:
            {} break;
            case CTX:
            {
                Address ctxID = Utils::ReadLowNibble(split->high);
                Address ctxVal = Utils::ReadHighNibble(split->low) >> 4;

                switch(ctxID)
                {
                    case 0:
                    {
                        m_RamCtx = ctxVal;
                    } break;
                    case 1:
                    {
                        m_InstCtx = ctxVal;
                    } break;
                    case 2:
                    {
                        m_DataCtx = ctxVal;
                    } break;
                    case 3:
                    {
                        m_BackCtx = ctxVal;
                    } break;
                    case 4:
                    {
                        m_ForeCtx = ctxVal;
                    } break;
                    default: break;
                }
            } break;
            default: break;
        }
    }
}
