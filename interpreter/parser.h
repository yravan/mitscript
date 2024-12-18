
#include "parser_src/nonterminals.h"
#include "AST.h"
#include "PrettyPrinter.h"
#include "parser_src/TreeConverter.cpp"

class Parser{
public:
    void printTokenStream(MITScript & lexer, antlr4::CommonTokenStream & tokens){
    // Print source as text
    std::cout << "source: " << tokens.getText() << "\n";

    // Print the tokens
    std::cout << "tokens: ";

    const auto &vocab = lexer.getVocabulary();
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i != 0) {
        std::cout << " ";
        }

        const auto token = tokens.get(i);
        std::cout << vocab.getSymbolicName(token->getType());
    }
    std::cout << "\n";

    }

    AST::Program * ParseProgram(antlr4::CommonTokenStream & tokens){
        AST::Program * result = nullptr;
        CST::ProgramNode* cst_tree;
        try{
            cst_tree = CST::Program(tokens);
            DEBUG_PRINT("------------------------------------\nCST:    " + cst_tree->to_string() + "\n");
            CST::CSTConverter converter = CST::CSTConverter();
            DEBUG_PRINT("------------------------------------\nConversion:    ");
            AST::Program* program = converter.convert(*cst_tree);

            AST::PrettyPrinter printer;

            #ifdef DEBUG
            std::cout << "AST:        " << std::endl;
            program->accept(printer);
            #endif

            result = program;
        }
        catch(const std::runtime_error& err){
            std::cout << err.what();
        }
        return result;
    }

};
