Phase 1: Scanner

Owners:
Jianning Yan (1000461715)
Wen Ying Zhang (1000404207)

Requirement:
Lexical analyzer should store appropriate information for each token identified into a tracing file, and any error recognized into an error file. 

Trace output:
When the -Tn (trace scanner) switch is activated in the compiler, the global variable traceScaner is set to "TRUE"(1). Trace information shuld be sent to the globaly visible FILE* variable traceFile. At this stage, if the input is valid no other action has to be taken other than that specified above. The trace should output the tokens in the same order that they appear in the program. 

Error Handling:
If the scanner encouters an illegal input it should report an error. Use the yERROR(x) macro to report any error. Check for corner cases such as out of bounds integers, and identifiers that exceed the allowed length. 

Starter Files:
compiler467.c - The main module for the project
Makefile - a makefile for the project
scanner.l - the skeleton flex scanner
parser.y - the skeleton bison parser
compiler467.man - the man page for the compiler
globalvars.c - the global variables
common.h - the global definitions

Special Problems Encouter:
1. Understand the problem statement and the requirement of the lab
2. Understand how to compile and run the starter
3. Syntax of the lex file (missing a space between token and brace)
4. Getting the right regular expression for integer, float and identifier
5. Including all neccessary tokens (we missed a few at the beginning so the complier returned errors)
6. Order of the token (we put the identifier before keywords, so all the keywords were not recognized)

How to run the program:
make clean && make compiler467 && compiler467 -Tn [path-to-the-MiniGLSL-code]
