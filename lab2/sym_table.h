#ifndef SYM_TABLE_H
#define SYM_TABLE_H
#include <iostream>
#include <map>
using namespace std ;

enum symbol_type { INTEGER_SYM = 0 , FUNC = 1 } ;
enum define_check { DEF = 1 , UNDEF = 0 , UNDECL = 2 } ;

struct symbol_details
{   
    symbol_type type ;
    define_check check ;        
    char* return_type ; 
    int val ;           /* In this lab only integer variables are to declared. So corresponding values to that will be stored */
};

// SYMBOL TABLE : map between variable/fun names to tits details
extern map<char* , struct symbol_details> symbol_table ;


/* Symbol Table functions */

// lookup - if symbol is present then it returns its details else returns empty symbol with undecl type 
struct symbol_details* symbol_lookup(char* name) ;

// insertion into symbol table
void insert_symbol(char* name , symbol_type type , define_check check , char const* return_type , int val);

// updating symbol table
void update_symbol_details(char* name , define_check check , int val);

#endif