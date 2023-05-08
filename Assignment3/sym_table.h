#ifndef SYM_TABLE_H
#define SYM_TABLE_H
#include <iostream>
#include <vector>
#include <map>
#include<string>
using namespace std ;

enum symbol_type { VAR_SYM = 0 , VAR_ARRAY_SYM = 1 , FUNC_SYM = 2 } ;
enum define_check { DEF = 1 , UNDEF = 0 , UNDECL = 2 } ;
enum symbol_value_type { INT_SYM = 0 , BOOL_SYM = 1};
enum scope {GLOBAL = 0 , LOCAL = 1};

struct parameters_list
{
    symbol_value_type sym_val_type;     // only variables are accepted by grammmar
    struct parameters_list *next;
};

struct symbol_details
{   
    symbol_type type ;
    define_check check ;        
    symbol_value_type sym_val_type ;
    int value ;         
    int size ; 
    int* array_elements ; 
    struct parameters_list *parameters;
    // struct stmt_list* func_code;       // Not needed as we are converting into C code for evaluation
}; 

// SYMBOL TABLE : map between variable/fun names to its details
extern map<string, map<string , struct symbol_details>> Symbol_table;

/* Symbol Table functions - lookup , insert , update */

// lookup - if symbol is present then it returns its details else returns empty symbol with undecl type 
tuple<define_check , symbol_type , scope> symbol_lookup(char* func_name , char* var_name) ;

// insertion into symbol table 
define_check insert_symbol(char* func_name , char* var_name , symbol_type type , symbol_value_type sym_val_type ,  int size , struct parameters_list* param_list);

// updating symbol table
// define_check update_symbol_details(char* func_name , char* var_name , int val , int index);

#endif