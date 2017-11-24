#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "common.h"


symbol_node *global_head;
int scope_symbol_table = 0;

symbol_node* create_node(int type, char* id, int data, int scope,
		int type_class, int line_number, symbol_node* next)
{
    symbol_node* new_symbol_node = (symbol_node*)malloc(sizeof(symbol_node));
    if(new_symbol_node == NULL)
    {
        printf("Error creating a new symbol node.\n");
        exit(0);
    }
    new_symbol_node->type = type;
    new_symbol_node->id = id;
    new_symbol_node->data = data;
    new_symbol_node->scope = scope;
    new_symbol_node->type_class = type_class;
    new_symbol_node->line_number = line_number;
    new_symbol_node->next = next;

    return new_symbol_node;
}

symbol_node* add_to_beginning(int type, char* id, int data, int scope,
		int type_class, int line_number, symbol_node* head)
{
    symbol_node* new_symbol_node = create_node(type, id, data, scope,
    		type_class, line_number, head);
    head = new_symbol_node;
    return head;
}

symbol_node* add_to_end(int type, char* id, int data, int scope,
		int type_class, int line_number, symbol_node* head)
{
    if(head == NULL)
        return NULL;
    /* go to the last node */
    symbol_node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;

    /* create a new node */
    symbol_node* new_node =  create_node(type, id, data, scope,
    		type_class, line_number, NULL);
    cursor->next = new_node;

    return head;
}

symbol_node* search(char* id, int scope)
{

    symbol_node *cursor = global_head;
    while(cursor != NULL)
    {
        if((strcmp(cursor->id, id) == 0) && (cursor->scope == scope))
            return cursor;
        cursor = cursor->next;
    }
    return NULL;
}

symbol_node* valid_scope_node(char* id, int scope)
{

    symbol_node *cursor = global_head;
    while(cursor != NULL)
    {
        if((strcmp(cursor->id, id) == 0) && (cursor->scope <= scope))
            return cursor;
        cursor = cursor->next;
    }
    return NULL;
}

void free_symbol_table()
{
	symbol_node* head = global_head;
	symbol_node *cursor, *tmp;

    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while(cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}

void add_predefined()
{
	if (global_head == NULL)
		return;

	global_head = add_to_beginning(VEC4, "gl_FragColor", 0, 0, RESULT, 0, global_head);
	global_head = add_to_beginning(BOOL, "gl_FragDepth", 0, 0, RESULT, 0, global_head);
	global_head = add_to_beginning(VEC4, "gl_FragCoord", 0, 0, RESULT, 0, global_head);

	global_head = add_to_beginning(VEC4, "gl_TexCoord", 0, 0, ATTRIBUTE, 0, global_head);
	global_head = add_to_beginning(VEC4, "gl_Color", 0, 0, ATTRIBUTE, 0, global_head);
	global_head = add_to_beginning(VEC4, "gl_Secondary", 0, 0, ATTRIBUTE, 0, global_head);
	global_head = add_to_beginning(VEC4, "gl_FogFragCoord", 0, 0, ATTRIBUTE, 0, global_head);

	global_head = add_to_beginning(VEC4, "gl_Light_Half", 0, 0, UNIFORM, 0, global_head);
	global_head = add_to_beginning(VEC4, "gl_Light_Ambient", 0, 0, UNIFORM, 0, global_head);
	global_head = add_to_beginning(VEC4, "gl_Material_Shininess", 0, 0, UNIFORM, 0, global_head);

	global_head = add_to_beginning(VEC4, "env1", 0, 0, UNIFORM, 0, global_head);
	global_head = add_to_beginning(VEC4, "env2", 0, 0, UNIFORM, 0, global_head);
	global_head = add_to_beginning(VEC4, "env3", 0, 0, UNIFORM, 0, global_head);
}

void print_all()
{
	printf("****************************************\n");
	int i = 0;
	symbol_node* cursor = global_head;
    while(cursor != NULL)
    {
    	i++;
        printf("%d) type: %d, id: %s, data: %d, scope: %d, "
        		"type class: %d, line number: %d\n",
        			i, cursor->type, cursor->id, cursor->data, cursor->scope,
					cursor->type_class, cursor->line_number);
        cursor = cursor->next;
    }
    printf("****************************************\n");
}

void build_symbol_table (node *ast)
{
	if (ast == NULL)
		return;
	switch (ast->kind) {
	case SCOPE_NODE:
		scope_symbol_table++;
		build_symbol_table(ast->scope.declarations);
		build_symbol_table(ast->scope.statements);
		scope_symbol_table--;
		break;
	//case EXPRESSION_NODE:
	//break;
	case UNARY_EXPRESION_NODE:
		build_symbol_table(ast->unary_expr.right);
		break;
	case BINARY_EXPRESSION_NODE:
		build_symbol_table(ast->binary_expr.left);
		build_symbol_table(ast->binary_expr.right);
		break;
	case INT_NODE:
		//printf("%d ", ast->int_value);
		break;
	case FLOAT_NODE:
		//printf("%f ", ast->float_value);
		break;
	case BOOL_NODE:
		//if (ast->bool_value == 1)
		//	printf("true ");
		//else
		//	printf("false ");
		break;
	case VAR_NODE:
		//if (ast->variable.is_array == 1) {
		//	printf("(INDEX %s %s %d)\n", type_to_string(ast->type), ast->variable.id, ast->variable.index);
		//} else {
		//	printf("%s ", ast->variable.id);
		//}
		break;
	case FUNCTION_NODE:
		build_symbol_table(ast->function_call.arguments);
		break;
	case CONSTRUCTOR_NODE:
		build_symbol_table(ast->constructor.type);
		build_symbol_table(ast->constructor.arguments);
		break;
	case STATEMENT_NODE:
		build_symbol_table(ast->statements.statements);
		build_symbol_table(ast->statements.statement);
		break;
	case IF_STATEMENT_NODE:
		build_symbol_table(ast->statement_if_else.condition_expression);
		build_symbol_table(ast->statement_if_else.if_statement);
		build_symbol_table(ast->statement_if_else.else_statement);
		break;
	case WHILE_STATEMENT_NODE:
		/* do nothing? */
		break;
	case ASSIGNMENT_NODE:
		build_symbol_table(ast->statement_assign.variable);
		//TODO: name = ast->assignment.left->variable_exp.identifier;
		build_symbol_table(ast->statement_assign.expression);
		break;
	case NESTED_SCOPE_NODE:
		/* do nothing? */
	break;
	case DECLARATIONS_NODE:
		build_symbol_table(ast->declarations.declarations);
		build_symbol_table(ast->declarations.declaration);
		break;
	case DECLARATION_NODE:
		if (true) {
			//printf("%s ", ast->declaration.id);
			build_symbol_table(ast->declaration.type);
			//ast_print(ast->declaration.expression);

			symbol_node* already_declared = search(ast->declaration.id, scope_symbol_table);
			if (already_declared != NULL) {
				fprintf(errorFile, "[ERROR, DECLARATION_NODE] This ID is already declared in this scope!\n");
			} else {
				if (ast->declaration.is_const == 0)
					global_head = add_to_beginning(ast->declaration.type->type_node.type_name,
						ast->declaration.id, 0, scope_symbol_table,
						NONE, 0, global_head);
				else
					global_head = add_to_beginning(ast->declaration.type->type_node.type_name,
						ast->declaration.id, 0, scope_symbol_table,
						CONSTANT, 0, global_head);
			}
		}
		break;
   case TYPE_NODE:
		break;
   case NESTED_EXPRESSION_NODE:
	  	break;
	case EXP_VAR_NODE:
	    break;
	case ARGUMENTS_NODE:
		break;
	default:
		printf("DEFAULT\n");
		break;
	}
}

/*add_to_beginning(int type, char* id, int data, int scope,
		int type_class, int line_number, symbol_node* head)*/
