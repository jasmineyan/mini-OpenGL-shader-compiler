#include "ast.h"
#include "common.h"
#include "parser.tab.h"
#include "semantic.h"

int scope = 0;
int args_num = 0;

int check_args_num(node *ast, int count) {

	if(ast == NULL)
		return count;

	switch(ast->kind){
		case ARGUMENTS_NODE:
			count = check_args_num(ast->arguments.arg, count);
			if (ast->arguments.expr) {
				count ++;
			}
	}
	return count;
}

// ast must be type arguments
int check_args(node *ast, bool is_func, int func_name, int input_scope) {
	int arg_result1 = -1;
	int arg_result2 = -1;

	if(ast == NULL || (!is_func && (func_name == UNKOWN_TYPE || func_name == IDENTIFIER)))
		return -1;

	switch(ast->kind) {
		case ARGUMENTS_NODE:
			args_num = check_args_num(ast->arguments.arg, 0);
			arg_result1 = check_args(ast->arguments.arg, is_func, func_name, input_scope);
			arg_result2 = check_args(ast->arguments.expr, is_func, func_name, input_scope);

			if (arg_result2 != 1) {
				return arg_result2;
			}

			return arg_result1;
			return arg_result2;
			break;
		case EXP_VAR_NODE:
			arg_result1 = check_args(ast->expr_variable.variable, is_func, func_name, input_scope);
			return arg_result1;
		    break;
		case VAR_NODE:
			if (true) {
				node_complete_type result;
				result.is_index_vector = false;
				result.type = UNKOWN_TYPE;
				result.type_class = NONE;
				symbol_node* var_symbol_node = valid_scope_node(ast->variable.id, input_scope);
				if (var_symbol_node) {
					result.type = var_symbol_node->type;
					result.is_index_vector = ast->variable.is_array;
					result.type_class = var_symbol_node->type_class;
				}
				if (is_func) {
					//LIT
					if (func_name == 1 && args_num == 1) {
						if (result.type != VEC4 || result.is_index_vector) {
							fprintf(errorFile, "[ERROR, FUNCTION_NODE, LIT] Args cannot have non VEC4 types!\n");
							return -4;
						} else {
							return 1;
						}
					}
					//DP3
					else if (func_name == 0 && args_num == 2) {
						if ((result.type != VEC4 && result.type != VEC3 &&
								result.type != IVEC4 && result.type != IVEC3) || result.is_index_vector) {
							fprintf(errorFile, "[ERROR, FUNCTION_NODE, DP3] Args contains invalid types!\n");
							return -4;
						} else {
							return 1;
						}
					}
					//RSQ
					else if (func_name == 2 && args_num == 1) {
						if ((result.type != FLOAT && result.type != INT) &&
								!(result.is_index_vector && (result.type == VEC2 || result.type == VEC3 ||
										result.type == VEC4 || result.type == IVEC2 || result.type == IVEC3 || result.type == IVEC4))) {
							fprintf(errorFile, "[ERROR, FUNCTION_NODE, RSQ] Args contains invalid types!\n");
							return -4;
						} else {
							return 1;
						}
					} else {
						//fprintf(errorFile, "[ERROR, FUNCTION_NODE] Unknown function name!\n");
						return -3;
					}
				} else { //constructor
					if ((func_name == INT || func_name == BOOL || func_name == FLOAT) && args_num == 1) {
						if (func_name != result.type) {
							fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, INT/BOOL/FLOAT] Args contains invalid types!\n");
							return -5;
						} else {
							return 1;
						}
					} else if (func_name == VEC2 || func_name == VEC3 || func_name == VEC4) {
						if (result.type != FLOAT) {
							fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC2/VEC3/VEC4] Args contains non FLOAT types!\n");
							return -5;
						} else {
							return 1;
						}
					} else if (func_name == IVEC2 || func_name == IVEC3 || func_name == IVEC4 ) {
						if (result.type != INT) {
							fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, IVEC2/IVEC3/IVEC4] Args contains non INT types!\n");
							return -5;
						} else {
							return 1;
						}
					} else if (func_name == BVEC2 || func_name == BVEC3 || func_name == BVEC4) {
						if (result.type != BOOL) {
							fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, BVEC2/BVEC3/BVEC4] Args contains non BOOL types!\n");
							return -5;
						} else {
							return 1;
						}
					} else {
						//fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE] Unknown constructor type!\n");
						return -3;
					}
				}
				return 1;
			}
			break;
		case BOOL_NODE:
			if (is_func) {
				//LIT
				if (func_name == 1 && args_num == 1) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, LIT] Args cannot have non VEC4 types!\n");
					return -4;
				}
				//DP3
				else if (func_name == 0 && args_num == 2) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, DP3] Args contains invalid types!\n");
					return -4;
				}
				//RSQ
				else if (func_name == 2 && args_num == 1) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, RSQ] Args contains invalid types!\n");
					return -4;
				} else {
					//fprintf(errorFile, "[ERROR, FUNCTION_NODE] Unknown function name!\n");
					return -3;
				}
			} else { //constructor
				if (func_name == BOOL || func_name == BVEC2 || func_name == BVEC3 || func_name == BVEC4) {
					return 1;
				} else if (func_name == INT || func_name == FLOAT) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, INT/BOOL/FLOAT] Args contains invalid types!\n");
					return -5;
				} else if (func_name == VEC2 || func_name == VEC3 || func_name == VEC4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC2/VEC3/VEC4] Args contains non FLOAT types!\n");
					return -5;
				} else if (func_name == IVEC2 || func_name == IVEC3 || func_name == IVEC4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, IVEC2/IVEC3/IVEC4] Args contains non INT types!\n");
					return -5;
				} else {
					//fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE] Unknown constructor type!\n");
					return -3;
				}
			}
			break;
		case INT_NODE:
			if (is_func) {
				//LIT
				if (func_name == 1 && args_num == 1) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, LIT] Args cannot have non VEC4 types!\n");
					return -4;
				}
				//DP3
				else if (func_name == 0 && args_num == 2) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, DP3] Args contains invalid types!\n");
					return -4;
				}
				//RSQ
				else if (func_name == 2 && args_num == 1) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, RSQ] Args contains invalid types!\n");
					return -4;
				} else {
					//fprintf(errorFile, "[ERROR, FUNCTION_NODE] Unknown function name!\n");
					return -3;
				}
			} else { //constructor
				if (func_name == INT || func_name == IVEC2 || func_name == IVEC3 || func_name == IVEC4) {
					return 1;
				} else if (func_name == BOOL || func_name == FLOAT) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, INT/BOOL/FLOAT] Args contains invalid types!\n");
					return -5;
				} else if (func_name == VEC2 || func_name == VEC3 || func_name == VEC4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC2/VEC3/VEC4] Args contains non FLOAT types!\n");
					return -5;
				} else if (func_name == BVEC2 || func_name == BVEC3 || func_name == BVEC4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, BVEC2/BVEC3/BVEC4] Args contains non BOOL types!\n");
					return -5;
				} else {
					//fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE] Unknown constructor type!\n");
					return -3;
				}
			}
			break;
		case FLOAT_NODE:
			if (is_func) {
				//LIT
				if (func_name == 1) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, LIT] Args cannot have non VEC4 types!\n");
					return -4;
				}
				//DP3
				else if (func_name == 0) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, DP3] Args contains invalid types!\n");
					return -4;
				}
				//RSQ
				else if (func_name == 2) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, RSQ] Args contains invalid types!\n");
					return -4;
				} else {
					//fprintf(errorFile, "[ERROR, FUNCTION_NODE] Unknown function name!\n");
					return -3;
				}
			} else { //constructor
				if (func_name == FLOAT || func_name == VEC2 || func_name == VEC3 || func_name == VEC4) {
					return 1;
				} else if (func_name == BOOL || func_name == INT) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, INT/BOOL/FLOAT] Args contains invalid types!\n");
					return -5;
				} else if (func_name == IVEC2 || func_name == IVEC3 || func_name == IVEC4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, IVEC2/IVEC3/IVEC4] Args contains non INT types!\n");
					return -5;
				} else if (func_name == BVEC2 || func_name == BVEC3 || func_name == BVEC4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, BVEC2/BVEC3/BVEC4] Args contains non BOOL types!\n");
					return -5;
				} else {
					//fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE] Unknown constructor type!\n");
					return -3;
				}
			}
			break;
		default:
			if (is_func) {
				fprintf(errorFile, "[ERROR, FUNCTION_NODE] invalid function call!\n");
			} else {
				fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE] invalid constructor call!\n");
			}
			return -1;
			break;
	}
}

int semantic_check(node *ast) {

	if(ast == NULL)
		return -2;

	int s_result1 = 0;
	int s_result2 = 0;
	node_complete_type type1 = {0};
	node_complete_type type2 = {0};

	switch(ast->kind) {
	case SCOPE_NODE:
		scope++;
		s_result1 = semantic_check(ast->scope.declarations);
		s_result2 = semantic_check(ast->scope.statements);
		scope--;

		if (s_result1 == -1 || s_result2 == -1)
			return -1;
		else
			return s_result1;

		break;
	case STATEMENT_NODE:
		s_result1 = semantic_check(ast->statements.statements);
		s_result2 = semantic_check(ast->statements.statement);

		if (s_result1 == -1 || s_result2 == -1)
			return -1;
		else
			return s_result1;

		break;
	case BINARY_EXPRESSION_NODE:
		s_result1 = semantic_check(ast->binary_expr.left);
		s_result2 = semantic_check(ast->binary_expr.right);

		if (s_result1 == -1 || s_result2 == -1)
			return -1;

		get_type(ast->binary_expr.left, &type1, scope);
		get_type(ast->binary_expr.right, &type2, scope);

		if (ast->binary_expr.op == '*') {
			if (type1.type == INT) {
				if (type2.type == INT) {
					return INT;
				} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
					if (type2.is_index_vector) {
						return INT;
					}
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == IVEC2 || type1.type == IVEC3 || type1.type == IVEC4) {
				if (type1.is_index_vector) {
					if (type2.type == INT) {
						return INT;
					} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
						if (type1.is_index_vector) {
							return INT;
						}
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
						return -1;
					}
				} else {
					if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
						if (type1.type == type2.type){
							return type1.type;
						} else {
							fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
							return -1;
						}
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
						return -1;
					}
				}
			} else if (type1.type == FLOAT) {
				if (type2.type == FLOAT) {
					return FLOAT;
				} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
					if (type2.is_index_vector) {
						return FLOAT;
					}
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == VEC2 || type1.type == VEC3 || type1.type == VEC4) {
				if (type1.is_index_vector) {
					if (type2.type == FLOAT) {
						return FLOAT;
					} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
						if (type1.is_index_vector) {
							return FLOAT;
						}
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
						return -1;
					}
				} else {
					if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
						if (type1.type == type2.type){
							return type1.type;
						} else {
							fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
							return -1;
						}
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *]  Base type does not match!\n");
						return -1;
					}
				}
			} else {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, *] Cannot have non arithmetic types!\n");
				return -1;
			}
		} else if (ast->binary_expr.op == '+' || ast->binary_expr.op == '-') {
			if (type1.type == INT) {
				if (type2.type == INT) {
					return INT;
				} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Cannot operate IVEC with int!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == IVEC2 || type1.type == IVEC3 || type1.type == IVEC4) {
				if (type2.type == INT) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Cannot operate IVEC with INT!\n");
					return -1;
				} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
					if (type1.is_index_vector == type2.is_index_vector && type1.type == type2.type) {
						return type1.type;
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Cannot operate unindexed IVEC with indexed IVEC!\n");
						return -1;
					}
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == FLOAT) {
				if (type2.type == FLOAT) {
					return FLOAT;
				} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Cannot operate VEC with FLOAT!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == VEC2 || type1.type == VEC3 || type1.type == VEC4) {
				if (type2.type == FLOAT) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Cannot operate VEC with FLOAT!\n");
					return -1;
				} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
					if (type1.is_index_vector == type2.is_index_vector && type1.type == type2.type) {
						return type1.type;
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Cannot operate unindexed VEC with indexed VEC!\n");
						return -1;
					}
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-]  Base type does not match!\n");
					return -1;
				}
			} else {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, +-] Cannot have non arithmetic types!\n");
				return -1;
			}
		} else if (ast->binary_expr.op == '/' || ast->binary_expr.op == '^') {
			if (type1.type == INT) {
				if (type2.type == INT) {
					return INT;
				} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, /^]  Cannot operate vectors!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, /^]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == FLOAT) {
				if (type2.type == FLOAT) {
					return FLOAT;
				} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, /^]  Cannot operate vectors!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, /^]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == IVEC2 || type1.type == IVEC3 || type1.type == IVEC4 ||
					type1.type == VEC2 || type1.type == VEC3 || type1.type == VEC4) {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, /^]  Cannot operate vectors!\n");
				return -1;
			}
			else {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, /^] Cannot have non arithmetic types!\n");
				return -1;
			}
		} else if (ast->binary_expr.op == AND || ast->binary_expr.op == OR) {
			if (type1.type == BOOL) {
				if (type2.type == BOOL) {
					return BOOL;
				} else if (type2.type == BVEC2 || type2.type == BVEC3 || type2.type == BVEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, &&||]  Cannot operate BVEC with BOOL!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, &&||]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == BVEC2 || type1.type == BVEC3 || type1.type == BVEC4) {
				if (type2.type == FLOAT) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, &&||]  Cannot operate BVEC with BOOL!\n");
					return -1;
				} else if (type2.type == BVEC2 || type2.type == BVEC3 || type2.type == BVEC4) {
					if (type1.is_index_vector == type2.is_index_vector && type1.type == type2.type) {
						return type1.type;
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, &&||]  Cannot operate unindexed BVEC with indexed BVEC!\n");
						return -1;
					}
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, &&||]  Base type does not match!\n");
					return -1;
				}
			} else {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, &&||] Cannot have non logic types!\n");
				return -1;
			}
		} else if (ast->binary_expr.op == '<' || ast->binary_expr.op == LEQ || ast->binary_expr.op == '>' || ast->binary_expr.op == GEQ) {
			if (type1.type == INT) {
				if (type2.type == INT) {
					return INT;
				} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, compare_size]  Cannot operate vectors!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, compare_size]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == FLOAT) {
				if (type2.type == FLOAT) {
					return FLOAT;
				} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, compare_size]  Cannot operate vectors!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, compare_size]  Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == IVEC2 || type1.type == IVEC3 || type1.type == IVEC4 ||
					type1.type == VEC2 || type1.type == VEC3 || type1.type == VEC4) {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, compare_size]  Cannot operate vectors!\n");
				return -1;
			}
			else {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, compare_size] Cannot have non arithmetic types!\n");
				return -1;
			}
		} else if (ast->binary_expr.op == EQ || ast->binary_expr.op == NEQ) {
			if (type1.type == INT) {
				if (type2.type == INT) {
					return INT;
				} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Cannot operate IVEC with INT!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == IVEC2 || type1.type == IVEC3 || type1.type == IVEC4) {
				if (type2.type == INT) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Cannot operate IVEC with INT!\n");
					return -1;
				} else if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
					if (type1.is_index_vector == type2.is_index_vector && type1.type == type2.type) {
						return type1.type;
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Base type does not match!\n");
						return -1;
					}
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == FLOAT) {
				if (type2.type == FLOAT) {
					return FLOAT;
				} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Cannot operate VEC with FLOAT!\n");
					return -1;
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Base type does not match!\n");
					return -1;
				}
			} else if (type1.type == VEC2 || type1.type == VEC3 || type1.type == VEC4) {
				if (type2.type == FLOAT) {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Cannot operate VEC with FLOAT!\n");
					return -1;
				} else if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
					if (type1.is_index_vector == type2.is_index_vector && type1.type == type2.type) {
						return type1.type;
					} else {
						fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Base type does not match!\n");
						return -1;
					}
				} else {
					fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Base type does not match!\n");
					return -1;
				}
			} else {
				fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE, EQNEQ] Cannot have non arithmetic types!\n");
				return -1;
			}
		} else {
			fprintf(errorFile, "[ERROR, BINARY_EXPRESSION_NODE] Cannot recognize expression operation!\n");
			return -1;
		}
		break;
	case UNARY_EXPRESION_NODE:
		get_type(ast->binary_expr.right, &type1, scope);

		if (ast->binary_expr.op == '-') {
			if (type1.type == INT ||
					type1.type == IVEC2 || type1.type == IVEC3 || type1.type == IVEC4 ||
					type1.type == FLOAT ||
					type1.type == VEC2 || type1.type == VEC3 || type1.type == VEC4){
				return type1.type;
			} else {
				fprintf(errorFile, "[ERROR, UNARY_EXPRESSION_NODE, -] Cannot have non arithmetic types!\n");
				return -1;
			}
		} else if (ast->binary_expr.op == '!') {
			if (type1.type == BOOL ||
					type1.type == BVEC2 || type1.type == BVEC3 || type1.type == BVEC4){
				return type1.type;
			} else {
				fprintf(errorFile, "[ERROR, UNARY_EXPRESSION_NODE, !] Cannot have non logic types!\n");
				return -1;
			}
		} else {
			fprintf(errorFile, "[ERROR, UNARY_EXPRESSION_NODE] Cannot recognize expression operation!\n");
			return -1;
		}

		s_result1 = semantic_check(ast->unary_expr.right);
		if (s_result1 == -1)
			return -1;

		break;
	case ASSIGNMENT_NODE:
		s_result1 = semantic_check(ast->statement_assign.variable);
		s_result2 = semantic_check(ast->statement_assign.expression);

		if (s_result1 == -1 || s_result2 == -1)
			return -1;

		get_type(ast->statement_assign.variable, &type1, scope);
		get_type(ast->statement_assign.expression, &type2, scope);

		if (type1.type_class == CONSTANT) {
			fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign constant variable!\n");
			return -1;
		} else if (type1.type_class == ATTRIBUTE || type1.type_class == UNIFORM) {
			fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign read only variable!\n");
			return -1;
		}

		if (type1.type == type2.type &&
				type1.is_index_vector == type2.is_index_vector) {
			return type1.type;
		}

		if (type1.type == INT) {
			if (type2.type == IVEC2 || type2.type == IVEC3 || type2.type == IVEC4) {
				if (type2.is_index_vector) {
					return INT;
				} else {
					fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign unindexed IVEC to INT!\n");
					return -1;
				}
			} else {
				fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Base type does not match!\n");
				return -1;
			}
		} else if (type1.type == BOOL) {
			if (type2.type == BVEC2 || type2.type == BVEC3 || type2.type == BVEC4) {
				if (type2.is_index_vector) {
					return BOOL;
				} else {
					fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign unindexed BVEC to BOOL!\n");
					return -1;
				}
			} else {
				fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Base type does not match!\n");
				return -1;
			}
		} else if (type1.type == FLOAT) {
			if (type2.type == VEC2 || type2.type == VEC3 || type2.type == VEC4) {
				if (type2.is_index_vector) {
					return FLOAT;
				} else {
					fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign unindexed VEC to FLOAT!\n");
					return -1;
				}
			} else {
				fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Base type does not match!\n");
				return -1;
			}
		} else if (type1.type == VEC2 || type1.type == VEC3 || type1.type == VEC4) {
			if (type1.is_index_vector) {
				if (type2.type == FLOAT) {
					return FLOAT;
				}
			} else if (!type1.is_index_vector) {
				if (type2.type == FLOAT) {
					fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign FLOAT to unindexed VEC!\n");
					return -1;
				} else if (type1.type == type2.type) {
					return type1.type;
				}
			}
			fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Base type does not match!\n");
			return -1;
		} else if (type1.type == BVEC2 || type1.type == BVEC3 || type1.type == BVEC4) {
			if (type1.is_index_vector) {
				if (type2.type == BOOL) {
					return BOOL;
				}
			} else if (!type1.is_index_vector) {
				if (type2.type == BOOL) {
					fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign BOOL to unindexed BVEC!\n");
					return -1;
				}
			}
			fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Base type does not match!\n");
			return -1;
		} else if (type1.type == IVEC2 || type1.type == IVEC3 || type1.type == IVEC4) {
			if (type1.is_index_vector) {
				if (type2.type == INT) {
					return INT;
				}
			} else if (!type1.is_index_vector) {
				if (type2.type == INT) {
					fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Cannot assign INT to unindexed IVEC!\n");
					return -1;
				}
			}
			fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Base type does not match!\n");
			return -1;
		} else {
			fprintf(errorFile, "[ERROR, ASSIGNMENT_NODE] Invalid type!\n");
			return -1;
		}
		break;
	case IF_STATEMENT_NODE:
		s_result1 = semantic_check(ast->statement_if_else.condition_expression);
		get_type(ast->statement_if_else.condition_expression, &type1, scope);

		if (s_result1 == -1) {
			return -1;
		}
		if(type1.type != BOOL) {
			fprintf(errorFile, "[ERROR, IF_STATEMENT_NODE] Condition expression is not type BOOL!\n");
		}

		s_result1 = semantic_check(ast->statement_if_else.if_statement);
		s_result2 = semantic_check(ast->statement_if_else.else_statement);

		if (s_result1 == -1 || s_result2 == -1)
			return -1;
		else
			return s_result1;
		break;
	case CONSTRUCTOR_NODE:
		if (true) {
			s_result1 = semantic_check(ast->constructor.type);
			s_result2 = semantic_check(ast->constructor.arguments);

			if (s_result1 == -1 || s_result1 == -2)
				return -1;

			int arg_count = check_args_num(ast->constructor.arguments, 0);

			if (ast->constructor.type->type_node.type_name == INT || ast->constructor.type->type_node.type_name == BOOL || ast->constructor.type->type_node.type_name == FLOAT) {
				if (arg_count >= 2) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, INT/BOOL/FLOAT] Cannot have more than one arg %d!\n",arg_count);
					return -2;
				} else if (arg_count < 1) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, INT/BOOL/FLOAT] Cannot have less than one arg!\n");
					return -2;
				}
			} else if (ast->constructor.type->type_node.type_name == VEC2 || ast->constructor.type->type_node.type_name == BVEC2 || ast->constructor.type->type_node.type_name == IVEC2) {
				if (arg_count >= 3) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC2/BVEC2/IVEC2] Cannot have more than two arg!\n");
					return -2;
				} else if (arg_count < 2) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC2/BVEC2/IVEC2] Cannot have less than two arg!\n");
					return -2;
				}
			} else if (ast->constructor.type->type_node.type_name == VEC3 || ast->constructor.type->type_node.type_name == BVEC3 || ast->constructor.type->type_node.type_name == IVEC3) {
				if (arg_count >= 4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC3/BVEC3/IVEC3] Cannot have more than three arg!\n");
					return -2;
				} else if (arg_count < 3) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC3/BVEC3/IVEC3] Cannot have less than three arg!\n");
					return -2;
				}
			} else if (ast->constructor.type->type_node.type_name == VEC4 || ast->constructor.type->type_node.type_name == BVEC4 || ast->constructor.type->type_node.type_name == IVEC4) {
				if (arg_count >= 5) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC4/BVEC4/IVEC4] Cannot have more than four arg!\n");
					return -2;
				} else if (arg_count < 4) {
					fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE, VEC4/BVEC4/IVEC4] Cannot have less than four arg!\n");
					return -2;
				}
			} else {
				fprintf(errorFile, "[ERROR, CONSTRUCTOR_NODE] Unknown constructor type!\n");
				return -3;
			}

			if (check_args(ast->constructor.arguments,
					false,
					ast->constructor.type->type_node.type_name, scope) != 1) {
				return -1;
			}

			return s_result1;
		}
		break;
	case FUNCTION_NODE:
		if (true) {
			s_result1 = semantic_check(ast->function_call.arguments);
			if (s_result1 == -1)
					return -1;

			int arg_count = check_args_num(ast->constructor.arguments, 0);
			//LIT
			if (ast->function_call.name == 1) {
				if (arg_count >= 2) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, LIT] Cannot have more than one arg!\n");
					return -2;
				} else if (arg_count < 1) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, LIT] Cannot have less than one arg!\n");
					return -2;
				}
			}
			//DP3
			else if (ast->function_call.name  == 0) {
				if (arg_count >= 3) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, DP3] Cannot have more than two args!\n");
					return -2;
				} else if (arg_count < 2) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, DP3] Cannot have less than two arg!\n");
					return -2;
				}
			}
			//RSQ
			else if (ast->function_call.name  == 2) {
				if (arg_count >= 2) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, RSQ] Cannot have more than one arg!\n");
					return -2;
				} else if (arg_count < 1) {
					fprintf(errorFile, "[ERROR, FUNCTION_NODE, RSQ] Cannot have less than one arg!\n");
					return -2;
				}
			} else {
				fprintf(errorFile, "[ERROR, FUNCTION_NODE] Unknown function name!\n");
				return -3;
			}

			if (check_args(ast->function_call.arguments,
					true,
					ast->function_call.name, scope) != 1) {
				return -1;
			}

			return s_result1;
		}
		break;
	case DECLARATIONS_NODE:
		s_result1 = semantic_check(ast->declarations.declaration);
		s_result2 = semantic_check(ast->declarations.declarations);

		if (s_result1 == -1 || s_result2 == -1)
				return -1;
		else
			return s_result1;
		break;
	case DECLARATION_NODE:
		if (true) {
			s_result1 = semantic_check(ast->declaration.type);
			s_result2 = semantic_check(ast->declaration.expression);
			if (s_result1 == -1 || s_result2 == -1)
					return -1;
			else
				return s_result1;
		}
		break;
	case INT_NODE:
		return INT;
		break;
	case FLOAT_NODE:
		return FLOAT;
		break;
	case BOOL_NODE:
		return BOOL;
		break;
	case VAR_NODE:
		if (true) {
			int variable_type = UNKOWN_TYPE;
			//printf ("id: %s, scope: %d\n",ast->variable.id, scope);
			symbol_node* var_symbol_node = valid_scope_node(ast->variable.id, scope);
			if (var_symbol_node != NULL) {
				variable_type = var_symbol_node->type;
			} else {
				fprintf(errorFile, "[ERROR, VAR_NODE] Variable not defined!\n");
				return -1;
			}

			if (variable_type == UNKOWN_TYPE) {
				fprintf(errorFile, "[ERROR, VAR_NODE] Variable type unknown!\n");
				return -1;
			}

			if (ast->variable.is_array) {
				if (variable_type == VEC2 || variable_type == IVEC2 || variable_type == BVEC2) {
					if (ast->variable.index >= 2) {
						fprintf(errorFile, "[ERROR, VAR_NODE] Index out of scope!\n");
						return -1;
					}
				} else if (variable_type == VEC3 || variable_type == IVEC3 || variable_type == BVEC3) {
					if (ast->variable.index >= 3) {
						fprintf(errorFile, "[ERROR, VAR_NODE] Index out of scope!\n");
						return -1;
					}
				} else if (variable_type == VEC4 || variable_type == IVEC4 || variable_type == BVEC4) {
					if (ast->variable.index >= 4) {
						fprintf(errorFile, "[ERROR, VAR_NODE] Index out of scope!\n");
						return -1;
					}
				}
			}
			return variable_type;
		}
		break;
	case TYPE_NODE:
		if (ast->type_node.type_name == UNKOWN_TYPE) {
			fprintf(errorFile, "[ERROR, TYPE_NODE] type unknown!\n");
			return -1;
		}
		return ast->type_node.type_name;
		break;
    case NESTED_EXPRESSION_NODE:
    	s_result1 = semantic_check(ast->nested_expr.expr);
	  	if (s_result1 == -1)
	  		return -1;
		else
			return s_result1;
	  	break;
	case EXP_VAR_NODE:
		s_result1 = semantic_check(ast->expr_variable.variable);
		if (s_result1 == -1)
			return -1;
		else
			return s_result1;
	    break;
	case ARGUMENTS_NODE:
		s_result1 = semantic_check(ast->arguments.arg);
		s_result2 = semantic_check(ast->arguments.expr);

		if (s_result1 == -1 || s_result2 == -1)
			return -1;
		else
			return s_result1;
		break;
	default:
		break;
	}
}
