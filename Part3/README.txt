Phaser 3: AST constrution and semantic checking

Owners: 
Jianning Yan (1000461715)
Wen Ying Zhang (1000404207)

Description:
This phase is about constructiong abstract syntax trees(AST) and making three AST visitors: one that does semantic checking, one that de-allocates an AST, and one that prints an AST. The following will be implemented: AST constrution, AST tear-down, AST printing (-Da) and semantic checking.

Starter Files:
compiler467: revised mian module
Makefile: revised makefile
scanner.l: the workign Flex scanner
parser.y: working Bison parser with full phase 2 functionality
  
We start by running the starter code and make sure the starter code has no error. 

We implement the data structure for ast according to the parser the starter code provides. Then we develop our own symbol table to store variable types. 

We spend most of our time writing semantic check and printing the AST. We use shader.frag and phong.frag to test our printing and semantic check and we also write some simple test cases while we are implementing the functions.

Special Problems Encountered:
1. Understand the problem statement and the requirement of the lab - read the lab handout carefully to make sure we totally understand the problem. We did not know that vector and base type can multiply by each other so we ignore that case at the beginning, due to that, shader.frag was not compiled successfully and we have to change that function at the end. 

2. According to parser input, declare ast table. We cannot print the variable type at first because we did not have a ast simple table. 

3. Implement the correct ast print. Our ast print was very messy at the beginning. It takes us a while to implement it and makes it readable. Now our ast print looks a lot prettier than before and more readable. 

4. How to implement correct semantic check. There are a lot of corner cases that we did not pay attention to at the beginning.

5. Getting the correct types for nested binary and unary expressions took a lot of debugging. It required recursion combined with symbol table usage.

How to run the program:
make clean && make && compiler467 -Da [path-to-the-MiniGLSL-code]

