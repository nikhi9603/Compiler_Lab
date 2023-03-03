struct sym_node 
{
    /* Type 0 - variable */
    /* Type 1 - function */
    int type ;
    char name[20] ; 
    float value ;
};

int symTable_lookup(char sym_name[20]);

void insertSymbol(int sym_type , char sym_name[20] , float sym_value);