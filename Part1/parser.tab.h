/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    myToken1 = 258,
    myToken2 = 259,
    INT_NUM = 260,
    FLOAT_NUM = 261,
    INT_TYPE = 262,
    FLOAT_TYPE = 263,
    O_BRACKET = 264,
    C_BRACKET = 265,
    O_BRACE = 266,
    C_BRACE = 267,
    O_SQARE_B = 268,
    C_SQARE_B = 269,
    PLUS = 270,
    MINUS = 271,
    MUL = 272,
    DIV = 273,
    POWER = 274,
    ASSIGN = 275,
    BOOLEAN_TYPE = 276,
    BOOLEAN_VALUE = 277,
    NOT = 278,
    AND = 279,
    OR = 280,
    EQUAL = 281,
    NEQUAL = 282,
    LESSTHAN = 283,
    GREATERTHAN = 284,
    LESSEQUAL = 285,
    GREATEREQUAL = 286,
    IF = 287,
    ELSE = 288,
    WHILE = 289,
    VOID = 290,
    IDENTIFIER = 291,
    COMMA = 292,
    SEMICOLON = 293,
    COLON = 294,
    CONST = 295,
    VEC2 = 296,
    VEC3 = 297,
    VEC4 = 298,
    BVEC2 = 299,
    BVEC3 = 300,
    BVEC4 = 301,
    IVEC2 = 302,
    IVEC3 = 303,
    IVEC4 = 304
  };
#endif
/* Tokens.  */
#define myToken1 258
#define myToken2 259
#define INT_NUM 260
#define FLOAT_NUM 261
#define INT_TYPE 262
#define FLOAT_TYPE 263
#define O_BRACKET 264
#define C_BRACKET 265
#define O_BRACE 266
#define C_BRACE 267
#define O_SQARE_B 268
#define C_SQARE_B 269
#define PLUS 270
#define MINUS 271
#define MUL 272
#define DIV 273
#define POWER 274
#define ASSIGN 275
#define BOOLEAN_TYPE 276
#define BOOLEAN_VALUE 277
#define NOT 278
#define AND 279
#define OR 280
#define EQUAL 281
#define NEQUAL 282
#define LESSTHAN 283
#define GREATERTHAN 284
#define LESSEQUAL 285
#define GREATEREQUAL 286
#define IF 287
#define ELSE 288
#define WHILE 289
#define VOID 290
#define IDENTIFIER 291
#define COMMA 292
#define SEMICOLON 293
#define COLON 294
#define CONST 295
#define VEC2 296
#define VEC3 297
#define VEC4 298
#define BVEC2 299
#define BVEC3 300
#define BVEC4 301
#define IVEC2 302
#define IVEC3 303
#define IVEC4 304

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 57 "parser.y" /* yacc.c:1909  */

  int num_int;
  float num_float;
  char* str;
  bool boolean;
  

#line 160 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
