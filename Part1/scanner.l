%{
/**********************************************************************
 *
 * **YOUR GROUP INFO SHOULD GO HERE**
 *
 *  Interface to the scanner module for CSC467 course project
 **********************************************************************/


#include "common.h"
#include "parser.tab.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

#define YY_USER_INIT { yyin = inputFile; }
#define	yyinput      input
#define yTRACE(x)    { if (traceScanner) fprintf(traceFile, "TOKEN %3d : %s\n", x, yytext); }
#define yERROR(x)    { fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: %s\n", yyline, x); errorOccurred = TRUE; yyterminate(); }

int CheckInt(void);
int yyline = 1;

%}
%option noyywrap

WS          [ \t]+
integer     [\+\-]?(0|([1-9][0-9]*))
float       [\+\-]?([0-9]*[.])?[0-9]+
identifier  [a-zA-Z_][a-zA-Z0-9_]*


%%
"/*"        {
              int c1      = 0;
              int c2      = yyinput();
              int curline = yyline;
              for(;;) {
                if (c2 == EOF) {
                  fprintf(errorFile, "\nLEXICAL ERROR, LINE %d: Unmatched /*\n", curline);
                  errorOccurred = TRUE;
                  yyterminate();
                }
                if (c1 == '*' && c2 == '/')
                  break;
                c1 = c2;
                c2 = yyinput();
                if (c1 == '\n' && c2 != EOF)
                  yyline++;
              }
            }
            
"int"		{
				yTRACE(INT_TYPE);
				return INT_TYPE;
			}
	
"float"		{
				yTRACE(FLOAT_TYPE);
				return FLOAT_TYPE;
			}

"if"		{
				yTRACE(IF);
				return IF;
			}
			
"else"		{
				yTRACE(ELSE);
				return ELSE;
			}
			
"while"		{
				yTRACE(WHILE);
				return WHILE;
			}
			
"bool"      {
				yTRACE(BOOLEAN_TYPE);
				return BOOLEAN_TYPE;
			}

"void"      {
				yTRACE(VOID);
				return VOID;
			}

"vec2"		{
				yTRACE(VEC2);
				return VEC2;
			}

"vec3"		{
				yTRACE(VEC3);
				return VEC3;
			}

"vec4"		{
				yTRACE(VEC4);
				return VEC4;
			}

"bvec2"		{
				yTRACE(BVEC2);
				return BVEC2;
			}

"bvec3"		{
				yTRACE(BVEC3);
				return BVEC3;
			}

"bvec4"		{
				yTRACE(BVEC4);
				return BVEC4;
			}

"ivec2"		{
				yTRACE(IVEC2);
				return IVEC2;
			}

"ivec3"		{
				yTRACE(IVEC3);
				return IVEC3;
			}

"ivec4"		{
				yTRACE(IVEC4);
				return IVEC4;
			}

"const"		{
				yTRACE(CONST);
				return CONST;
			}

("false"|"true") {
				yylval.boolean = (strcmp("true",yytext) == 0) ? true : false;
				yTRACE(BOOLEAN_VALUE);
				return BOOLEAN_VALUE;
			}

\(			{
				yTRACE(O_BRACKET);
				return O_BRACKET;
			}
			
\)			{
				yTRACE(C_BRACKET);
				return C_BRACKET;
			}
			
\{			{
				yTRACE(O_BRACE);
				return O_BRACE;
			}
			
\}			{
				yTRACE(C_BRACE);
				return C_BRACE;
			}
			
\[			{
				yTRACE(O_SQARE_B);
				return O_SQARE_B;
			}
			
\]			{
				yTRACE(C_SQARE_B);
				return C_SQARE_B;
			}
			
\+			{
				yTRACE(PLUS);
				return PLUS;
			}
			
\-			{
				yTRACE(MINUS);
				return MINUS;
			}
			
\*			{
				yTRACE(MUL);
				return MUL;
			}
			
\/			{
				yTRACE(DIV);
				return DIV;
			}
			
\^			{
				yTRACE(POWER);
				return POWER;
			}
			
\=			{
				yTRACE(ASSIGN);
				return ASSIGN;
			}
			
\!			{
				yTRACE(NOT);
				return NOT;
			}			
			
&&			{
				yTRACE(AND);
				return AND;
			}
			
\|\|		{
				yTRACE(OR);
				return OR;
			}
			
\=\=		{
				yTRACE(EQUAL);
				return EQUAL;
			}
			
\!\=		{
				yTRACE(NEQUAL);
				return NEQUAL;
			}
			
\<			{
				yTRACE(LESSTHAN);
				return LESSTHAN;
			}
			
\>			{
				yTRACE(GREATERTHAN);
				return GREATERTHAN;
			}
			
\<\=		{
				yTRACE(LESSEQUAL);
				return LESSEQUAL;
			}
			
\>\=		{
				yTRACE(GREATEREQUAL);
				return GREATEREQUAL;
			}
			
,			{
				yTRACE(COMMA);
				return COMMA;
			}
			
;			{
				yTRACE(SEMICOLON);
				return SEMICOLON;
			}

:			{
				yTRACE(COLON);
				return COLON;
			}
			
{identifier}	{	
				if (yyleng > 32)
				{
					yERROR("The identifier exceeds the maximum length!");
				}
				else
				{
					yylval.str = strndup(yytext, yyleng);
					yTRACE(IDENTIFIER);
					return IDENTIFIER;
				}	
			}
			
{integer}     {

				int temp = atoi(yytext);
				if (temp > 32767 || temp < -32768)
				{
					yERROR("The integer is out of bound!");
				}
				else
				{
					yylval.num_int = temp;
					yTRACE(INT_NUM);
					return INT_NUM;
				}				
			}
			
{float}		{
				yylval.num_float = atof(yytext);
                yTRACE(FLOAT_NUM);
				return FLOAT_NUM;
			}

\n			{
				yyline++;
			}
			

{WS}	    {
			}
			
.           { yERROR("Unknown token"); }
			
%%