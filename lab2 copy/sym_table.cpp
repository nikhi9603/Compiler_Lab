#include "sym_table.h"

struct symbol_details* symbol_lookup(char* name) 
{
    if(symbol_table.find(name) != symbol_table.end())       // symbol is present
    {
        return &symbol_table[name] ;
    }
    else    // not found
    {
        struct symbol_details *empty_symbol=(struct symbol_details*)malloc(sizeof(struct symbol_details));

        if(empty_symbol == NULL)
        {
            printf("Memory not available to allocate\n");
		    exit(-1);    
        }

        empty_symbol->check = UNDECL ;
        return empty_symbol;
    }
}

void insert_symbol(char* name , symbol_type type , define_check check , char* return_type , int val)
{
    struct symbol_details* sym = symbol_lookup(name) ;

    if ( sym->check == UNDECL )
    {
       cerr << "Undeclared variable" << name << endl ;
    }
    else
    {  
        struct symbol_details newSymbol;
        newSymbol.check = check ;
        newSymbol.type = type ;
        newSymbol.return_type = return_type;
        newSymbol.val = val ;

        symbol_table.insert({name , newSymbol}) ;
    }
  
    return ;
}

void update_symbol_details(char* name , define_check check , int val)
{
    struct symbol_details* sym = symbol_lookup(name) ;

    if ( sym->check == UNDECL )
    {
       cerr << "Undeclared variable" << name << endl ;
    }
    else
    {
        sym->check = DEF ;
        sym->val = val ;
        return ;
    }
}
