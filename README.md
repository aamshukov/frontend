# fe
**Front End (FE)** parsing framework in **modern C++**.

Implementation of **parsing classical** algorithms with some exposure to **IR**.

Covers:
- **Grammar** representation
- **FA** (NFA/DFA) related algorithms and construction
- **RE** related algorithms and construction
- Fully functional **lexical analyzer** with Unicode support
- Calculation of **FIRST(k) and FOLLOW(k), EFF(k), LA(k) sets**
- **Recursive descent** parser
- **LL(k)** table construction for **strong-LL(k)** and **full-LL(k)** methods
- **LR(k)** table construction 
- **Earley parser** implementation to handle all ambiguous tree(s), supports **AST** construction
- **Backtracking** techniques
- **CST** (Concret Syntax Tree) construction
- **AST** (Abstract Syntax Tree) ability to construct and convert from **CST**
- **ASD** (Abstract Syntax Dag) conversion from **CST**/**AST**
- **IR** related algorithms, **TAC**, **QUADRUPLES** and **SSA** (comming soon ...)
- **Compiler/Interpretator** controller implementation

and much more ... stay tuned.
