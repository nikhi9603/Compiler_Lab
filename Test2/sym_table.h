#ifndef SYM_TABLE_H
#define SYM_TABLE_H
#include <iostream>
#include <vector>
#include <map>
#include<string>
using namespace std ;

enum symbol_type { VAR_SYM = 0 , VAR_ARRAY_SYM } ;
enum define_check { DEF = 1 , UNDEF = 0 , UNDECL = 2 } ;
enum symbol_value_type { INT_SYM = 0 , BOOL_SYM = 1};

struct symbol_details
{   
    symbol_type type ;
    define_check check ;        
    symbol_value_type sym_val_type ;
    int value ;
    int size ;
    struct symbol_details *next;
};

// SYMBOL TABLE : map between variable/fun names to tits details
extern map<string , struct symbol_details> symbol_table ;

/* Symbol Table functions - lookup , insert , update */

// lookup - if symbol is present then it returns its details else returns empty symbol with undecl type 
struct symbol_details* symbol_lookup(char* name) ;

// insertion into symbol table
void insert_symbol(char* name , symbol_type type , define_check check , char const* return_type , int val);

// updating symbol table
void update_symbol_details(char* name ,int val);

#endif