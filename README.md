# fe
Front end (parsing) framework.

Implementation of **parsing classical FE (Front End)** algorithms with some exposure to IR.
Covers:
- FA (NFA/DFA)
- RE
- Fully functional lexical analyzer with Unicode support
- calculation of FIRST(k) and FOLLOW(k), EFF(k)
- LL(k) table construction for strong-LL(k) and full-LL(k) methods
- LR(k) table construction 
- Earley parser implementation to handle all ambiguous tree(s)
- backtracking techniques
- CST (Concret Syntax Tree) construction
- AST (Abstract Syntax Tree) ability to construct and convert from CST
- ASD (Abstract Syntax Dag) conversion from CST/AST

and much more ...
