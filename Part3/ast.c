#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"

#define DEBUG_PRINT_TREE 0

int scope_print = 0;
node *ast = NULL;

node *ast_allocate(node_kind kind, ...) {
  va_list args;

  // make the node
  node *ast = (node *) malloc(sizeof(node));
  memset(ast, 0, sizeof *ast);
  ast->kind = kind;

  va_start(args, kind); 

  switch(kind) {
  
  case SCOPE_NODE:
	  ast->scope.declarations = va_arg(args, node *);
	  ast->scope.statements = va_arg(args, node *);
	  break;
  //case EXPRESSION_NODE:
  	  //break;
  case UNARY_EXPRESION_NODE:
	  ast->unary_expr.op = va_arg(args, int);
	  ast->unary_expr.right = va_arg(args, node *);
  	  break;
  case BINARY_EXPRESSION_NODE:
	  ast->binary_expr.op = va_arg(args, int);
	  ast->binary_expr.left = va_arg(args, node *);
	  ast->binary_expr.right = va_arg(args, node *);
  	  break;
  case INT_NODE:
	  ast->type = INT;
	  ast->int_value = va_arg(args, int);
  	  break;
  case FLOAT_NODE:
	  ast->type = FLOAT;
	  ast->float_value = va_arg(args, double);
  	  break;
  case BOOL_NODE:
	  ast->type = BOOL;
	  ast->bool_value = va_arg(args, int);
  	  break;
  case VAR_NODE:
	  ast->variable.is_array = va_arg(args, int);
	  ast->variable.id = va_arg(args, char *);
	  ast->variable.index =  va_arg(args, int);
  	  break;
  case FUNCTION_NODE:
	  ast->function_call.name = va_arg(args, int);
	  ast->function_call.arguments = va_arg(args, node *);
  	  break;
  case CONSTRUCTOR_NODE:
	  ast->constructor.type = va_arg(args, node *);
	  ast->constructor.arguments = va_arg(args, node *);
  	  break;
  case STATEMENT_NODE:
	  ast->statements.statements = va_arg(args, node *);
	  ast->statements.statement = va_arg(args, node *);
  	  break;
  case IF_STATEMENT_NODE:
	  ast->statement_if_else.condition_expression = va_arg(args, node *);
	  ast->statement_if_else.if_statement = va_arg(args, node *);
	  ast->statement_if_else.else_statement = va_arg(args, node *);
  	  break;
  case WHILE_STATEMENT_NODE:
	  ast->statement_while.condition_expression = va_arg(args, node *);
	  ast->statement_while.while_statement = va_arg(args, node *);
  	  break;
  case ASSIGNMENT_NODE:
	  ast->statement_assign.variable = va_arg(args, node *);
	  ast->statement_assign.expression = va_arg(args, node *);
	  break;
  case NESTED_SCOPE_NODE:
	  ast->nested_scope = va_arg(args, node *);
	  break;
  case DECLARATIONS_NODE:
	  ast->declarations.declarations = va_arg(args, node *);
	  ast->declarations.declaration = va_arg(args, node *);
	  break;
  case DECLARATION_NODE:
	  ast->declaration.is_const = va_arg(args, int);
	  ast->declaration.type = va_arg(args, node *);
	  ast->declaration.id = va_arg(args, char *);
	  ast->declaration.expression = va_arg(args, node *);
	  break;
  case TYPE_NODE:
	  ast->type_node.type_name = va_arg(args, int);
	  break;
  case NESTED_EXPRESSION_NODE:
  	  ast->nested_expr.expr = va_arg(args, node *);
  	  break;
  case EXP_VAR_NODE:
  	  ast->expr_variable.variable = va_arg(args, node *);
  	  break;
  case ARGUMENTS_NODE:
  	  ast->arguments.arg = va_arg(args, node *);
  	  ast->arguments.expr = va_arg(args, node *);
  	  break;
  default: break;
  }

  va_end(args);

  return ast;
}
char* function_name_to_string (int name) {
	switch (name) {
	case 0:
		return "DP3";
	case 1:
		return "LIT";
	case 2:
		return "RSQ";
	default:
		return "unknown function";
	}
}

char* type_to_string (int type) {
	switch (type) {
	case INT:
		return "int";
	case BOOL:
		return "bool";
	case FLOAT:
		return "float";
	case VEC2:
		return "vec2";
	case VEC3:
		return "vec3";
	case VEC4:
		return "vec4";
	case IVEC2:
		return "ivec2";
	case IVEC3:
		return "ivec3";
	case IVEC4:
		return "ivec4";
	case BVEC2:
		return "bvec2";
	case BVEC3:
		return "bvec3";
	case BVEC4:
		return "bvec4";
	case UNKOWN_TYPE:
	default:
		return "unknown type";
	}
}

char* op_to_string(int op){
	switch(op){
	case AND:
		return "&&";
	case OR:
		return "||";
	case EQ:
		return "==";
	case NEQ:
		return "!=";
	case '<':
		return "<";
	case LEQ:
		return "<=";
	case '>':
		return ">";
	case GEQ:
		return ">=";
	case '+':
		return "+";
	case '-':
		return "-";
	case '*':
		return "*";
	case '/':
		return "/";
	case '^':
		return "^";
	case '!':
		return "!";
	default:
		return "unknown operation";
	}
}

void ast_free(node *ast) {

	if(ast == NULL)
		return;

	switch(ast->kind) {
	case SCOPE_NODE:
		ast_free(ast->scope.declarations);
		ast_free(ast->scope.statements);
		free(ast);
		ast=NULL;
		break;
	case STATEMENT_NODE:
		ast_free(ast->statements.statements);
		ast_free(ast->statements.statement);
		free(ast);
		ast=NULL;
		break;
	case BINARY_EXPRESSION_NODE:
		ast_free(ast->binary_expr.left);
		ast_free(ast->binary_expr.right);
		free(ast);
		ast=NULL;
		break;
	case UNARY_EXPRESION_NODE:
		ast_free(ast->unary_expr.right);
		free(ast);
		ast=NULL;
		break;
	case ASSIGNMENT_NODE:
		ast_free(ast->statement_assign.variable);
		ast_free(ast->statement_assign.expression);
		free(ast);
		ast=NULL;
		break;
	case IF_STATEMENT_NODE:
		ast_free(ast->statement_if_else.condition_expression);
		ast_free(ast->statement_if_else.if_statement);
		ast_free(ast->statement_if_else.else_statement);
		free(ast);
		ast=NULL;
		break;
	case CONSTRUCTOR_NODE:
		ast_free(ast->constructor.type);
		ast_free(ast->constructor.arguments);
		free(ast);
		ast=NULL;
		break;
	case FUNCTION_NODE:
		ast_free(ast->function_call.arguments);
		free(ast);
		ast=NULL;
		break;
	case DECLARATIONS_NODE:
		ast_free(ast->declarations.declaration);
		ast_free(ast->declarations.declarations);
		free(ast);
		ast=NULL;
		break;
	case DECLARATION_NODE:
		free(ast->declaration.id);
		ast_free(ast->declaration.type);
		ast_free(ast->declaration.expression);
		free(ast);
		ast=NULL;
		break;
	case INT_NODE:
		free(ast);
		ast=NULL;
		break;
	case FLOAT_NODE:
		free(ast);
		ast=NULL;
		break;
	case BOOL_NODE:
		free(ast);
		ast=NULL;
		break;
	case VAR_NODE:
		free(ast->variable.id);
		free(ast);
		ast=NULL;
		break;
	case TYPE_NODE:
		free(ast);
		ast=NULL;
		break;
    case NESTED_EXPRESSION_NODE:
	  	ast_free(ast->nested_expr.expr);
	  	free(ast);
	  	ast=NULL;
	  	break;
	case EXP_VAR_NODE:
	    ast_free(ast->expr_variable.variable);
	    free(ast);
	    ast=NULL;
	    break;
	case ARGUMENTS_NODE:
		ast_free(ast->arguments.arg);
		ast_free(ast->arguments.expr);
		free(ast);
		ast=NULL;
		break;
	default:
		break;
	}
}

void get_type(node *ast, node_complete_type *result, int input_scope) {
	if(ast == NULL) {
		result->is_index_vector = false;
		result->type = UNKOWN_TYPE;
		result->type_class = NONE;
		return;
	}

	switch(ast->kind) {
	case SCOPE_NODE:
		input_scope++;
		get_type(ast->scope.declarations, result, input_scope);
		get_type(ast->scope.statements, result, input_scope);
		input_scope--;
		break;
	case STATEMENT_NODE:
		get_type(ast->statements.statements, result, input_scope);
		get_type(ast->statements.statement, result, input_scope);
		break;
	case BINARY_EXPRESSION_NODE:
		get_type(ast->binary_expr.left, result, input_scope);
		get_type(ast->binary_expr.right, result, input_scope);
		break;
	case UNARY_EXPRESION_NODE:
		get_type(ast->unary_expr.right, result, input_scope);
		break;
	case ASSIGNMENT_NODE:
		get_type(ast->statement_assign.variable, result, input_scope);
		get_type(ast->statement_assign.expression, result, input_scope);
		break;
	case IF_STATEMENT_NODE:
		get_type(ast->statement_if_else.condition_expression, result, input_scope);
		get_type(ast->statement_if_else.if_statement, result, input_scope);
		get_type(ast->statement_if_else.else_statement, result, input_scope);
		break;
	case CONSTRUCTOR_NODE:
		get_type(ast->constructor.type, result, input_scope);
		get_type(ast->constructor.arguments, result, input_scope);
		break;
	case FUNCTION_NODE:
		if (true) {
			result->is_index_vector = false;
			result->type = UNKOWN_TYPE;
			result->type_class = NONE;
			node_complete_type arg_type;
			get_type(ast->function_call.arguments, &arg_type, input_scope);
			if (ast->function_call.name == 1) {//LIT
				if (arg_type.type == VEC4) {
					result->type = VEC4;
					result->type_class = FUNCTION;
				}
			} else if (ast->function_call.name == 0) {//DP3
				if (arg_type.type == VEC4 || arg_type.type == VEC3) {
					result->type = FLOAT;
					result->type_class = FUNCTION;
				} else if (arg_type.type == IVEC4 || arg_type.type == IVEC3) {
					result->type = INT;
					result->type_class = FUNCTION;
				}
			} else if (ast->function_call.name == 2) {//RSQ
				if (arg_type.type == FLOAT || arg_type.type == INT) {
					result->type = FLOAT;
					result->type_class = FUNCTION;
				}
			}
			return;
		}
		break;
	case DECLARATIONS_NODE:
		get_type(ast->declarations.declaration, result, input_scope);
		get_type(ast->declarations.declarations, result, input_scope);
		break;
	case DECLARATION_NODE:
		get_type(ast->declaration.type, result, input_scope);
		get_type(ast->declaration.expression, result, input_scope);
		break;
	case INT_NODE:
		if (true) {
			result->is_index_vector = false;
			result->type = INT;
			result->type_class = LITERAL;
			return;
		}
		break;
	case FLOAT_NODE:
		if (true) {
			result->is_index_vector = false;
			result->type = FLOAT;
			result->type_class = LITERAL;
			return;
		}
		break;
	case BOOL_NODE:
		if (true) {
			result->is_index_vector = false;
			result->type = BOOL;
			result->type_class = LITERAL;
			return;
		}
		break;
	case VAR_NODE:
		if (true)
		{
			result->is_index_vector = false;
			result->type = UNKOWN_TYPE;
			result->type_class = NONE;
		    symbol_node* var_symbol_node = valid_scope_node(ast->variable.id, input_scope);
			if (var_symbol_node) {
				result->type = var_symbol_node->type;
				result->is_index_vector = ast->variable.is_array;
				result->type_class = var_symbol_node->type_class;
			}
			return;
		}
		break;
	case TYPE_NODE:
		break;
	case NESTED_EXPRESSION_NODE:
		get_type(ast->nested_expr.expr, result, input_scope);
		break;
	case EXP_VAR_NODE:
		get_type(ast->expr_variable.variable, result, input_scope);
		break;
	case ARGUMENTS_NODE:
		get_type(ast->arguments.arg, result, input_scope);
		get_type(ast->arguments.expr, result, input_scope);
		break;
	default:
		break;
	}
}

void ast_print(node * ast, int print_index) {
    if (ast == NULL) {
   		 return;
    }

    switch (ast->kind) {
    case SCOPE_NODE:
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf("(SCOPE\n");
   	 scope_print++;
   	 ast_print(ast->scope.declarations,print_index+1);
   	 ast_print(ast->scope.statements,print_index+1);
   	 scope_print--;
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf(")\n");
   	 break;
    case UNARY_EXPRESION_NODE:
   	 if (true) {
   		 node_complete_type right_type;
   		 get_type(ast->unary_expr.right, &right_type, scope_print);

   		 if ((right_type.type == VEC2 || right_type.type == VEC3 || right_type.type == VEC4) && right_type.is_index_vector) {
   			 right_type.type = FLOAT;
   		 } else if ((right_type.type == IVEC2 || right_type.type == IVEC3 || right_type.type == IVEC4) && right_type.is_index_vector) {
   			 right_type.type = INT;
    	 } else if ((right_type.type == BVEC2 || right_type.type == BVEC3 || right_type.type == BVEC4) && right_type.is_index_vector) {
    		 right_type.type = BOOL;
    	 }

   		 printf("(UNARY %s %s ", type_to_string(right_type.type), op_to_string(ast->unary_expr.op));
   		 ast_print(ast->unary_expr.right,0);
   		 printf(")");
   	 }
   	 break;
    case BINARY_EXPRESSION_NODE:
   	 if (true) {
   		 node_complete_type left_type;
   		 get_type(ast->binary_expr.left, &left_type, scope_print);

   		 if ((left_type.type == VEC2 || left_type.type == VEC3 || left_type.type == VEC4) && left_type.is_index_vector) {
   			 left_type.type = FLOAT;
   		 } else if ((left_type.type == IVEC2 || left_type.type == IVEC3 || left_type.type == IVEC4) && left_type.is_index_vector) {
   			 left_type.type = INT;
    	 } else if ((left_type.type == BVEC2 || left_type.type == BVEC3 || left_type.type == BVEC4) && left_type.is_index_vector) {
    		 left_type.type = BOOL;
    	 }
   		 printf("(BINARY %s %s ", type_to_string(left_type.type), op_to_string(ast->binary_expr.op));
   		 ast_print(ast->binary_expr.left,0);
   		 ast_print(ast->binary_expr.right,0);
   		 printf(")");
   	 }
   	 break;
    case INT_NODE:
   	 printf("%d ", ast->int_value);
   	 break;
    case FLOAT_NODE:
   	 printf("%f ", ast->float_value);
   	 break;
    case BOOL_NODE:
   	 if (ast->bool_value == 1)
   		 printf("true ");
   	 else
   		 printf("false ");
   	 break;
    case VAR_NODE:
   	 if (true)
   	 {
   		 int variable_type = UNKOWN_TYPE;
   		 symbol_node* var_symbol_node = valid_scope_node(ast->variable.id, scope_print);
   		 if (var_symbol_node) {
   			 variable_type = var_symbol_node->type;
   		 }
   		 if (ast->variable.is_array == 1)
   		 {
   			if (variable_type == VEC2 || variable_type == VEC3 || variable_type == VEC4) {
				 variable_type = FLOAT;
			 } else if (variable_type == IVEC2 || variable_type == IVEC3 || variable_type == IVEC4) {
				 variable_type = INT;
			 } else if (variable_type == BVEC2 || variable_type == BVEC3 || variable_type == BVEC4) {
				 variable_type = BOOL;
			 }
   			 printf("(INDEX %s %s %d)", type_to_string(variable_type), ast->variable.id, ast->variable.index);
   		 }
   		 else
   		 {
   			 printf("%s", ast->variable.id);
   		 }
   	 }
   	 break;
    case FUNCTION_NODE:
   	 printf("(CALL %s ", function_name_to_string(ast->function_call.name));
   	 ast_print(ast->function_call.arguments,0);
   	 printf(")");
   	 break;
    case CONSTRUCTOR_NODE:
   	 printf("(CALL ");
   	 ast_print(ast->constructor.type,0);
   	 ast_print(ast->constructor.arguments,0);
   	 printf(")");
   	 break;
    case STATEMENT_NODE:
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf("(STATEMENTS\n");
   	 ast_print_child(ast->statements.statements,print_index+1);
   	 ast_print(ast->statements.statement,print_index+1);
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf(")\n");
   	 break;
    case IF_STATEMENT_NODE:
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf("(IF  ");
   	 ast_print(ast->statement_if_else.condition_expression,print_index+1);
   	 printf("\n");
   	 ast_print(ast->statement_if_else.if_statement,print_index+1);
   	 if (ast->statement_if_else.else_statement != NULL){
   	   	 if (ast->statement_if_else.else_statement->kind != SCOPE_NODE)
   	   	ast_print(ast->statement_if_else.else_statement,print_index+1);
   	 }
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf(")\n");
   	 break;
    case WHILE_STATEMENT_NODE:
   	 /* do nothing? */
   	 break;
    case ASSIGNMENT_NODE:
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 node_complete_type variable_type;
   	 get_type(ast->statement_assign.variable, &variable_type, scope_print);

   	 if ((variable_type.type == VEC2 || variable_type.type == VEC3 || variable_type.type == VEC4) && variable_type.is_index_vector) {
   		 variable_type.type = FLOAT;
   	 } else if ((variable_type.type == IVEC2 || variable_type.type == IVEC3 || variable_type.type == IVEC4) && variable_type.is_index_vector) {
   		 variable_type.type = INT;
   	 } else if ((variable_type.type == BVEC2 || variable_type.type == BVEC3 || variable_type.type == BVEC4) && variable_type.is_index_vector) {
   		 variable_type.type = BOOL;
   	 }

   	 printf("(ASSIGN %s ", type_to_string(variable_type.type));
   	 ast_print(ast->statement_assign.variable,0);
   	 ast_print(ast->statement_assign.expression,0);
   	 printf(")\n");
   	 break;
    case NESTED_SCOPE_NODE:
   	 /* do nothing? */
   	 break;
    case DECLARATIONS_NODE:
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf("(DECLARATIONS\n");
   	 ast_print_child(ast->declarations.declarations,print_index+1);
   	 ast_print(ast->declarations.declaration,print_index+1);
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf(")\n");
   	 break;
    case DECLARATION_NODE:
   	 for (int i=0; i<print_index; i++){
   		 printf("  ");
   	 }
   	 printf("(DECLARATION  ");
   	 printf("%s ", ast->declaration.id);
   	 ast_print(ast->declaration.type,print_index+1);
   	 ast_print(ast->declaration.expression,print_index+1);
   	 printf(")\n");
   	 break;
   case TYPE_NODE:
   	 printf("%s ", type_to_string(ast->type_node.type_name));
   	 break;
   case NESTED_EXPRESSION_NODE:
     	 break;
    case EXP_VAR_NODE:
   	 ast_print(ast->expr_variable.variable,0);
    	break;
    case ARGUMENTS_NODE:
   	 break;
    default:
   	 printf("DEFAULT\n");
   	 break;
    }
}


void ast_print_child(node * ast, int print_index){
    if(ast==NULL){
   	 return;
    }
    switch(ast->kind){
    case DECLARATIONS_NODE:
   	 ast_print_child(ast->declarations.declarations,print_index);
   	 ast_print(ast->declarations.declaration,print_index);
   	 break;
    case STATEMENT_NODE:
   	 ast_print_child(ast->statements.statements,print_index);
   	 ast_print(ast->statements.statement,print_index);
   	 break;
    }
}
