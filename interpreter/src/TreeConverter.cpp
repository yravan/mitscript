#pragma once
/*
Written by me
*/
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "AST.h"
#include "CST.h"
#include "../MITScript.h"

class CSTConverter {
public:
    // Convert a ProgramNode to a Program structure
    AST::Program* convert(const ProgramNode& programNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ProgramNode)" << std::endl;
#endif
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ProgramNode)" << std::endl;
#endif
        return new AST::Program(convert(*programNode.child));
    }

    AST::Block* convert(const StatementListNode& statementListNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(statementListNode)" << std::endl;
#endif
        std::vector<AST::Statement*> statements;
        statements.push_back(convert(*statementListNode.statement));
        if (statementListNode.rest != nullptr){
            std::vector<AST::Statement*> remainder = convert(*statementListNode.rest)->statements;
            statements.insert(statements.end(), remainder.begin(), remainder.end());
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(statementListNode)" << std::endl;
#endif
        return new AST::Block(statements);
    }

    AST::Statement* convert(const StatementNode& statementNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(StatementNode)" << std::endl;
#endif
        if (statementNode.assignment != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(StatementNode)" << std::endl;
#endif
            return convert(*statementNode.assignment);
        }
        if (statementNode.callStatement != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(StatementNode)" << std::endl;
#endif
            return convert(*statementNode.callStatement);
        }
        if (statementNode.global != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(StatementNode)" << std::endl;
#endif
            return convert(*statementNode.global);
        }
        if (statementNode.ifStatement != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(StatementNode)" << std::endl;
#endif
            return convert(*statementNode.ifStatement);
        }
        if (statementNode.whileLoop != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(StatementNode)" << std::endl;
#endif
            return convert(*statementNode.whileLoop);
        }
        if (statementNode.returnNode != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(StatementNode)" << std::endl;
#endif
            return convert(*statementNode.returnNode);
        }
    }

    // Convert a BlockNode to a Block structure
    AST::Block* convert(const BlockNode& blockNode) {
        // Implementation to convert BlockNode to Block
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(BlockNode)" << std::endl;
#endif
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(BlockNode)" << std::endl;
#endif
        if (blockNode.inside != nullptr){
            return new AST::Block(convert(*blockNode.inside));
        }
        return new AST::Block(std::vector<AST::Statement*>());
    }
    // Convert a BlockNode to a Block structure
    std::vector<AST::Statement*> convert(const BlockPrimeNode& blockPrimeNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(BlockPrimeNode)" << std::endl;
#endif
        // Implementation to convert BlockNode to Block
        std::vector<AST::Statement*> statements;
        statements.push_back(convert(*blockPrimeNode.statement));
        if (blockPrimeNode.rest != nullptr){
            std::vector<AST::Statement*> remainder = convert(*blockPrimeNode.rest);
            statements.insert(statements.end(), remainder.begin(), remainder.end());
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(BlockPrimeNode)" << std::endl;
#endif
        return statements;
    }

    // Convert a GlobalNode to a Global structure
    AST::Global* convert(const GlobalNode& globalNode) {
        // Implementation to convert GlobalNode to Global
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(GlobalNode)" << std::endl;
#endif
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(GlobalNode)" << std::endl;
#endif
        return new AST::Global(globalNode.name->getText());
    }

    AST::Expression* convert(const UnitNode& unitNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(UnitNode)" << std::endl;
#endif
        AST::Expression* expr;
        if (unitNode.lhs != nullptr){
            expr = convert(*unitNode.lhs);
        }
        if (unitNode.constant != nullptr){
            expr = convert(*unitNode.constant);
        }
        if (unitNode.call != nullptr){
            expr = convert(*unitNode.call);
        }
        if (unitNode.boolean != nullptr){
            expr = convert(*unitNode.boolean);
        }
        if (unitNode.minus){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(UnitNode)" << std::endl;
#endif
            return new AST::UnaryExpression(expr, "-", true);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(UnitNode)" << std::endl;
#endif
        return expr;
    }


    // Convert an AssignmentNode to an Assignment structure
    AST::Assignment* convert(const AssignmentNode& assignmentNode) {
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Entering: convert(AssignmentNode)" << std::endl;
#endif
        // Implementation to convert AssignmentNode to Assignment
        AST::Expression* lhs = convert(*assignmentNode.lhs);
        AST::Expression* expr = convert(*assignmentNode.expr);
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(AssignmentNode)" << std::endl;
#endif
        return new AST::Assignment(lhs, expr);
    }

    // Convert a ReturnNode to a Return structure
    AST::Return* convert(const ReturnNode& returnNode) {
        // Implementation to convert ReturnNode to Return
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Entering: convert(ReturnNode)" << std::endl;
            std::cout << "Exiting: convert(ReturnNode)" << std::endl;
#endif
        return new AST::Return(convert(*returnNode.expr));

    }

    // Convert a FunctionNode to a FunctionDeclaration structure
    AST::FunctionDeclaration* convert(const FunctionNode& functionNode) {
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Entering: convert(FunctionNode)" << std::endl;
#endif
        // Implementation to convert FunctionNode to FunctionDeclaration
        if (functionNode.args == nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(FunctionNode)" << std::endl;
#endif
            return new AST::FunctionDeclaration(std::vector<std::string>(), convert(*functionNode.block));
        }
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(FunctionNode)" << std::endl;
#endif
        return new AST::FunctionDeclaration(convert(*functionNode.args), convert(*functionNode.block));

    }

    // Convert a CallNode to a Call structure
    AST::Call* convert(const CallNode& callNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(CallNode)" << std::endl;
#endif
        // Implementation to convert CallNode to Call
        AST::Expression * targ_expr = convert(*callNode.lhs);
        std::vector<AST::Expression*> params;
        if (callNode.arguments != nullptr) {
            params = convert(*callNode.arguments);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(CallNode)" << std::endl;
#endif
        return new AST::Call(targ_expr, params);
    }

    // Convert an ExpressionNode to an Expression structure
    AST::Expression* convert(const ExpressionNode& expressionNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ExpressionNode)" << std::endl;
#endif
        // Implementation to convert ExpressionNode to Expression
        if (expressionNode.fun != nullptr){
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ExpressionNode)" << std::endl;
#endif
            return convert(*expressionNode.fun);
        }
        if (expressionNode.boolean != nullptr){
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ExpressionNode)" << std::endl;
#endif
            return convert(*expressionNode.boolean);
        }
        if (expressionNode.record != nullptr){
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ExpressionNode)" << std::endl;
#endif
            return convert(*expressionNode.record);
        }
    }

    AST::Expression* convert(const ConstantNode& constantNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ConstantNode)" << std::endl;
#endif
        switch (constantNode.value->getType()) {
            case MITScript::INT: {
                int intValue = std::stoi(constantNode.value->getText()); // Assuming there's a method to get int value
#ifdef PRETTY_PRINT_DEBUG
                std::cout << "Exiting: convert(ConstantNode)" << std::endl;
#endif
                return new AST::IntegerConstant(intValue);
            }
            case MITScript::STRING: {
                std::string stringValue = constantNode.value->getText(); // Assuming there's a method to get string value
#ifdef PRETTY_PRINT_DEBUG
                std::cout << "Exiting: convert(ConstantNode)" << std::endl;
#endif
                return new AST::StringConstant(stringValue);
            }
            case MITScript::NONE: {
#ifdef PRETTY_PRINT_DEBUG
                std::cout << "Exiting: convert(ConstantNode)" << std::endl;
#endif
                return new AST::NoneConstant(); // Assuming NoneConstant has a default constructor
            }
            case MITScript::TRUE: {
#ifdef PRETTY_PRINT_DEBUG
                std::cout << "Exiting: convert(ConstantNode)" << std::endl;
#endif
                return new AST::BooleanConstant(true);
            }
            case MITScript::FALSE: {
#ifdef PRETTY_PRINT_DEBUG
                std::cout << "Exiting: convert(ConstantNode)" << std::endl;
#endif
                return new AST::BooleanConstant(false);
            }
            default:
                // Handle unknown type or throw an exception
                throw std::runtime_error("Unknown constant type");
        }
    }
    // Convert a PredicateNode to a Predicate structure
    AST::Expression* convert(const PredicateNode& predicateNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(PredicateNode)" << std::endl;
#endif
        AST::Expression* left_expr = convert(*predicateNode.arith_1);
        if (predicateNode.arith_2 != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(PredicateNode)" << std::endl;
#endif
            return new AST::BinaryExpression(left_expr, predicateNode.cop_token->getText(),convert(*predicateNode.arith_2) );
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(PredicateNode)" << std::endl;
#endif
        return left_expr;

    }

    // Convert a BoolUnitNode to a BoolUnit structure
    AST::Expression* convert(const BoolUnitNode& boolUnitNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(BoolUnitNode)" << std::endl;
#endif
        // Implementation to convert BoolUnitNode
        AST::Expression* pred = convert(*boolUnitNode.predicate);
        if (boolUnitNode.negate){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(BoolUnitNode)" << std::endl;
#endif
            return new AST::UnaryExpression(pred, "!", true);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(BoolUnitNode)" << std::endl;
#endif
        return pred;
    }

    // Convert a ProductPrimeNode to a ProductPrime structure
    AST::Expression* convert(const ProductPrimeNode& productPrimeNode, AST::Expression* left_op) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ProductPrimeNode)" << std::endl;
#endif
        // Implementation to convert ProductPrimeNode
        AST::Expression * right_op = convert(*productPrimeNode.unit);
        AST::Expression * expr = new AST::BinaryExpression(left_op, productPrimeNode.op_token->getText(), right_op);
        if (productPrimeNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(ProductPrimeNode)" << std::endl;
#endif
            return convert(*productPrimeNode.rest, expr);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ProductPrimeNode)" << std::endl;
#endif
        return expr;
    }

    // Convert a ProductNode to a Product structure
    AST::Expression* convert(const ProductNode& productNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ProductNode)" << std::endl;
#endif
        // Implementation to convert ProductNode
        AST::Expression* first_op = convert(*productNode.unit);
        if (productNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Entering: convert(ProductNode)" << std::endl;
#endif
            return convert(*productNode.rest, first_op);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ProductNode)" << std::endl;
#endif
        return first_op;
    }

    // Convert an ArithmeticPrimeNode to an ArithmeticPrime structure
    AST::Expression* convert(const ArithmeticPrimeNode& arithmeticPrimeNode, AST::Expression* left_op) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ArithmeticPrimeNode)" << std::endl;
#endif
        // Implementation to convert ArithmeticPrimeNode
        AST::Expression * right_op = convert(*arithmeticPrimeNode.prod);
        AST::Expression * expr = new AST::BinaryExpression(left_op, arithmeticPrimeNode.op_token->getText(), right_op);
        if (arithmeticPrimeNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(ArithmeticPrimeNode)" << std::endl;
#endif
            return convert(*arithmeticPrimeNode.rest, expr);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ArithmeticPrimeNode)" << std::endl;
#endif
        return expr;
    }

    // Convert an ArithmeticNode to an Arithmetic structure
    AST::Expression* convert(const ArithmeticNode& arithmeticNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ArithmeticNode)" << std::endl;
#endif
        // Implementation to convert ArithmeticNode
        AST::Expression* first_op = convert(*arithmeticNode.prod);
        if (arithmeticNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(ArithmeticNode)" << std::endl;
#endif
            return convert(*arithmeticNode.rest, first_op);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ArithmeticNode)" << std::endl;
#endif
        return first_op;
    }

    // Convert a ConjunctionPrimeNode to a ConjunctionPrime structure
    AST::Expression* convert(const ConjunctionPrimeNode& conjunctionPrimeNode, AST::Expression* left_op) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ConjunctionPrimeNode)" << std::endl;
#endif
        // Implementation to convert ConjunctionPrimeNode
        AST::Expression * right_op = convert(*conjunctionPrimeNode.bool_unit);
        AST::Expression * expr = new AST::BinaryExpression(left_op, conjunctionPrimeNode.op_token->getText(), right_op);
        if (conjunctionPrimeNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(ConjunctionPrimeNode)" << std::endl;
#endif
            return convert(*conjunctionPrimeNode.rest, expr);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ConjunctionPrimeNode)" << std::endl;
#endif
        return expr;
    }

    // Convert a ConjunctionNode to a Conjunction structure
    AST::Expression* convert(const ConjunctionNode& conjunctionNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ConjunctionNode)" << std::endl;
#endif
        // Implementation to convert ConjunctionNode
        AST::Expression* first_op = convert(*conjunctionNode.bool_unit);
        if (conjunctionNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(ConjunctionNode)" << std::endl;
#endif
            return convert(*conjunctionNode.rest, first_op);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ConjunctionNode)" << std::endl;
#endif
        return first_op;
    }

    // Convert a BooleanPrimeNode to a BooleanPrime structure
    AST::Expression* convert(const BooleanPrimeNode& booleanPrimeNode, AST::Expression* left_op) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(BooleanPrimeNode)" << std::endl;
#endif
        // Implementation to convert BooleanPrimeNode
        AST::Expression* right_op = convert(*booleanPrimeNode.conjunction);
        AST::Expression* expr = new AST::BinaryExpression(left_op, booleanPrimeNode.op_token->getText(), right_op);
        if (booleanPrimeNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(BooleanPrimeNode)" << std::endl;
#endif
            return convert(*booleanPrimeNode.rest, expr);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(BooleanPrimeNode)" << std::endl;
#endif
        return expr;
    }

    // Convert a BooleanNode to a Boolean structure
    AST::Expression* convert(const BooleanNode& booleanNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(BooleanNode)" << std::endl;
#endif
        // Implementation to convert BooleanNode
        AST::Expression* first_op = convert(*booleanNode.conjunction);
        if (booleanNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
            std::cout << "Exiting: convert(BooleanNode)" << std::endl;
#endif
            return convert(*booleanNode.rest, first_op);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(BooleanNode)" << std::endl;
#endif
        return first_op;
    }

    // Convert a RecordPrimeNode to a RecordPrime structure
    std::map<std::string, AST::Expression*> convert(const RecordPrimeNode& recordPrimeNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(RecordPrimeNode)" << std::endl;
#endif
        // Implementation to convert RecordPrimeNode
        std::map<std::string,AST::Expression*> map;
        map[recordPrimeNode.name->getText()] = convert(*recordPrimeNode.expr);
        if (recordPrimeNode.rest != nullptr){
            std::map<std::string, AST::Expression*>rest = convert(*recordPrimeNode.rest);
            map.insert(rest.begin(), rest.end());
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(RecordPrimeNode)" << std::endl;
#endif
        return map;
    }

    // Convert a RecordNode to a Record structure
    AST::Record* convert(const RecordNode& recordNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(RecordNode)" << std::endl;
#endif
        // Implementation to convert RecordNode
        std::map<std::string, AST::Expression*> map;
        if (recordNode.inside != nullptr){
            map = convert(*recordNode.inside); 
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(RecordNode)" << std::endl;
#endif
        return new AST::Record(map);
    }

    // Convert a LHSPrimeNode to a LHSPrime structure
    AST::Expression* convert(const LHSPrimeNode& lhsPrimeNode,  AST::Expression* left_expr) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(LHSPrimeNode)" << std::endl;
#endif
        // Implementation to convert LHSPrimeNode
        if (lhsPrimeNode.name != nullptr){
            left_expr = new AST::FieldDereference(left_expr, lhsPrimeNode.name->getText());
        }
        if (lhsPrimeNode.expr != nullptr){
            left_expr = new AST::IndexExpression(left_expr, convert(*lhsPrimeNode.expr));
        }
        if (lhsPrimeNode.rest != nullptr){
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(LHSPrimeNode)" << std::endl;
#endif
            return convert(*lhsPrimeNode.rest, left_expr);
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(LHSPrimeNode)" << std::endl;
#endif
        return left_expr;
    }

    // Convert a LHSNode to a LHS structure
    AST::Expression* convert(const LHSNode& lhsNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(LHSNode)" << std::endl;
#endif
        if (lhsNode.inside != nullptr){
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(LHSNode)" << std::endl;
#endif
            return convert(*lhsNode.inside, new AST::Expression(lhsNode.name->getText()));
        }else{
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(LHSNode)" << std::endl;
#endif
            return new AST::Expression(lhsNode.name->getText());
        }

    }

    // Convert a ParameterPrimeNode to a ParameterPrime structure
    std::vector<AST::Expression*>  convert(const ParameterPrimeNode& parameterPrimeNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ParameterPrimeNode)" << std::endl;
#endif
        // Implementation to convert ParameterPrimeNode
        std::vector<AST::Expression*> parameters; // Create a new AST::vector to hold the converted expressions
        parameters.push_back(convert(*parameterPrimeNode.expr));
        if (parameterPrimeNode.rest != nullptr){
            std::vector<AST::Expression*> remainder = convert(*parameterPrimeNode.rest);
            parameters.insert(parameters.end(), remainder.begin(), remainder.end());
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ParameterPrimeNode)" << std::endl;
#endif
        return parameters;
    }

    // Convert a ParameterNode to a Parameter structure
    std::vector<AST::Expression*> convert(const ParameterNode& parameterNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ParameterNode)" << std::endl;
#endif
        // Implementation to convert ParameterNode
        std::vector<AST::Expression*> parameters; // Create a new AST::vector to hold the converted expressions
        if (parameterNode.expr != nullptr){
            parameters.push_back(convert(*parameterNode.expr));
            if (parameterNode.rest != nullptr){
                std::vector<AST::Expression*> remainder = convert(*parameterNode.rest);
                parameters.insert(parameters.end(), remainder.begin(), remainder.end());
            }
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ParameterNode)" << std::endl;
#endif
        return parameters;
    }

    // Convert an ArgumentNode to an Argument structure
    std::vector<std::string> convert(const ArgumentNode& argumentNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ArgumentNode)" << std::endl;
#endif
        // Implementation to convert ArgumentNode
        std::vector<std::string> arguments; // Create a new AST::vector to hold the converted expressions
        if (argumentNode.name != nullptr){
            arguments.push_back(argumentNode.name->getText());
            if (argumentNode.rest != nullptr){
                std::vector<std::string> remainder = convert(*argumentNode.rest);
                arguments.insert(arguments.end(), remainder.begin(), remainder.end());
            }
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ArgumentNode)" << std::endl;
#endif
        return arguments;
    }

    // Convert an ArgumenPrimeNode to an ArgumenPrime structure
    std::vector<std::string> convert(const ArgumentPrimeNode& argumentPrimeNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering: convert(ArgumentPrimeNode)" << std::endl;
#endif
        // Implementation to convert ArgumenPrimeNode
        std::vector<std::string> arguments; // Create a new AST::vector to hold the converted expressions
        arguments.push_back(argumentPrimeNode.name->getText());
        if (argumentPrimeNode.rest != nullptr){
            std::vector<std::string> remainder = convert(*argumentPrimeNode.rest);
            arguments.insert(arguments.end(), remainder.begin(), remainder.end());
        }
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Exiting: convert(ArgumentPrimeNode)" << std::endl;
#endif
        return arguments;
    }

    // Convert a CallStatementNode to a CallStatement structure
    AST::CallStatement* convert(const CallStatementNode& callStatementNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering/Exiting: convert(CallStatementNode)" << std::endl;
#endif
        // Implementation to convert CallStatementNode
        return new AST::CallStatement(convert(*callStatementNode.call));
    }

    // Convert a WhileNode to a WhileLoop structure
    AST::WhileLoop* convert(const WhileNode& whileNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering/Exiting: convert(WhileNode)" << std::endl;
#endif
        // Implementation to convert WhileNode
        return new AST::WhileLoop(convert(*whileNode.expr), convert(*whileNode.block));
    }

    // Convert an IfStatementNode to an IfStatement structure
    AST::IfStatement* convert(const IfStatementNode& ifStatementNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering/Exiting: convert(IfStatementNode)" << std::endl;
#endif
        // Implementation to convert IfStatementNode
        AST::Block* else_block = nullptr;
        if (ifStatementNode.else_block != nullptr){
            else_block = convert(*ifStatementNode.else_block);
        }
        return new AST::IfStatement(convert(*ifStatementNode.expr), convert(*ifStatementNode.block_if), else_block);
    }

    // Convert an ElseNode to an Else structure
    AST::Block* convert(const ElseNode& elseNode) {
#ifdef PRETTY_PRINT_DEBUG
        std::cout << "Entering/Exiting: convert(ElseNode)" << std::endl;
#endif

        return convert(*elseNode.block);
    }
};
