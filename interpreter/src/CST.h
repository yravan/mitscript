#pragma once
/*
Written by me
*/

#ifdef DEBUG
#define DEBUG_PRINT(x) (std::cout<<x<<std::endl)
#else
#define DEBUG_PRINT(x) do{} while(0)
#endif

#include <string>
#include <sstream>
#include <stdexcept>
#include "antlr4-runtime.h"
#include <iostream>

class ConstantNode; // Converted
class UnitNode;// Converted
class ProductPrimeNode;// Converted
class ProductNode;// Converted
class ArithmeticPrimeNode;// Converted
class ArithmeticNode;// Converted
class PredicateNode;// Converted
class BoolUnitNode;// Converted
class ConjunctionPrimeNode;// Converted
class ConjunctionNode;// Converted
class BooleanPrimeNode;// Converted
class BooleanNode;// Converted
class RecordPrimeNode;// Converted
class RecordNode;// Converted
class LHSPrimeNode;// Converted
class LHSNode;// Converted
class ExpressionNode;// Converted
class ParameterPrimeNode;// Converted
class ParameterNode;// Converted
class CallNode;// Converted
class CallStatementNode;/// Converted
class AssignmentNode;// Converted
class GlobalNode;// Converted
class ProgramNode;// Converted
class ReturnNode;// Converted
class FunctionNode;// Converted
class ArgumentNode;// Converted
class ArgumentPrimetNode;// Converted
class WhileNode;// Converted
class IfStatementNode;// Converted
class ElseNode;// Converted
class BlockNode;// Converted
class BlockPrimeNode;// Converted
class StatementNode;// Converted
class StatementListNode;// Converted

void reportError(antlr4::Token &token);

// Base class for CST Nodes
class CSTNode {
public:
    virtual std::string to_string() = 0;
    // void convert(CSTConverter &c) override{
    //     c.visit(*this);
    // }
};

// ExpressionNode class

class ExpressionNode : public CSTNode {
public:
    ExpressionNode(FunctionNode* fun_, BooleanNode* boolean_, RecordNode* record_);
    std::string to_string() override;


    FunctionNode* fun;
    BooleanNode* boolean;
    RecordNode* record;
};

// Function to report errors
void reportError(antlr4::Token& token);

// ConstantNode class
class ConstantNode : public CSTNode {
public:
    ConstantNode(antlr4::Token* token_);
    std::string to_string() override;


    antlr4::Token* value;
};

// UnitNode class
class UnitNode : public CSTNode {
public:
    UnitNode(bool minus_,LHSNode* lhs_, ConstantNode* constant_, CallNode* call_, BooleanNode* boolean_);
    std::string to_string() override;


    bool minus;
    LHSNode* lhs;
    ConstantNode* constant;
    CallNode* call;
    BooleanNode* boolean;
};

// ProductPrimeNode class
class ProductPrimeNode : public CSTNode {
public:
    ProductPrimeNode(antlr4::Token* op_token_, UnitNode* unit_, ProductPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* op_token;
    UnitNode* unit;
    ProductPrimeNode* rest;
};

// ProductNode class
class ProductNode : public CSTNode {
public:
    ProductNode(UnitNode* unit_, ProductPrimeNode* rest_);
    std::string to_string() override;


    UnitNode* unit;
    ProductPrimeNode* rest;
};

class ArithmeticPrimeNode : public CSTNode {
public:
    ArithmeticPrimeNode(antlr4::Token* op_token_, ProductNode* prod_, ArithmeticPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* op_token;
    ProductNode* prod;
    ArithmeticPrimeNode* rest;
};

// ArithmeticNode class
class ArithmeticNode : public CSTNode {
public:
    ArithmeticNode(ProductNode* prod_, ArithmeticPrimeNode* rest_);
    std::string to_string() override;


    ProductNode* prod;
    ArithmeticPrimeNode* rest;
};

// PredicateNode class
class PredicateNode : public CSTNode {
public:
    PredicateNode(ArithmeticNode* arith_1_, antlr4::Token* cop_token_, ArithmeticNode* arith_2_);
    std::string to_string() override;


    ArithmeticNode* arith_1;
    antlr4::Token* cop_token;
    ArithmeticNode* arith_2;
};

// BoolUnitNode class
class BoolUnitNode : public CSTNode {
public:
    BoolUnitNode(bool negate_, PredicateNode* predicate_);
    std::string to_string() override;


    bool negate;
    PredicateNode* predicate;
};

// ConjunctionPrimeNode class
class ConjunctionPrimeNode : public CSTNode {
public:
    ConjunctionPrimeNode(antlr4::Token* op_token_, BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* op_token;
    BoolUnitNode* bool_unit;
    ConjunctionPrimeNode* rest;
};

// ConjunctionNode class
class ConjunctionNode : public CSTNode {
public:
    ConjunctionNode(BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_);
    std::string to_string() override;


    BoolUnitNode* bool_unit;
    ConjunctionPrimeNode* rest;
};

// BooleanPrimeNode class
class BooleanPrimeNode : public CSTNode {
public:
    BooleanPrimeNode(antlr4::Token* op_token_, ConjunctionNode* conjunction_, BooleanPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* op_token;
    ConjunctionNode* conjunction;
    BooleanPrimeNode* rest;
};

// BooleanNode class
class BooleanNode : public CSTNode {
public:
    BooleanNode(ConjunctionNode* conjunction_, BooleanPrimeNode* rest_);
    std::string to_string() override;


    ConjunctionNode* conjunction;
    BooleanPrimeNode* rest;
};

// RecordPrimeNode class
class RecordPrimeNode : public CSTNode {
public:
    RecordPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, RecordPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* name;
    ExpressionNode* expr;
    RecordPrimeNode* rest;
};

// RecordNode class
class RecordNode : public CSTNode {
public:
    RecordNode(RecordPrimeNode* inside_);
    std::string to_string() override;


    RecordPrimeNode* inside;
};

// LHSPrimeNode class
class LHSPrimeNode : public CSTNode {
public:
    LHSPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, LHSPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* name;
    ExpressionNode* expr;
    LHSPrimeNode* rest;
};

// LHSNode class
class LHSNode : public CSTNode {
public:
    LHSNode(antlr4::Token* name_, LHSPrimeNode* inside_);
    std::string to_string() override;


    antlr4::Token* name;
    LHSPrimeNode* inside;
};



// ArgumentPrimeNode class
class ArgumentPrimeNode : public CSTNode {
public:
    ArgumentPrimeNode(antlr4::Token* name_, ArgumentPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* name;
    ArgumentPrimeNode* rest;
};
// ArgumentNode class
class ArgumentNode : public CSTNode {
public:
    ArgumentNode(antlr4::Token* name_, ArgumentPrimeNode* rest_);
    std::string to_string() override;


    antlr4::Token* name;
    ArgumentPrimeNode* rest;
};
class FunctionNode : public CSTNode {
public:
    FunctionNode(ArgumentNode* args_, BlockNode* block_);
    std::string to_string() override;


    ArgumentNode* args;
    BlockNode* block;
};


class ReturnNode : public CSTNode {
public:
    ReturnNode(ExpressionNode* expr_);
    std::string to_string() override;


    ExpressionNode* expr;
};


class WhileNode : public CSTNode {
public:
    WhileNode(ExpressionNode* expr_, BlockNode* block_);
    std::string to_string() override;


    ExpressionNode* expr;
    BlockNode* block;
};


class ElseNode : public CSTNode {
public:
    ElseNode(BlockNode* block_);
    std::string to_string() override;


    BlockNode* block;
};
class IfStatementNode : public CSTNode {
public:
    IfStatementNode(ExpressionNode* expr_, BlockNode* block_if_, ElseNode* else_block_);
    std::string to_string() override;


    ExpressionNode* expr;
    BlockNode* block_if;
    ElseNode* else_block;
};


class BlockPrimeNode : public CSTNode {
public:
    BlockPrimeNode(StatementNode* statement_, BlockPrimeNode* rest_);
    std::string to_string() override;


    StatementNode* statement;
    BlockPrimeNode* rest;
};

// RecordNode class
class BlockNode : public CSTNode {
public:
    BlockNode(BlockPrimeNode* inside_);
    std::string to_string() override;


    BlockPrimeNode* inside;
};


// ParameterPrimeNode class
class ParameterPrimeNode : public CSTNode {
public:
    ParameterPrimeNode(ExpressionNode* expr_, ParameterPrimeNode* rest_);
    std::string to_string() override;


    ExpressionNode* expr;
    ParameterPrimeNode* rest;
};

// ParameterNode class
class ParameterNode : public CSTNode {
public:
    ParameterNode(ExpressionNode* expr_, ParameterPrimeNode* rest_);
    std::string to_string() override;


    ExpressionNode* expr;
    ParameterPrimeNode* rest;
};

// CallNode class
class CallNode : public CSTNode {
public:
    CallNode(LHSNode* lhs_, ParameterNode* arguments_);
    std::string to_string() override;


    LHSNode* lhs;
    ParameterNode* arguments;
};

// CallStatementNode class
class CallStatementNode : public CSTNode {
public:
    CallStatementNode(CallNode* call_);
    std::string to_string() override;


    CallNode* call;
};

// AssignmentNode class
class AssignmentNode : public CSTNode {
public:
    AssignmentNode(LHSNode* lhs_, ExpressionNode* expr_);
    std::string to_string() override;


    LHSNode* lhs;
    ExpressionNode* expr;
};

// GlobalNode class
class GlobalNode : public CSTNode {
public:
    GlobalNode(antlr4::Token* name_);
    std::string to_string() override;


    antlr4::Token* name;
};




class StatementNode : public CSTNode {
public:
    StatementNode(AssignmentNode* assignment_ = nullptr,
                  CallStatementNode* callStatement_ = nullptr,
                  GlobalNode* global_ = nullptr,
                  IfStatementNode* ifStatement_ = nullptr,
                  WhileNode* whileLoop_ = nullptr,
                  ReturnNode* return_ = nullptr);

    std::string to_string() override;


    AssignmentNode* assignment;
    CallStatementNode* callStatement;
    GlobalNode* global;
    IfStatementNode* ifStatement;
    WhileNode* whileLoop;
    ReturnNode* returnNode;
};



// ProgramNode class
class StatementListNode : public CSTNode {
public:
    StatementListNode(StatementNode* statement_, StatementListNode* rest_);
    std::string to_string() override;


    StatementNode* statement;
    StatementListNode* rest;
};

// ProgramNode class
class ProgramNode : public CSTNode {
public:
    ProgramNode(StatementListNode* child_);
    std::string to_string() override;


    StatementListNode* child;
};


