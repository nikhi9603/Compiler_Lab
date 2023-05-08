#include "sym_table.h"
#include <cstring>
map<string , map<string , struct symbol_details>> Symbol_table;


// SYMBOL TABLE LOOKUP
tuple<define_check , symbol_type , scope> symbol_lookup(char* func_name , char* var_name) 
{
    // cout << "Symbol_table : " << func_name << " " << var_name << endl;
    if(Symbol_table.find("global") == Symbol_table.end())
    {
        Symbol_table.insert(make_pair("global" , map<string , struct symbol_details>()));
    }

    if( strcmp(func_name, "global") != 0 )
    {
        if(Symbol_table[func_name].find(var_name) != Symbol_table[func_name].end())       // symbol is present
        {
            // cout << "case 1   " << func_name << " " << var_name << endl;
            return make_tuple(Symbol_table[func_name][var_name].check , Symbol_table[func_name][var_name].type , LOCAL);
        }
    }

    if(Symbol_table["global"].find(var_name) != Symbol_table["global"].end())
    {
            // cout << "case 2   " << func_name << " " << var_name << endl;
        return make_tuple(Symbol_table["global"][var_name].check , Symbol_table["global"][var_name].type , GLOBAL);
    }

    // if such var_name does not exist in both local and global scope 
            // cout << "case 3   " << func_name << " " << var_name << endl;
    return make_tuple(UNDECL , VAR_SYM , GLOBAL);     // when undeclared, type of symbol , scope need not matter
}

// INSERTION INTO SYMBOL TABLE
define_check insert_symbol(char* func_name , char* var_name , symbol_type type , symbol_value_type sym_val_type ,  int size=0 , struct parameters_list* param_list=NULL)
{
    tuple <define_check , symbol_type , scope> sym_check = symbol_lookup(func_name , var_name) ;

    if ( get<0>(sym_check) == UNDECL || ((strcmp(func_name , "global") != 0) && get<2>(sym_check) == GLOBAL))
    {  
        struct symbol_details newSymbol;
        newSymbol.check = UNDEF ;
        newSymbol.type = type ;
        newSymbol.sym_val_type = sym_val_type;
        newSymbol.value = 0;
        
        if(newSymbol.type == VAR_ARRAY_SYM)
        {
            newSymbol.size = size;
            newSymbol.array_elements = (int*)malloc(size*sizeof(int));
            newSymbol.parameters = NULL;

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
            newSymbol.parameters = NULL;
        }
        else if (newSymbol.type == FUNC_SYM)
        {
            newSymbol.size = 0;
            newSymbol.array_elements = NULL;
            newSymbol.parameters = param_list;
        }

        Symbol_table[func_name].insert({var_name , newSymbol});

        // creatinf a new empty map for functions in symbol table
        if (newSymbol.type == FUNC_SYM)
        {
            Symbol_table.insert(make_pair(var_name , map<string , struct symbol_details>()));
        }
        return UNDECL;
    }

    return get<0>(sym_check) ;          
    // UNDEF , DEF => insertion not possible
    // UNDECL => insertion done now
} 


// UPDATING SYMBOL TABLE
// define_check update_symbol_details(char* func_name , char* var_name , int val , int index=0)
// { 
//     tuple <define_check , symbol_type , scope> sym_check = symbol_lookup(func_name , var_name) ;

//     if( get<0>(sym_check) == UNDECL )
//     {
//         return UNDECL;       
//     }

//     if ( strcmp(func_name, "global") != 0 && get<2>(sym_check) == GLOBAL )   
//     {
//         func_name = "global";
//     }

//     struct symbol_details sym = Symbol_table[func_name][var_name];

//     if(sym.sym_val_type == BOOL_SYM)
//     {
//         if(val >= 1)
//         {
//             val = 1;
//         }
//         else
//         {
//             val = 0;
//         }
//     }

//     if(sym.type == VAR_SYM)
//     {
//         Symbol_table[func_name][var_name].value = val;
//         Symbol_table[func_name][var_name].check = DEF ;
//     }
//     else if(sym.type == VAR_ARRAY_SYM)
//     {
//         if(index > Symbol_table[func_name][var_name].size)
//         {
//             cerr << "Index out of bounds error" << endl;
//             exit(0);
//         }
//         *(Symbol_table[func_name][var_name].array_elements + index) = val ;
//     }

//     return get<0>(sym_check) ;             
//     // UNDECL => updation not possible
//     // DEF , UNDEF : updation done now
// }
