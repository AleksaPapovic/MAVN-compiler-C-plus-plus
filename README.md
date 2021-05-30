# MAVN-compiler-C++-

This compiler translates higher level Mips assembly code to regular Mips 32bit assembly.
Higher level Mips assembly introduces concepts of functions and register variables, meaning that you cannot access real registers. For now there are only 12 instructions that compiler supports. In folder tests/mavn_files you can find some examples of input programs. Also there are some compiled files in folder tests/compiled_files.
Compiler has implemented lexical analysis with state machine, syntax analysis with recursive descent algorithm, liveness analysis, interference matrix building and register allocation.
You can change number of real registers for allocation (REG_NUMBER) in Constants.h. Compiler also gives you error messages if there is an error with input file or some algorithm can't be executed.
