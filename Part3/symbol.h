#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum {
	NONE = 0,
	CONSTANT,
	ATTRIBUTE,
	UNIFORM,
	RESULT,
	FUNCTION,
	LITERAL
} node_type_class;

typedef struct symbol_node
{
    int type;
    char *id;
    int data;
    int scope;
    int type_class;
    int line_number;
    struct symbol_node *next;
} symbol_node;

void build_symbol_table (node *ast);
void free_symbol_table();
symbol_node* search(char* id, int scope);
symbol_node* valid_scope_node(char* id, int scope);
void add_predefined();
void print_all();

#endif
