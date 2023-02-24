#include "sym_table.h"
struct sym_node symbol_table[100];


int symTable_lookup(char sym_name[20])
{
    int mark = 0;
    for(int i = 0 ; i < 100 ; i++)
    {
        if(strcmp(symbol_table[i].name , sym_name) == 0)
        {
            return 1;
        }
    }
}
