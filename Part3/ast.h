
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

#include "symbol.h"

// Dummy node just so everything compiles, create your own node/nodes
//
// The code provided below is an example ONLY. You can use/modify it,
// but do not assume that it is correct or complete.
//
// There are many ways of making AST nodes. The approach below is an example
// of a descriminated union. If you choose to use C++, then I suggest looking
// into inheritance.

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum {
  UNKNOWN               = 0,

  SCOPE_NODE            = (1 << 0),
  
  EXPRESSION_NODE       = (1 << 2),
  UNARY_EXPRESION_NODE  = (1 << 2) | (1 << 3),
  BINARY_EXPRESSION_NODE= (1 << 2) | (1 << 4),
  INT_NODE              = (1 << 2) | (1 << 5), 
  FLOAT_NODE            = (1 << 2) | (1 << 6),
  BOOL_NODE             = (1 << 2) | (1 << 7),
  VAR_NODE              = (1 << 2) | (1 << 8),
  FUNCTION_NODE         = (1 << 2) | (1 << 9),
  CONSTRUCTOR_NODE      = (1 << 2) | (1 << 10),

  STATEMENT_NODE        = (1 << 1),
  IF_STATEMENT_NODE     = (1 << 1) | (1 << 11),
  WHILE_STATEMENT_NODE  = (1 << 1) | (1 << 12),
  ASSIGNMENT_NODE       = (1 << 1) | (1 << 13),
  NESTED_SCOPE_NODE     = (1 << 1) | (1 << 14),

  DECLARATION_NODE      = (1 << 15),

  TYPE_NODE     		= (1 << 16),
  NESTED_EXPRESSION_NODE= (1 << 17),
  EXP_VAR_NODE  		= (1 << 18),
  ARGUMENTS_NODE 		= (1 << 19),
  DECLARATIONS_NODE     = (1 << 20)

} node_kind;

typedef enum {
	UNKOWN_TYPE,
	INT,
	BOOL,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	BVEC2,
	BVEC3,
	BVEC4,
	IVEC2,
	IVEC3,
	IVEC4,
	IDENTIFIER
} node_type;

typedef struct node_complete_type
{
	int type_class;
	int type;
	bool is_index_vector;
} node_complete_type;


/*
typedef enum {
	AND,
	OR,
	EQ,
	NEQ,
	LT,
	LEQ,
	GT,
	GEQ,
	ADD,
	SUB,
	MULT,
	DIV,
	POW
} op_type;

typedef enum {
	LIT,
	DP3,
	RSQ
} function_name;
*/

struct node_ {

  // an example of tagging each node with a type
  node_kind kind;
  int type;

  int int_value;
  float float_value;
  int bool_value;

  union {
	/* statement forms */
    struct {
    	node *declarations;
    	node *statements;
    } scope;

    node *nested_scope;

    struct {
       	node *declarations;
       	node *declaration;
    } declarations;

    struct {
        node *statements;
        node *statement;
    } statements;

    // declaration
    struct {
    	int is_const;
    	node *type;
        char *id;
        node *expression;
    } declaration;

    // statement
    struct {
    	node *variable;
    	node *expression;
    } statement_assign;

    struct {
        node *condition_expression;
        node *if_statement;
        node *else_statement;
    }statement_if_else;

    struct {
		node *condition_expression;
		node *while_statement;
    }statement_while;
  
    /* expression forms */
    struct {
    	int op;
    	node *right;
    } unary_expr;

    struct {
    	int op;
    	node *left;
    	node *right;
    } binary_expr;

    struct {
    	int is_array;
    	char *id;
    	int index;
    } variable;

    struct {
    	int name;
    	node *arguments;
    } function_call;

    struct {
    	node *type;
    	node *arguments;
    } constructor;

    struct {
    	int type_name;
    }type_node;

    struct {
    	node *expr;
    }nested_expr;

    struct {
    	node *variable;
    }expr_variable;

    struct {
    	node *arg;
    	node *expr;
    }arguments;

  };
};

void get_type(node *ast, node_complete_type *result, int input_scope);
node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast, int print_index);
char* function_name_to_string (int name);
char* type_to_string (int type);
void ast_print_child(node * ast, int print_index);

#endif /* AST_H_ */
