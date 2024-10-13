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
        DEBUG_PRINT("Entering: convert(ProgramNode)");
        DEBUG_PRINT("Exiting: convert(ProgramNode)");
        return new AST::Program(convert(*programNode.child));
    }

    AST::Block* convert(const StatementListNode& statementListNode) {
        DEBUG_PRINT("Entering: convert(statementListNode)");
        
        std::vector<AST::Statement*> statements;
        statements.push_back(convert(*statementListNode.statement));
        if (statementListNode.rest != nullptr) {
            std::vector<AST::Statement*> remainder = convert(*statementListNode.rest)->statements;
            statements.insert(statements.end(), remainder.begin(), remainder.end());
        }

        DEBUG_PRINT("Exiting: convert(statementListNode)");
        return new AST::Block(statements);
    }

    AST::Statement* convert(const StatementNode& statementNode) {
        DEBUG_PRINT("Entering: convert(StatementNode)");
        
        if (statementNode.assignment != nullptr) {
            DEBUG_PRINT("Exiting: convert(StatementNode)");
            return convert(*statementNode.assignment);
        }
        if (statementNode.callStatement != nullptr) {
            DEBUG_PRINT("Exiting: convert(StatementNode)");
            return convert(*statementNode.callStatement);
        }
        if (statementNode.global != nullptr) {
            DEBUG_PRINT("Exiting: convert(StatementNode)");
            return convert(*statementNode.global);
        }
        if (statementNode.ifStatement != nullptr) {
            DEBUG_PRINT("Exiting: convert(StatementNode)");
            return convert(*statementNode.ifStatement);
        }
        if (statementNode.whileLoop != nullptr) {
            DEBUG_PRINT("Exiting: convert(StatementNode)");
            return convert(*statementNode.whileLoop);
        }
        if (statementNode.returnNode != nullptr) {
            DEBUG_PRINT("Exiting: convert(StatementNode)");
            return convert(*statementNode.returnNode);
        }
        return nullptr;
    }

    // Convert a BlockNode to a Block structure
    AST::Block* convert(const BlockNode& blockNode) {
        DEBUG_PRINT("Entering: convert(BlockNode)");
        if (blockNode.inside != nullptr) {
            DEBUG_PRINT("Exiting: convert(BlockNode)");
            return new AST::Block(convert(*blockNode.inside));
        }
        DEBUG_PRINT("Exiting: convert(BlockNode)");
        return new AST::Block(std::vector<AST::Statement*>());
    }

    std::vector<AST::Statement*> convert(const BlockPrimeNode& blockPrimeNode) {
        DEBUG_PRINT("Entering: convert(BlockPrimeNode)");
        
        std::vector<AST::Statement*> statements;
        statements.push_back(convert(*blockPrimeNode.statement));
        if (blockPrimeNode.rest != nullptr) {
            std::vector<AST::Statement*> remainder = convert(*blockPrimeNode.rest);
            statements.insert(statements.end(), remainder.begin(), remainder.end());
        }

        DEBUG_PRINT("Exiting: convert(BlockPrimeNode)");
        return statements;
    }

    // Convert a GlobalNode to a Global structure
    AST::Global* convert(const GlobalNode& globalNode) {
        DEBUG_PRINT("Entering: convert(GlobalNode)");
        DEBUG_PRINT("Exiting: convert(GlobalNode)");
        return new AST::Global(globalNode.name->getText());
    }

    AST::Expression* convert(const UnitNode& unitNode) {
        DEBUG_PRINT("Entering: convert(UnitNode)");
        
        AST::Expression* expr;
        if (unitNode.lhs != nullptr) {
            expr = convert(*unitNode.lhs);
        }
        if (unitNode.constant != nullptr) {
            expr = convert(*unitNode.constant);
        }
        if (unitNode.call != nullptr) {
            expr = convert(*unitNode.call);
        }
        if (unitNode.boolean != nullptr) {
            expr = convert(*unitNode.boolean);
        }
        if (unitNode.minus) {
            DEBUG_PRINT("Exiting: convert(UnitNode)");
            return new AST::UnaryExpression(expr, "-", true);
        }

        DEBUG_PRINT("Exiting: convert(UnitNode)");
        return expr;
    }

    AST::Assignment* convert(const AssignmentNode& assignmentNode) {
        DEBUG_PRINT("Entering: convert(AssignmentNode)");
        
        AST::Expression* lhs = convert(*assignmentNode.lhs);
        AST::Expression* expr = convert(*assignmentNode.expr);

        DEBUG_PRINT("Exiting: convert(AssignmentNode)");
        return new AST::Assignment(lhs, expr);
    }

    AST::Return* convert(const ReturnNode& returnNode) {
        DEBUG_PRINT("Entering: convert(ReturnNode)");
        DEBUG_PRINT("Exiting: convert(ReturnNode)");
        return new AST::Return(convert(*returnNode.expr));
    }

    AST::FunctionDeclaration* convert(const FunctionNode& functionNode) {
        DEBUG_PRINT("Entering: convert(FunctionNode)");
        if (functionNode.args == nullptr) {
            DEBUG_PRINT("Exiting: convert(FunctionNode)");
            return new AST::FunctionDeclaration(std::vector<std::string>(), convert(*functionNode.block));
        }
        DEBUG_PRINT("Exiting: convert(FunctionNode)");
        return new AST::FunctionDeclaration(convert(*functionNode.args), convert(*functionNode.block));
    }

    AST::Call* convert(const CallNode& callNode) {
        DEBUG_PRINT("Entering: convert(CallNode)");
        
        AST::Expression* targ_expr = convert(*callNode.lhs);
        std::vector<AST::Expression*> params;
        if (callNode.arguments != nullptr) {
            params = convert(*callNode.arguments);
        }

        DEBUG_PRINT("Exiting: convert(CallNode)");
        return new AST::Call(targ_expr, params);
    }

    AST::Expression* convert(const ExpressionNode& expressionNode) {
        DEBUG_PRINT("Entering: convert(ExpressionNode)");
        
        if (expressionNode.fun != nullptr) {
            DEBUG_PRINT("Exiting: convert(ExpressionNode)");
            return convert(*expressionNode.fun);
        }
        if (expressionNode.boolean != nullptr) {
            DEBUG_PRINT("Exiting: convert(ExpressionNode)");
            return convert(*expressionNode.boolean);
        }
        if (expressionNode.record != nullptr) {
            DEBUG_PRINT("Exiting: convert(ExpressionNode)");
            return convert(*expressionNode.record);
        }
        return nullptr;
    }

    AST::Expression* convert(const ConstantNode& constantNode) {
        DEBUG_PRINT("Entering: convert(ConstantNode)");
        
        switch (constantNode.value->getType()) {
            case MITScript::INT:
                DEBUG_PRINT("Exiting: convert(ConstantNode)");
                return new AST::IntegerConstant(std::stoi(constantNode.value->getText()));
            case MITScript::STRING:
                DEBUG_PRINT("Exiting: convert(ConstantNode)");
                return new AST::StringConstant(constantNode.value->getText());
            case MITScript::NONE:
                DEBUG_PRINT("Exiting: convert(ConstantNode)");
                return new AST::NoneConstant();
            case MITScript::TRUE:
                DEBUG_PRINT("Exiting: convert(ConstantNode)");
                return new AST::BooleanConstant(true);
            case MITScript::FALSE:
                DEBUG_PRINT("Exiting: convert(ConstantNode)");
                return new AST::BooleanConstant(false);
            default:
                throw std::runtime_error("Unknown constant type");
        }

    }

    AST::Expression* convert(const PredicateNode& predicateNode) {
        DEBUG_PRINT("Entering: convert(PredicateNode)");
        
        AST::Expression* left_expr = convert(*predicateNode.arith_1);
        if (predicateNode.arith_2 != nullptr) {
            DEBUG_PRINT("Exiting: convert(PredicateNode)");
            return new AST::BinaryExpression(left_expr, predicateNode.cop_token->getText(), convert(*predicateNode.arith_2));
        }

        DEBUG_PRINT("Exiting: convert(PredicateNode)");
        return left_expr;
    }

    AST::Expression* convert(const BoolUnitNode& boolUnitNode) {
        DEBUG_PRINT("Entering: convert(BoolUnitNode)");
        
        AST::Expression* pred = convert(*boolUnitNode.predicate);
        if (boolUnitNode.negate) {
            DEBUG_PRINT("Exiting: convert(BoolUnitNode)");
            return new AST::UnaryExpression(pred, "!", true);
        }

        DEBUG_PRINT("Exiting: convert(BoolUnitNode)");
        return pred;
    }

    AST::Expression* convert(const ProductPrimeNode& productPrimeNode, AST::Expression* left_op) {
        DEBUG_PRINT("Entering: convert(ProductPrimeNode)");
        
        AST::Expression* right_op = convert(*productPrimeNode.unit);
        AST::Expression* expr = new AST::BinaryExpression(left_op, productPrimeNode.op_token->getText(), right_op);
        if (productPrimeNode.rest != nullptr) {
            DEBUG_PRINT("Exiting: convert(ProductPrimeNode)");
            return convert(*productPrimeNode.rest, expr);
        }

        DEBUG_PRINT("Exiting: convert(ProductPrimeNode)");
        return expr;
    }

    AST::Expression* convert(const ProductNode& productNode) {
        DEBUG_PRINT("Entering: convert(ProductNode)");
        
        AST::Expression* first_op = convert(*productNode.unit);
        if (productNode.rest != nullptr) {
            return convert(*productNode.rest, first_op);
        }

        DEBUG_PRINT("Exiting: convert(ProductNode)");
        return first_op;
    }

    AST::Expression* convert(const ArithmeticPrimeNode& arithmeticPrimeNode, AST::Expression* left_op) {
        DEBUG_PRINT("Entering: convert(ArithmeticPrimeNode)");
        
        AST::Expression* right_op = convert(*arithmeticPrimeNode.prod);
        AST::Expression* expr = new AST::BinaryExpression(left_op, arithmeticPrimeNode.op_token->getText(), right_op);
        if (arithmeticPrimeNode.rest != nullptr) {
            DEBUG_PRINT("Exiting: convert(ArithmeticPrimeNode)");
            return convert(*arithmeticPrimeNode.rest, expr);
        }

        DEBUG_PRINT("Exiting: convert(ArithmeticPrimeNode)");
        return expr;
    }

    AST::Expression* convert(const ArithmeticNode& arithmeticNode) {
        DEBUG_PRINT("Entering: convert(ArithmeticNode)");
        
        AST::Expression* first_op = convert(*arithmeticNode.prod);
        if (arithmeticNode.rest != nullptr) {
            return convert(*arithmeticNode.rest, first_op);
        }

        DEBUG_PRINT("Exiting: convert(ArithmeticNode)");
        return first_op;
    }

    AST::Expression* convert(const ConjunctionPrimeNode& conjunctionPrimeNode, AST::Expression* left_op) {
        DEBUG_PRINT("Entering: convert(ConjunctionPrimeNode)");
        
        AST::Expression* right_op = convert(*conjunctionPrimeNode.bool_unit);
        AST::Expression* expr = new AST::BinaryExpression(left_op, conjunctionPrimeNode.op_token->getText(), right_op);
        if (conjunctionPrimeNode.rest != nullptr) {
            DEBUG_PRINT("Exiting: convert(ConjunctionPrimeNode)");
            return convert(*conjunctionPrimeNode.rest, expr);
        }

        DEBUG_PRINT("Exiting: convert(ConjunctionPrimeNode)");
        return expr;
    }

    AST::Expression* convert(const ConjunctionNode& conjunctionNode) {
        DEBUG_PRINT("Entering: convert(ConjunctionNode)");
        
        AST::Expression* first_op = convert(*conjunctionNode.bool_unit);
        if (conjunctionNode.rest != nullptr) {
            return convert(*conjunctionNode.rest, first_op);
        }

        DEBUG_PRINT("Exiting: convert(ConjunctionNode)");
        return first_op;
    }

    AST::Expression* convert(const BooleanPrimeNode& booleanPrimeNode, AST::Expression* left_op) {
        DEBUG_PRINT("Entering: convert(BooleanPrimeNode)");
        
        AST::Expression* right_op = convert(*booleanPrimeNode.conjunction);
        AST::Expression* expr = new AST::BinaryExpression(left_op, booleanPrimeNode.op_token->getText(), right_op);
        if (booleanPrimeNode.rest != nullptr) {
            DEBUG_PRINT("Exiting: convert(BooleanPrimeNode)");
            return convert(*booleanPrimeNode.rest, expr);
        }

        DEBUG_PRINT("Exiting: convert(BooleanPrimeNode)");
        return expr;
    }

    AST::Expression* convert(const BooleanNode& booleanNode) {
        DEBUG_PRINT("Entering: convert(BooleanNode)");
        
        AST::Expression* first_op = convert(*booleanNode.conjunction);
        if (booleanNode.rest != nullptr) {
            DEBUG_PRINT("Exiting: convert(BooleanNode)");
            return convert(*booleanNode.rest, first_op);
        }

        DEBUG_PRINT("Exiting: convert(BooleanNode)");
        return first_op;
    }

    std::map<std::string, AST::Expression*> convert(const RecordPrimeNode& recordPrimeNode) {
        DEBUG_PRINT("Entering: convert(RecordPrimeNode)");
        
        std::map<std::string, AST::Expression*> map;
        map[recordPrimeNode.name->getText()] = convert(*recordPrimeNode.expr);
        if (recordPrimeNode.rest != nullptr) {
            std::map<std::string, AST::Expression*> rest = convert(*recordPrimeNode.rest);
            map.insert(rest.begin(), rest.end());
        }

        DEBUG_PRINT("Exiting: convert(RecordPrimeNode)");
        return map;
    }

    AST::Record* convert(const RecordNode& recordNode) {
        DEBUG_PRINT("Entering: convert(RecordNode)");
        
        std::map<std::string, AST::Expression*> map;
        if (recordNode.inside != nullptr) {
            map = convert(*recordNode.inside);
        }

        DEBUG_PRINT("Exiting: convert(RecordNode)");
        return new AST::Record(map);
    }

    AST::Expression* convert(const LHSPrimeNode& lhsPrimeNode, AST::Expression* left_expr) {
        DEBUG_PRINT("Entering: convert(LHSPrimeNode)");
        
        if (lhsPrimeNode.name != nullptr) {
            left_expr = new AST::FieldDereference(left_expr, lhsPrimeNode.name->getText());
        }
        if (lhsPrimeNode.expr != nullptr) {
            left_expr = new AST::IndexExpression(left_expr, convert(*lhsPrimeNode.expr));
        }
        if (lhsPrimeNode.rest != nullptr) {
            DEBUG_PRINT("Exiting: convert(LHSPrimeNode)");
            return convert(*lhsPrimeNode.rest, left_expr);
        }

        DEBUG_PRINT("Exiting: convert(LHSPrimeNode)");
        return left_expr;
    }

    AST::Expression* convert(const LHSNode& lhsNode) {
        DEBUG_PRINT("Entering: convert(LHSNode)");
        
        if (lhsNode.inside != nullptr) {
            return convert(*lhsNode.inside, new AST::Expression(lhsNode.name->getText()));
        } else {
            DEBUG_PRINT("Exiting: convert(LHSNode)");
            return new AST::Expression(lhsNode.name->getText());
        }

        DEBUG_PRINT("Exiting: convert(LHSNode)");
    }

    std::vector<AST::Expression*> convert(const ParameterPrimeNode& parameterPrimeNode) {
        DEBUG_PRINT("Entering: convert(ParameterPrimeNode)");
        
        std::vector<AST::Expression*> parameters;
        parameters.push_back(convert(*parameterPrimeNode.expr));
        if (parameterPrimeNode.rest != nullptr) {
            std::vector<AST::Expression*> remainder = convert(*parameterPrimeNode.rest);
            parameters.insert(parameters.end(), remainder.begin(), remainder.end());
        }

        DEBUG_PRINT("Exiting: convert(ParameterPrimeNode)");
        return parameters;
    }

    std::vector<AST::Expression*> convert(const ParameterNode& parameterNode) {
        DEBUG_PRINT("Entering: convert(ParameterNode)");
        
        std::vector<AST::Expression*> parameters;
        if (parameterNode.expr != nullptr) {
            parameters.push_back(convert(*parameterNode.expr));
            if (parameterNode.rest != nullptr) {
                std::vector<AST::Expression*> remainder = convert(*parameterNode.rest);
                parameters.insert(parameters.end(), remainder.begin(), remainder.end());
            }
        }

        DEBUG_PRINT("Exiting: convert(ParameterNode)");
        return parameters;
    }

    std::vector<std::string> convert(const ArgumentNode& argumentNode) {
        DEBUG_PRINT("Entering: convert(ArgumentNode)");
        
        std::vector<std::string> arguments;
        if (argumentNode.name != nullptr) {
            arguments.push_back(argumentNode.name->getText());
            if (argumentNode.rest != nullptr) {
                std::vector<std::string> remainder = convert(*argumentNode.rest);
                arguments.insert(arguments.end(), remainder.begin(), remainder.end());
            }
        }

        DEBUG_PRINT("Exiting: convert(ArgumentNode)");
        return arguments;
    }

    std::vector<std::string> convert(const ArgumentPrimeNode& argumentPrimeNode) {
        DEBUG_PRINT("Entering: convert(ArgumentPrimeNode)");
        
        std::vector<std::string> arguments;
        arguments.push_back(argumentPrimeNode.name->getText());
        if (argumentPrimeNode.rest != nullptr) {
            std::vector<std::string> remainder = convert(*argumentPrimeNode.rest);
            arguments.insert(arguments.end(), remainder.begin(), remainder.end());
        }

        DEBUG_PRINT("Exiting: convert(ArgumentPrimeNode)");
        return arguments;
    }

    AST::CallStatement* convert(const CallStatementNode& callStatementNode) {
        DEBUG_PRINT("Entering/Exiting: convert(CallStatementNode)");
        return new AST::CallStatement(convert(*callStatementNode.call));
    }

    AST::WhileLoop* convert(const WhileNode& whileNode) {
        DEBUG_PRINT("Entering/Exiting: convert(WhileNode)");
        return new AST::WhileLoop(convert(*whileNode.expr), convert(*whileNode.block));
    }

    AST::IfStatement* convert(const IfStatementNode& ifStatementNode) {
        DEBUG_PRINT("Entering: convert(IfStatementNode)");
        
        AST::Block* else_block = nullptr;
        if (ifStatementNode.else_block != nullptr) {
            else_block = convert(*ifStatementNode.else_block);
        }
        DEBUG_PRINT("Exiting: convert(IfStatementNode)");
        return new AST::IfStatement(convert(*ifStatementNode.expr), convert(*ifStatementNode.block_if), else_block);
    }

    AST::Block* convert(const ElseNode& elseNode) {
        DEBUG_PRINT("Entering/Exiting: convert(ElseNode)");
        return convert(*elseNode.block);
    }
};
