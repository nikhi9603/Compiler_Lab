#include "sym_table.h"

map<string , struct symbol_details> symbol_table ;

struct symbol_details* symbol_lookup(char* name) 
{
    if(symbol_table.find(name) != symbol_table.end())       // symbol is present
    {
        struct symbol_details *s;
        s = &symbol_table[name];
        return s ;
    }
    else    // not found
    {
        struct symbol_details *empty_symbol=(struct symbol_details*)malloc(sizeof(struct symbol_details));

        if(empty_symbol == NULL)
        {
            cerr << "Memory not available to allocate\n" << endl ;
		    exit(0);    
        }
        empty_symbol->check = UNDECL ;
        return empty_symbol;
    }
}

void insert_symbol(char* name , symbol_type type , define_check check , char const* return_type , int val)
{
    struct symbol_details* sym = symbol_lookup(name) ;

    if ( sym->check != UNDECL )
    {
       cerr << "Undeclared variable " << name << endl ;
       exit(0);
    }
    else
    {  
        struct symbol_details newSymbol;
        newSymbol.check = check ;
        newSymbol.type = type ;
        newSymbol.return_type = (char *)return_type;
        newSymbol.val = val ;

        symbol_table.insert({name , newSymbol}) ;
        sym = symbol_lookup(name);
    }
  
    return ;
}

void update_symbol_details(char* name , int val)
{
    struct symbol_details* sym = symbol_lookup(name) ;

    if ( sym->check == UNDECL )
    {
       cerr << "Undeclared variable " << name << endl ;
       exit(0);
    }
    else
    {
        symbol_table[name].check = DEF ;
        symbol_table[name].val = val;
        return ;
    }
}
