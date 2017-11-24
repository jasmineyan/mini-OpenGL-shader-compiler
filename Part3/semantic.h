#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"
#include "symbol.h"

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

int check_args(node *ast, bool is_func, int func_name, int input_scope);
int semantic_check( node *ast);
int check_args_num(node *ast, int count);
int check_args_child(node *ast, bool is_func, int func_name, int input_scope);
#endif
