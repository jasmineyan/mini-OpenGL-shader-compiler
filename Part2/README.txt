Owners:

Jianning Yan (1000461715)
Wen Ying Zhang (1000404207)

Special Problems Encoutered (challenges faced):
1. Understand the problem statement and the requirement of the lab
2. Recognizing the empty string
3. Identifying token types and token values (ex: INT_T and INT_C)
4. Fixing potential bugs in the starter code provided

Overall Approach Taken:
We first read through the lab document carefully. We made sure the corresponding tokens needed are all added properly. We then added all the grammer provided in the lab document according to the syntax taught in our tutorial. After adding the grammer, we added the operator procedence with the tokens in the table provided.

At first we used a simplified version of our code from Lab1 for the scanner and token declarations in parser.y. Then after the Lab2 starter was uploaded, we changed to the Lab2 starter code and made sure everything was working. It took us some effort to notice a missing case for "," in scanner.l from the provided Lab2 starter. There was also a const mismatch we fixed, a similar problem that appeared in the Lab1 starter.

Details of Parser Verification:
We first made sure our parser worked with the demos provided on the course webside. We ran the program with the two frag codes. (shader.frag and phong.frag)

We also came up with our own test cases (in the test_cases folder). The three test cases increase in complexity. First only testing a basic declaration, then testing if-else and vector access. Finally a complete test case with calculations, if-else, function calls and declarations are included in the final test case.

How to run the program with demos provided:
make clean && make compiler467 && compiler467 -Tp [path-to-the-MiniGLSL-code]

How to run with test cases:
make clean && make compiler467 && compiler467 -Tp test_cases/test1_declare_simple_statement.frag

make clean && make compiler467 && compiler467 -Tp test_cases/test2_if_else.frag

make clean && make compiler467 && compiler467 -Tp test_cases/test3_function_call.frag


