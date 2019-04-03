# fe
**Front End** parsing framework in modern C++.

Implementation of **parsing classical FE (Front End)** algorithms with some exposure to IR.

The framework has implementation of various algorithms, methods and techniques, including:
- FA (NFA/DFA) related algorithms and construction
- RE related algorithms and construction
- Fully functional lexical analyzer with Unicode support
- calculation of FIRST(k) and FOLLOW(k), EFF(k)
- Recursive descent parser
- LL(k) table construction for strong-LL(k) and full-LL(k) methods
- LR(k) table construction 
- Earley parser implementation to handle all ambiguous tree(s)
- backtracking techniques
- CST (Concret Syntax Tree) construction
- AST (Abstract Syntax Tree) ability to construct and convert from CST
- ASD (Abstract Syntax Dag) conversion from CST/AST
- IR related algorithms, TAC and SSA (comming soon ...)
- Compiler/Interpretator controller implementation

and much more ... stay tuned.
