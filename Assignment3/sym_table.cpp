#include "sym_table.h"

map<string , map<string , struct symbol_details>> Symbol_table;

pair<define_check , symbol_type> symbol_lookup(char* func_name , char* var_name) 
{
    if (Symbol_table["global"].find(func_name) != Symbol_table["global"].end())
    {
        if(Symbol_table[func_name].find(var_name) != Symbol_table[func_name].end())       // symbol is present
        {
            return make_pair(Symbol_table[func_name][var_name].check , Symbol_table[func_name][var_name].type);
        }
        else    // not found
        {
            return make_pair(UNDECL , VAR_SYM);
        }
    }
    else    // such function only not exists
    {
        return make_pair(UNDECL , FUNC_SYM);
    }
}

define_check insert_symbol(char* func_name , char* var_name , symbol_type type , define_check check , symbol_value_type sym_val_type ,  int size)
{
    pair <define_check , symbol_type> sym_check = symbol_lookup(func_name , var_name) ;

    if( sym_check.first == UNDECL && sym_check.second == FUNC_SYM)
    {
        return UNDEF;       // CHECK AFTERWARDS
    }

    if ( sym_check.first == UNDECL )
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
        else if (newSymbol.type == VAR_SYM)
        {
            newSymbol.size = 0;
            newSymbol.array_elements = NULL;
        }
        else if (newSymbol.type == FUNC_SYM)
        {
            // HAVE TO UPDATE
        }
        Symbol_table[func_name].insert({var_name , newSymbol});
        // symbol_table.insert({name , newSymbol}) ;
    }
    return sym_check.first ;        // UPDATE NEEDED    
    // UNDEF , DEF => insertion not possible
    // UNDECL => insertion done now
} 


define_check update_symbol_details(char* func_name , char* var_name , int val , int index)
{ 
    pair <define_check , symbol_type> sym_check = symbol_lookup(func_name , var_name) ;

    if( sym_check.first == UNDECL && sym_check.second == FUNC_SYM)
    {
        return UNDEF;       // CHECK AFTERWARDS
    }

    if ( sym_check.first != UNDECL )   
    {
        struct symbol_details sym = Symbol_table[func_name][var_name];

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
            Symbol_table[func_name][var_name].value = val;
            Symbol_table[func_name][var_name].check = DEF ;
        }
        else if(sym.type == VAR_ARRAY_SYM)
        {
            if(index > Symbol_table[func_name][var_name].size)
            {
                cerr << "Index out of bounds error" << endl;
                exit(0);
            }
            *(Symbol_table[func_name][var_name].array_elements + index) = val ;
        }
        else
        {
            // CHECK IF NEEDED FOR FUNCTIO SYMBOL
        }
    }
    return sym_check.first;         // UPDATE NEEDED     
    // UNDECL => updation not possible
    // DEF , UNDEF : updation done now
}
