#ifndef CST_NODES_H
#define CST_NODES_H


#define DEBUG
#define PRINT_DEBUG

#include <string>
#include <sstream>
#include <stdexcept>
#include "antlr4-runtime.h"
#include <iostream>

class ConstantNode;
class UnitNode;
class ProductPrimeNode;
class ProductNode;
class ArithmeticPrimeNode;
class ArithmeticNode;
class PredicateNode;
class BoolUnitNode;
class ConjunctionPrimeNode;
class ConjunctionNode;
class BooleanPrimeNode;
class BooleanNode;
class RecordPrimeNode;
class RecordNode;
class LHSPrimeNode;
class LHSNode;
class ExpressionNode;
class ParameterPrimeNode;
class ParameterNode;
class CallNode;
class CallStatementNode;
class AssignmentNode;
class GlobalNode;
class ProgramNode;
class ReturnNode;
class FunctionNode;
class ArgumentNode;
class ArgumenPrimetNode;
class WhileNode;
class IfStatementNode;
class ElseNode;
class BlockNode;
class BlockPrimeNode;
class StatementNode;

void reportError(antlr4::Token &token);

// Base class for CST Nodes
class CSTNode {
public:
    virtual std::string to_string() = 0;
};

// ExpressionNode class

class ExpressionNode : public CSTNode {
public:
    ExpressionNode(FunctionNode* fun_, BooleanNode* boolean_, RecordNode* record_);
    std::string to_string() override;

private:
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

private:
    antlr4::Token* value;
};

// UnitNode class
class UnitNode : public CSTNode {
public:
    UnitNode(bool minus_,LHSNode* lhs_, ConstantNode* constant_, CallNode* call_, BooleanNode* boolean_);
    std::string to_string() override;

private:
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

private:
    antlr4::Token* op_token;
    UnitNode* unit;
    ProductPrimeNode* rest;
};

// ProductNode class
class ProductNode : public CSTNode {
public:
    ProductNode(UnitNode* unit_, ProductPrimeNode* rest_);
    std::string to_string() override;

private:
    UnitNode* unit;
    ProductPrimeNode* rest;
};

class ArithmeticPrimeNode : public CSTNode {
public:
    ArithmeticPrimeNode(antlr4::Token* op_token_, ProductNode* prod_, ArithmeticPrimeNode* rest_);
    std::string to_string() override;

private:
    antlr4::Token* op_token;
    ProductNode* prod;
    ArithmeticPrimeNode* rest;
};

// ArithmeticNode class
class ArithmeticNode : public CSTNode {
public:
    ArithmeticNode(ProductNode* prod_, ArithmeticPrimeNode* rest_);
    std::string to_string() override;

private:
    ProductNode* prod;
    ArithmeticPrimeNode* rest;
};

// PredicateNode class
class PredicateNode : public CSTNode {
public:
    PredicateNode(ArithmeticNode* arith_1_, antlr4::Token* cop_token_, ArithmeticNode* arith_2_);
    std::string to_string() override;

private:
    ArithmeticNode* arith_1;
    antlr4::Token* cop_token;
    ArithmeticNode* arith_2;
};

// BoolUnitNode class
class BoolUnitNode : public CSTNode {
public:
    BoolUnitNode(bool negate_, PredicateNode* predicate_);
    std::string to_string() override;

private:
    bool negate;
    PredicateNode* predicate;
};

// ConjunctionPrimeNode class
class ConjunctionPrimeNode : public CSTNode {
public:
    ConjunctionPrimeNode(antlr4::Token* op_token_, BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_);
    std::string to_string() override;

private:
    antlr4::Token* op_token;
    BoolUnitNode* bool_unit;
    ConjunctionPrimeNode* rest;
};

// ConjunctionNode class
class ConjunctionNode : public CSTNode {
public:
    ConjunctionNode(BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_);
    std::string to_string() override;

private:
    BoolUnitNode* bool_unit;
    ConjunctionPrimeNode* rest;
};

// BooleanPrimeNode class
class BooleanPrimeNode : public CSTNode {
public:
    BooleanPrimeNode(antlr4::Token* op_token_, ConjunctionNode* conjunction_, BooleanPrimeNode* rest_);
    std::string to_string() override;

private:
    antlr4::Token* op_token;
    ConjunctionNode* conjunction;
    BooleanPrimeNode* rest;
};

// BooleanNode class
class BooleanNode : public CSTNode {
public:
    BooleanNode(ConjunctionNode* conjunction_, BooleanPrimeNode* rest_);
    std::string to_string() override;

private:
    ConjunctionNode* conjunction;
    BooleanPrimeNode* rest;
};

// RecordPrimeNode class
class RecordPrimeNode : public CSTNode {
public:
    RecordPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, RecordPrimeNode* rest_);
    std::string to_string() override;

private:
    antlr4::Token* name;
    ExpressionNode* expr;
    RecordPrimeNode* rest;
};

// RecordNode class
class RecordNode : public CSTNode {
public:
    RecordNode(RecordPrimeNode* inside_);
    std::string to_string() override;

private:
    RecordPrimeNode* inside;
};

// LHSPrimeNode class
class LHSPrimeNode : public CSTNode {
public:
    LHSPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, LHSPrimeNode* rest_);
    std::string to_string() override;

private:
    antlr4::Token* name;
    ExpressionNode* expr;
    LHSPrimeNode* rest;
};

// LHSNode class
class LHSNode : public CSTNode {
public:
    LHSNode(antlr4::Token* name_, LHSPrimeNode* inside_);
    std::string to_string() override;

private:
    antlr4::Token* name;
    LHSPrimeNode* inside;
};



// ArgumentPrimeNode class
class ArgumentPrimeNode : public CSTNode {
public:
    ArgumentPrimeNode(antlr4::Token* name_, ArgumentPrimeNode* rest_);
    std::string to_string() override;

private:
    antlr4::Token* name;
    ArgumentPrimeNode* rest;
};
// ArgumentNode class
class ArgumentNode : public CSTNode {
public:
    ArgumentNode(antlr4::Token* name_, ArgumentPrimeNode* rest_);
    std::string to_string() override;

private:
    antlr4::Token* name;
    ArgumentPrimeNode* rest;
};
class FunctionNode : public CSTNode {
public:
    FunctionNode(ArgumentNode* args_, BlockNode* block_);
    std::string to_string() override;

private:
    ArgumentNode* args;
    BlockNode* block;
};


class ReturnNode : public CSTNode {
public:
    ReturnNode(ExpressionNode* expr_);
    std::string to_string() override;

private:
    ExpressionNode* expr;
};


class WhileNode : public CSTNode {
public:
    WhileNode(ExpressionNode* expr_, BlockNode* block_);
    std::string to_string() override;

private:
    ExpressionNode* expr;
    BlockNode* block;
};


class ElseNode : public CSTNode {
public:
    ElseNode(BlockNode* block_);
    std::string to_string() override;

private:
    BlockNode* block;
};
class IfStatementNode : public CSTNode {
public:
    IfStatementNode(ExpressionNode* expr_, BlockNode* block_if_, ElseNode* else_block_);
    std::string to_string() override;

private:
    ExpressionNode* expr;
    BlockNode* block_if;
    ElseNode* else_block;
};


class BlockPrimeNode : public CSTNode {
public:
    BlockPrimeNode(StatementNode* statement_, BlockPrimeNode* rest_);
    std::string to_string() override;

private:
    StatementNode* statement;
    BlockPrimeNode* rest;
};

// RecordNode class
class BlockNode : public CSTNode {
public:
    BlockNode(BlockPrimeNode* inside_);
    std::string to_string() override;

private:
    BlockPrimeNode* inside;
};


// ParameterPrimeNode class
class ParameterPrimeNode : public CSTNode {
public:
    ParameterPrimeNode(ExpressionNode* expr_, ParameterPrimeNode* rest_);
    std::string to_string() override;

private:
    ExpressionNode* expr;
    ParameterPrimeNode* rest;
};

// ParameterNode class
class ParameterNode : public CSTNode {
public:
    ParameterNode(ExpressionNode* expr_, ParameterPrimeNode* rest_);
    std::string to_string() override;

private:
    ExpressionNode* expr;
    ParameterPrimeNode* rest;
};

// CallNode class
class CallNode : public CSTNode {
public:
    CallNode(LHSNode* lhs_, ParameterNode* arguments_);
    std::string to_string() override;

private:
    LHSNode* lhs;
    ParameterNode* arguments;
};

// CallStatementNode class
class CallStatementNode : public CSTNode {
public:
    CallStatementNode(CallNode* call_);
    std::string to_string() override;

private:
    CallNode* call;
};

// AssignmentNode class
class AssignmentNode : public CSTNode {
public:
    AssignmentNode(LHSNode* lhs_, ExpressionNode* expr_);
    std::string to_string() override;

private:
    LHSNode* lhs;
    ExpressionNode* expr;
};

// GlobalNode class
class GlobalNode : public CSTNode {
public:
    GlobalNode(antlr4::Token* name_);
    std::string to_string() override;

private:
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

private:
    AssignmentNode* assignment;
    CallStatementNode* callStatement;
    GlobalNode* global;
    IfStatementNode* ifStatement;
    WhileNode* whileLoop;
    ReturnNode* returnNode;
};



// ProgramNode class
class ProgramNode : public CSTNode {
public:
    ProgramNode(BlockPrimeNode* child_);
    std::string to_string() override;

private:
    BlockPrimeNode* child;
};

#endif // CST_NODES_H

