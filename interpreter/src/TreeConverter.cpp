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
        return new AST::Program(convert(*programNode.child));
    }

    AST::Block* convert(const StatementListNode& statementListNode) {
        std::vector<Statement*> statements;
        statements.push_back(statementListNode.statement);
        if (statementListNode.rest != nullptr){
            std::vector<Statement*> remainder = convert(*statementListNode.rest)->statements;
            statements.insert(statements.end(), remainder.begin(), remainder.end());
        }
        return new AST::Block(statements);
    }

    AST::Statement* convert(const StatementNode& statementNode) {
        if (statementNode.assignment != nullptr){
            return convert(*statementNode.assignment);
        }
        if (statementNode.callStatement != nullptr){
            return convert(*statementNode.callStatement);
        }
        if (statementNode.global != nullptr){
            return convert(*statementNode.global);
        }
        if (statementNode.ifStatement != nullptr){
            return convert(*statementNode.ifStatement);
        }
        if (statementNode.whileLoop != nullptr){
            return convert(*statementNode.whileLoop);
        }
        if (statementNode.returnNode != nullptr){
            return convert(*statementNode.returnNode);
        }
    }

    // Convert a BlockNode to a Block structure
    AST::Block* convert(const BlockNode& blockNode) {
        // Implementation to convert BlockNode to Block
        return new AST::Block(convert(*blockNode.inside));
    }
    // Convert a BlockNode to a Block structure
    std::vector<Statement*> convert(const BlockPrimeNode& blockPrimeNode) {
        // Implementation to convert BlockNode to Block
        std::vector<Statement*> statements;
        statements.push_back(blockPrimeNode.statement);
        if (blockPrimeNode.rest != nullptr){
            std::vector<Statement*> remainder = convert(*blockPrimeNode.rest);
            statements.insert(statements.end(), remainder.begin(), remainder.end());
        }
        return statements;
    }

    // Convert a GlobalNode to a Global structure
    AST::Global* convert(const GlobalNode& globalNode) {
        // Implementation to convert GlobalNode to Global
        return new AST::Global(globalNode.name->getText());
    }


    // Convert a FunctioNode to a FunctionDeclaration structure
    AST::FunctionDeclaration* convert(const FunctioNode& functionNode) {
        // Implementation to convert FunctioNode to Global
        return new AST::Global(globalNode.name->getText());
    }

    AST::Expression* convert(const UnitNode& unitNode) {
        Expression* expr;
        if (unitNode.lhs != nullptr){
            expr = convert(lhs);
        }
        if (unitNode.constant != nullptr){
            expr = convert(constant);
        }
        if (unitNode.call != nullptr){
            expr = convert(call);
        }
        if (unitNode.boolean != nullptr){
            expr = convert(boolean);
        }
        if (boolUnitNode.negate){
            return new AST::UnaryExpression(expr, "-", true);
        }
        else return expr
    }


    // Convert an AssignmentNode to an Assignment structure
    AST::Assignment* convert(const AssignmentNode& assignmentNode) {
        // Implementation to convert AssignmentNode to Assignment
        Expression* lhs = convert(*assignmentNode.lhs);
        Expression* expr = convert(*assignmentNode.expr);
        return new AST::Assignment(lhs, expr);
    }

    // Convert a CallStatementNode to a CallStatement structure
    AST::CallStatement* convert(const CallStatementNode& callStatementNode) {
        // Implementation to convert CallStatementNode to CallStatement
    }

    // Convert a ReturnNode to a Return structure
    AST::Return* convert(const ReturnNode& returnNode) {
        // Implementation to convert ReturnNode to Return
        return new AST::Return(convert(*returnNode.expr));

    }

    // Convert a FunctionNode to a FunctionDeclaration structure
    AST::FunctionDeclaration* convert(const FunctionNode& functionNode) {
        // Implementation to convert FunctionNode to FunctionDeclaration
        if (functionNode.args == nullptr){
            return new AST::FunctionDeclaration(std::vector<std::string>(), convert(*functionNode.block));
        }
        return new AST::FunctionDeclaration(convert(*functionNode.args), convert(*functionNode.block))

    }

    // Convert a CallNode to a Call structure
    AST::Call* convert(const CallNode& callNode) {
        // Implementation to convert CallNode to Call
        Expression * targ_expr = convert(*callNode.lhs);
        std::vector<Expression*> params = convert(*callNode.arguments);
        return Call(targ_expr, params);
    }

    // Convert an ExpressionNode to an Expression structure
    AST::Expression* convert(const ExpressionNode& expressionNode) {
        // Implementation to convert ExpressionNode to Expression
        if (expressionNode.fun != nullptr){
            return convert(*expressionNode.fun)
        }
        if (expressionNode.boolean != nullptr){
            return convert(*expressionNode.boolean)
        }
        if (expressionNode.record != nullptr){
            return convert(*expressionNode.record)
        }
    }

    AST::Expression* convert(const ConstantNode& constantNode) override {
        switch (constantNode.value->getType()) {
            case MITScript::INT: {
                int intValue = std::stoi(constantNode.value->getText()); // Assuming there's a method to get int value
                return new AST::IntegerConstant(intValue);
            }
            case MITScript::STRING: {
                std::string stringValue = constantNode.value->getText(); // Assuming there's a method to get string value
                return new AST::StringConstant(stringValue);
            }
            case MITScript::NONE: {
                return new AST::NoneConstant(); // Assuming NoneConstant has a default constructor
            }
            case MITScript::BOOL: {
                bool boolValue = (constantNode.value->getText() == "true"); // Assuming there's a method to get bool value
                return new AST::BooleanConstant(boolValue);
            }
            default:
                // Handle unknown type or throw an exception
                throw std::runtime_error("Unknown constant type");
        }
    }
    // Convert a PredicateNode to a Predicate structure
    AST::Expression* convert(const PredicateNode& predicateNode) {
        Expression* left_expr = convert(*predicateNode.arith_1);
        if (predicateNode.arith_2 != nullptr){
            return new AST::BinaryExpression(left_expr, predicateNode.cop_token->getText(),convert(*predicateNode.arith_2) );
        }
        return left_expr;

    }

    // Convert a BoolUnitNode to a BoolUnit structure
    AST::Expression* convert(const BoolUnitNode& boolUnitNode) {
        // Implementation to convert BoolUnitNode
        Expression* pred = convert(*boolUnitNode.predicate);
        if (boolUnitNode.negate){
            return new AST::UnaryExpression(pred, "!", true);
        }
        else return pred
    }

    // Convert a ProductPrimeNode to a ProductPrime structure
    AST::Expression* convert(const ProductPrimeNode& productPrimeNode, Expression* left_op) {
        // Implementation to convert ProductPrimeNode
        Expression * right_op = convert(*productPrimeNode.unit);
        Expression * expr = BinaryExpression(left_op, productPrimeNode.op_token->getText(), right_op);
        if (productPrimeNode.rest != nullptr){
            return convert(*productPrimeNode.rest, expr);
        }
        return expr;
    }

    // Convert a ProductNode to a Product structure
    AST::Expression* convert(const ProductNode& productNode) {
        // Implementation to convert ProductNode
        Expression* first_op = convert(*productNode.unit);
        if (productNode.rest != nullptr){
            return convert(*productNode.rest, first_op);
        }
        return first_op;
    }

    // Convert an ArithmeticPrimeNode to an ArithmeticPrime structure
    AST::Expression* convert(const ArithmeticPrimeNode& arithmeticPrimeNode, Expression* left_op) {
        // Implementation to convert ArithmeticPrimeNode
        Expression * right_op = convert(*arithmeticPrimeNode.prod);
        Expression * expr = BinaryExpression(left_op, arithmeticPrimeNode.op_token->getText(), right_op);
        if (arithmeticPrimeNode.rest != nullptr){
            return convert(*arithmeticPrimeNode.rest, expr);
        }
        return expr;
    }

    // Convert an ArithmeticNode to an Arithmetic structure
    AST::Expression* convert(const ArithmeticNode& arithmeticNode) {
        // Implementation to convert ArithmeticNode
        Expression* first_op = convert(*arithmeticNode.prod);
        if (arithmeticNode.rest != nullptr){
            return convert(*arithmeticNode.rest, first_op);
        }
        return first_op;
    }

    // Convert a ConjunctionPrimeNode to a ConjunctionPrime structure
    AST::Expression* convert(const ConjunctionPrimeNode& conjunctionPrimeNode, Expression* left_op) {
        // Implementation to convert ConjunctionPrimeNode
        Expression * right_op = convert(*conjunctionPrimeNode.bool_unit);
        Expression * expr = BinaryExpression(left_op, conjunctionPrimeNode.op_token->getText(), right_op);
        if (conjunctionPrimeNode.rest != nullptr){
            return convert(*conjunctionPrimeNode.rest, expr);
        }
        return expr;
    }

    // Convert a ConjunctionNode to a Conjunction structure
    AST::Expression* convert(const ConjunctionNode& conjunctionNode) {
        // Implementation to convert ConjunctionNode
        Expression* first_op = convert(*conjunctionNode.bool_unit);
        if (conjunctionNode.rest != nullptr){
            return convert(*conjunctionNode.rest, first_op);
        }
        return first_op;
    }

    // Convert a BooleanPrimeNode to a BooleanPrime structure
    AST::Expression* convert(const BooleanPrimeNode& booleanPrimeNode, Expression* left_op) {
        // Implementation to convert BooleanPrimeNode
        Expression* right_op = convert(*booleanPrimeNode.conjunction);
        Expression* expr = BinaryExpression(left_op, booleanPrimeNode.op_token->getText(), right_op);
        if (booleanPrimeNode.rest != nullptr){
            return convert(*booleanPrimeNode.rest, expr);
        }
        return expr;
    }

    // Convert a BooleanNode to a Boolean structure
    AST::Expression* convert(const BooleanNode& booleanNode) {
        // Implementation to convert BooleanNode
        Expression* first_op = convert(*booleanNode.conjunction);
        if (booleanNode.rest != nullptr){
            return convert(*booleanNode.rest, first_op);
        }
        return first_op;
    }

    // Convert a RecordPrimeNode to a RecordPrime structure
    std::map<std::string, Expression*> convert(const RecordPrimeNode& recordPrimeNode) {
        // Implementation to convert RecordPrimeNode
        std::map<std::string, Expression*> map;
        map[recordPrimeNode.name->getText()] = convert(*recordPrimeNode.expr);
        if (recordPrimeNode.rest != nullptr){
            std::map<std::string, Expression*>rest = convert(*recordPrimeNode.rest);
            map.insert(rest.begin(), rest.end());
        }
        return map;
    }

    // Convert a RecordNode to a Record structure
    AST::Record* convert(const RecordNode& recordNode) {
        // Implementation to convert RecordNode
        std::map<std::string, Expression*> map;
        if (recordNode.inside != nullptr){
            map = convert(*recordNode.inside); 
        }
        return new AST::Record(map);
    }

    // Convert a LHSPrimeNode to a LHSPrime structure
    AST::Expression* convert(const LHSPrimeNode& lhsPrimeNode,  Expression* left_expr) {
        // Implementation to convert LHSPrimeNode
        if (lhsPrimeNode.name != nullptr){
            left_expr = FieldDereferernce(left_expr, lhsPrimeNode.name->getText());
        }
        if (lhsPrimeNode.expr != nullptr){
            left_expr = IndexExpression(left_expr, convert(*lhsPrimeNode.expr));
        }
        if (lhsPrimeNode.rest != nullptr){
            return convert(*lhsPrimeNode.rest, left_expr);
        }
        return left_expr
    }

    // Convert a LHSNode to a LHS structure
    AST::Expression* convert(const LHSNode& lhsNode) {
        if (lhsNode.inside != nullptr){
            return convert(*lhsNode.inside, new AST::Expression(lhsNode.name->getText()));
        }else{
            return new AST::Expression(lhsNode.name->getText());
        }

    }

    // Convert a ParameterPrimeNode to a ParameterPrime structure
    std::vector<Expression*>  convert(const ParameterPrimeNode& parameterPrimeNode) {
        // Implementation to convert ParameterPrimeNode
        std::vector<Expression*> parameters; // Create a new AST::vector to hold the converted expressions
        parameters.push_back(convert(*parameterPrimeNode.expr));
        if (parameterNode.rest != nullptr){
            std::vector<Expression*> remainder = convert(*parameterPrimeNode.rest);
            parameters.insert(parameters.end(), remainder.begin(), remainder.end());
        }
        return parameters;
    }

    // Convert a ParameterNode to a Parameter structure
    std::vector<Expression*> convert(const ParameterNode& parameterNode) {
        // Implementation to convert ParameterNode
        std::vector<Expression*> parameters; // Create a new AST::vector to hold the converted expressions
        parameters.push_back(convert(*parameterNode.expr));
        if (parameterNode.rest != nullptr){
            std::vector<Expression*> remainder = convert(*parameterNode.rest);
            parameters.insert(parameters.end(), remainder.begin(), remainder.end());
        }
        return parameters;
    }

    // Convert an ArgumentNode to an Argument structure
    std::vector<std::string> convert(const ArgumentNode& argumentNode) {
        // Implementation to convert ArgumentNode
        std::vector<Expression*> arguments; // Create a new AST::vector to hold the converted expressions
        arguments.push_back(argumentNode.name->getText());
        if (argumentNode.rest != nullptr){
            std::vector<std::string> remainder = convert(*argumentNode.rest);
            arguments.insert(arguments.end(), remainder.begin(), remainder.end());
        }
        return arguments;
    }

    // Convert an ArgumenPrimeNode to an ArgumenPrime structure
    std::vector<std::string> convert(const ArgumentPrimeNode& argumentPrimeNode) {
        // Implementation to convert ArgumenPrimeNode
        std::vector<Expression*> arguments; // Create a new AST::vector to hold the converted expressions
        arguments.push_back(argumentPrimeNode.name->getText());
        if (argumentPrimeNode.rest != nullptr){
            std::vector<std::string> remainder = convert(*argumentPrimeNode.rest);
            arguments.insert(arguments.end(), remainder.begin(), remainder.end());
        }
        return arguments;
    }

    // Convert a CallStatementNode to a CallStatement structure
    AST::CallStatement* convert(const CallStatementNode& callStatementNode) {
        // Implementation to convert CallStatementNode
        returne new AST::CallStatement(convert(*callStatementNode.call));
    }

    // Convert an AssignmentNode to an Assignment structure
    AST::Assignment* convert(const AssignmentNode& assignmentNode) {
        // Implementation to convert AssignmentNode
        return new AST::Assignment(convert(*assignmentNode.lhs), convert(*assignmentNode.expr));
    }

    // Convert a WhileNode to a WhileLoop structure
    AST::WhileLoop* convert(const WhileNode& whileNode) {
        // Implementation to convert WhileNode
        return new AST::WhileLoop(*whileNode.expr, *whileNode.block);
    }

    // Convert an IfStatementNode to an IfStatement structure
    AST::IfStatement* convert(const IfStatementNode& ifStatementNode) {
        // Implementation to convert IfStatementNode
        return new AST::IfStatement(convert(*ifStatementNode.expr), convert(*ifStatementNode.block_if), convert(*ifStatementNode.else_block));
    }

    // Convert an ElseNode to an Else structure
    AST::Block* convert(const ElseNode& elseNode) {
        return convert(*elseNode.block);
    }
};
