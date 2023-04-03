#include "sym_table.h"

map<string , struct symbol_details> symbol_table ;

pair<define_check , symbol_type> symbol_lookup(char* name) 
{
    if(symbol_table.find(name) != symbol_table.end())       // symbol is present
    {
        return make_pair(symbol_table[name].check , symbol_table[name].type);
    }
    else    // not found
    {
        return make_pair(UNDECL , VAR_SYM);
    }
}

define_check insert_symbol(char* name , symbol_type type , define_check check , symbol_value_type sym_val_type ,  int size)
{
    define_check sym_check = (symbol_lookup(name)).first;

    if ( sym_check == UNDECL )
    {  
        struct symbol_details newSymbol;
        newSymbol.check = check ;
        newSymbol.type = type ;
        newSymbol.sym_val_type = sym_val_type;
        
        if(newSymbol.type == VAR_ARRAY_SYM)
        {
            newSymbol.size = size;
            newSymbol.array_elements = (int*)malloc(size*sizeof(int));

            if(newSymbol.array_elements == NULL)
            {
                cerr << "Memory not available to allocate\n" << endl ;
		        exit(0);  
            }
            newSymbol.check = DEF;      //UNDEF => not possible in case of arrays
            // intialising with 0
            for (int i = 0 ; i<size ; i++)
            {
                *(newSymbol.array_elements + i) = 0;
            }
        }
        else
        {
            newSymbol.size = 0;
            newSymbol.array_elements = NULL;
        }
        symbol_table.insert({name , newSymbol}) ;
    }
    return sym_check ;     
    // UNDEF , DEF => insertion not possible
    // UNDECL => insertion done now
} 


define_check update_symbol_details(char* name , int val , int index)
{
    define_check sym_check = symbol_lookup(name).first ;
    // cout << "updated" << endl;

    if ( sym_check != UNDECL )   
    {
        struct symbol_details sym = symbol_table[name];

        if(sym.sym_val_type == BOOL_SYM)
        {
            if(val >= 1)
            {
                val = 1;
            }
            else
            {
                val = 0;
            }
        }

        if(sym.type == VAR_SYM)
        {
            symbol_table[name].value = val;
            symbol_table[name].check = DEF ;
        }
        else if(sym.type == VAR_ARRAY_SYM)
        {
            if(index > symbol_table[name].size)
            {
                cerr << "Index out of bounds error" << endl;
                exit(0);
            }
            *(symbol_table[name].array_elements + index) = val ;
        }
    }
    return sym_check;      
    // UNDECL => updation not possible
    // DEF , UNDEF : updation done now
}
