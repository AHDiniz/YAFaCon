#include "builder/assembler.h"

namespace YafaBuild
{
    void Assemble(const char *filename, uint16_t *data)
    {
        FILE *f = fopen(filename, "r");

        char c = getc(f);
        
        
        while (c != EOF)
        {

        }

        fclose(f);
    }
}
