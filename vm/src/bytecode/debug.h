#pragma once
// #define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) (std::cout<<x<<std::endl)
#define DEBUG_PRINT_AST(node, printer) (node.accept(printer))
#else
#define DEBUG_PRINT(x) do{} while(0)
#define DEBUG_PRINT_AST(node, printer) do{} while(0)
#endif


