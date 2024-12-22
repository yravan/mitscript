/*
Written by me
*/
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <unordered_map>
#include "AST.h"
#include "CST.h"
#include "../MITScript.h"

namespace CST{
    class CSTConverter {
    public:
        AST::Operator convert_operator(std::string op){
            if (op == "+"){
                return AST::Operator::PLUS;
            }
            if (op == "-"){
                return AST::Operator::SUB;
            }
            if (op == "*"){
                return AST::Operator::MUL;
            }
            if (op == "/"){
                return AST::Operator::DIV;
            }
            if (op == "|"){
                return AST::Operator::OR;
            }
            if (op == "&"){
                return AST::Operator::AND;
            }
            if (op == "!"){
                return AST::Operator::NEG;
            }
            if (op == "<"){
                return AST::Operator::LT;
            }
            if (op == ">"){
                return AST::Operator::GT;
            }
            if (op == "<="){
                return AST::Operator::LEQ;
            }
            if (op == ">="){
                return AST::Operator::GEQ;
            }
            if (op == "=="){
                return AST::Operator::EQ;
            }
            throw std::runtime_error("Operator " + op + " not supported");
            assert(0);
        }

        // Convert a ProgramNode to a Program structure
        AST::Program* convert(const ProgramNode& programNode) {
            if (programNode.child != nullptr){
                return new AST::Program(convert(*programNode.child));
            }else{
                return new AST::Program(nullptr);
            }
        }

        AST::Block* convert(const StatementListNode& statementListNode) {
            
            std::vector<AST::Statement*> statements;
            statements.push_back(convert(*statementListNode.statement));
            if (statementListNode.rest != nullptr) {
                std::vector<AST::Statement*> remainder = convert(*statementListNode.rest)->statements;
                statements.insert(statements.end(), remainder.begin(), remainder.end());
            }

            return new AST::Block(statements);
        }

        AST::Statement* convert(const StatementNode& statementNode) {
            
            if (statementNode.assignment != nullptr) {
                return convert(*statementNode.assignment);
            }
            if (statementNode.callStatement != nullptr) {
                return convert(*statementNode.callStatement);
            }
            if (statementNode.global != nullptr) {
                return convert(*statementNode.global);
            }
            if (statementNode.ifStatement != nullptr) {
                return convert(*statementNode.ifStatement);
            }
            if (statementNode.whileLoop != nullptr) {
                return convert(*statementNode.whileLoop);
            }
            if (statementNode.returnNode != nullptr) {
                return convert(*statementNode.returnNode);
            }
            return nullptr;
        }

        // Convert a BlockNode to a Block structure
        AST::Block* convert(const BlockNode& blockNode) {
            if (blockNode.inside != nullptr) {
                return new AST::Block(convert(*blockNode.inside));
            }
            return new AST::Block(std::vector<AST::Statement*>());
        }

        std::vector<AST::Statement*> convert(const BlockPrimeNode& blockPrimeNode) {
            
            std::vector<AST::Statement*> statements;
            statements.push_back(convert(*blockPrimeNode.statement));
            if (blockPrimeNode.rest != nullptr) {
                std::vector<AST::Statement*> remainder = convert(*blockPrimeNode.rest);
                statements.insert(statements.end(), remainder.begin(), remainder.end());
            }

            return statements;
        }

        // Convert a GlobalNode to a Global structure
        AST::Global* convert(const GlobalNode& globalNode) {
            return new AST::Global(globalNode.name->getText());
        }

        AST::Expression* convert(const UnitNode& unitNode) {
            
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
                AST::Operator op = convert_operator("-");
                return new AST::UnaryExpression(expr, op);
            }

            return expr;
        }

        AST::Assignment* convert(const AssignmentNode& assignmentNode) {
            
            AST::Expression* lhs = convert(*assignmentNode.lhs);
            AST::Expression* expr = convert(*assignmentNode.expr);

            return new AST::Assignment(lhs, expr);
        }

        AST::Return* convert(const ReturnNode& returnNode) {
            return new AST::Return(convert(*returnNode.expr));
        }

        AST::FunctionDeclaration* convert(const FunctionNode& functionNode) {
            if (functionNode.args == nullptr) {
                return new AST::FunctionDeclaration(std::vector<std::string>(), convert(*functionNode.block));
            }
            return new AST::FunctionDeclaration(convert(*functionNode.args), convert(*functionNode.block));
        }

        AST::Call* convert(const CallNode& callNode) {
            
            AST::Expression* targ_expr = convert(*callNode.lhs);
            std::vector<AST::Expression*> params;
            if (callNode.arguments != nullptr) {
                params = convert(*callNode.arguments);
            }

            return new AST::Call(targ_expr, params);
        }

        AST::Expression* convert(const ExpressionNode& expressionNode) {
            
            if (expressionNode.fun != nullptr) {
                return convert(*expressionNode.fun);
            }
            if (expressionNode.boolean != nullptr) {
                return convert(*expressionNode.boolean);
            }
            if (expressionNode.record != nullptr) {
                return convert(*expressionNode.record);
            }
            return nullptr;
        }

        AST::Expression* convert(const ConstantNode& constantNode) {
            
            switch (constantNode.value->getType()) {
                case MITScript::INT:
                    return new AST::IntegerConstant(std::stoi(constantNode.value->getText()));
                case MITScript::STRING:{
                    std::string val = constantNode.value->getText();
                    std::string result;
                    val.erase(0, 1);
                    val.pop_back();
                    for (int i = 0; i < val.length(); i++) {
                        if (val[i] == '\\' && i + 1 < val.length()) {
                            switch (val[i + 1]) {
                                case 'n': result += '\n'; break;
                                case 't': result += '\t'; break;
                                case '\\': result += '\\'; break;
                                case '"': result += '"'; break;
                                default: throw std::runtime_error("Invalid escape sequence detected");
                            }
                            i++; // Skip the next character since it's part of the escape sequence
                        } else {
                            result += val[i];
                        }
                    }
                    return new AST::StringConstant(result);
                }
                case MITScript::NONE:
                    return new AST::NoneConstant();
                case MITScript::TRUE:
                    return new AST::BooleanConstant(true);
                case MITScript::FALSE:
                    return new AST::BooleanConstant(false);
                default:
                    throw std::runtime_error("Unknown constant type");
            }

        }

        AST::Expression* convert(const PredicateNode& predicateNode) {
            

            AST::Expression* left_expr = convert(*predicateNode.arith_1);
            if (predicateNode.arith_2 != nullptr) {
                AST::Operator op = convert_operator(predicateNode.cop_token->getText());
                AST::Expression* right_expr = convert(*predicateNode.arith_2);
                return new AST::BinaryExpression(left_expr, op, right_expr);
            }

            return left_expr;
        }

        AST::Expression* convert(const BoolUnitNode& boolUnitNode) {
            
            AST::Expression* pred = convert(*boolUnitNode.predicate);
            if (boolUnitNode.negate) {
                AST::Operator op = convert_operator("!");
                return new AST::UnaryExpression(pred, op);
            }

            return pred;
        }

        AST::Expression* convert(const ProductPrimeNode& productPrimeNode, AST::Expression* left_op) {
            
            AST::Expression* right_op = convert(*productPrimeNode.unit);
            AST::Operator op = convert_operator(productPrimeNode.op_token->getText());
            AST::Expression* expr = new AST::BinaryExpression(left_op, op, right_op);
            if (productPrimeNode.rest != nullptr) {
                return convert(*productPrimeNode.rest, expr);
            }

            return expr;
        }

        AST::Expression* convert(const ProductNode& productNode) {
            
            AST::Expression* first_op = convert(*productNode.unit);
            if (productNode.rest != nullptr) {
                return convert(*productNode.rest, first_op);
            }

            return first_op;
        }

        AST::Expression* convert(const ArithmeticPrimeNode& arithmeticPrimeNode, AST::Expression* left_op) {
            
            AST::Expression* right_op = convert(*arithmeticPrimeNode.prod);
            AST::Operator op = convert_operator(arithmeticPrimeNode.op_token->getText());
            AST::Expression* expr = new AST::BinaryExpression(left_op, op, right_op);
            if (arithmeticPrimeNode.rest != nullptr) {
                return convert(*arithmeticPrimeNode.rest, expr);
            }

            return expr;
        }

        AST::Expression* convert(const ArithmeticNode& arithmeticNode) {
            
            AST::Expression* first_op = convert(*arithmeticNode.prod);
            if (arithmeticNode.rest != nullptr) {
                return convert(*arithmeticNode.rest, first_op);
            }

            return first_op;
        }

        AST::Expression* convert(const ConjunctionPrimeNode& conjunctionPrimeNode, AST::Expression* left_op) {
            
            AST::Expression* right_op = convert(*conjunctionPrimeNode.bool_unit);
            AST::Operator op = convert_operator(conjunctionPrimeNode.op_token->getText());
            AST::Expression* expr = new AST::BinaryExpression(left_op, op, right_op);
            if (conjunctionPrimeNode.rest != nullptr) {
                return convert(*conjunctionPrimeNode.rest, expr);
            }

            return expr;
        }

        AST::Expression* convert(const ConjunctionNode& conjunctionNode) {
            
            AST::Expression* first_op = convert(*conjunctionNode.bool_unit);
            if (conjunctionNode.rest != nullptr) {
                return convert(*conjunctionNode.rest, first_op);
            }

            return first_op;
        }

        AST::Expression* convert(const BooleanPrimeNode& booleanPrimeNode, AST::Expression* left_op) {
            
            AST::Expression* right_op = convert(*booleanPrimeNode.conjunction);
            AST::Operator op = convert_operator(booleanPrimeNode.op_token->getText());
            AST::Expression* expr = new AST::BinaryExpression(left_op, op, right_op);
            if (booleanPrimeNode.rest != nullptr) {
                return convert(*booleanPrimeNode.rest, expr);
            }

            return expr;
        }

        AST::Expression* convert(const BooleanNode& booleanNode) {
            
            AST::Expression* first_op = convert(*booleanNode.conjunction);
            if (booleanNode.rest != nullptr) {
                return convert(*booleanNode.rest, first_op);
            }

            return first_op;
        }

        std::vector<std::pair<std::string, AST::Expression*>> convert(const RecordPrimeNode& recordPrimeNode,std::vector<std::pair<std::string, AST::Expression*>>& map ) {
            map.push_back({recordPrimeNode.name->getText(), convert(*recordPrimeNode.expr)});
            if (recordPrimeNode.rest != nullptr) {
                convert(*recordPrimeNode.rest, map);
            }

            return map;
        }

        AST::Record* convert(const RecordNode& recordNode) {
            
            std::vector<std::pair<std::string, AST::Expression*>> map;
            if (recordNode.inside != nullptr) {
                map = convert(*recordNode.inside, map);
            }

            return new AST::Record(map);
        }

        AST::Expression* convert(const LHSPrimeNode& lhsPrimeNode, AST::Expression* left_expr) {
            
            if (lhsPrimeNode.name != nullptr) {
                left_expr = new AST::FieldDereference(left_expr, lhsPrimeNode.name->getText());
            }
            if (lhsPrimeNode.expr != nullptr) {
                left_expr = new AST::IndexExpression(left_expr, convert(*lhsPrimeNode.expr));
            }
            if (lhsPrimeNode.rest != nullptr) {
                return convert(*lhsPrimeNode.rest, left_expr);
            }

            return left_expr;
        }

        AST::Expression* convert(const LHSNode& lhsNode) {
            
            if (lhsNode.inside != nullptr) {
                return convert(*lhsNode.inside, new AST::Expression(lhsNode.name->getText()));
            } else {
                return new AST::Expression(lhsNode.name->getText());
            }

        }

        std::vector<AST::Expression*> convert(const ParameterPrimeNode& parameterPrimeNode) {
            
            std::vector<AST::Expression*> parameters;
            parameters.push_back(convert(*parameterPrimeNode.expr));
            if (parameterPrimeNode.rest != nullptr) {
                std::vector<AST::Expression*> remainder = convert(*parameterPrimeNode.rest);
                parameters.insert(parameters.end(), remainder.begin(), remainder.end());
            }

            return parameters;
        }

        std::vector<AST::Expression*> convert(const ParameterNode& parameterNode) {
            
            std::vector<AST::Expression*> parameters;
            if (parameterNode.expr != nullptr) {
                parameters.push_back(convert(*parameterNode.expr));
                if (parameterNode.rest != nullptr) {
                    std::vector<AST::Expression*> remainder = convert(*parameterNode.rest);
                    parameters.insert(parameters.end(), remainder.begin(), remainder.end());
                }
            }

            return parameters;
        }

        std::vector<std::string> convert(const ArgumentNode& argumentNode) {
            
            std::vector<std::string> arguments;
            if (argumentNode.name != nullptr) {
                arguments.push_back(argumentNode.name->getText());
                if (argumentNode.rest != nullptr) {
                    std::vector<std::string> remainder = convert(*argumentNode.rest);
                    arguments.insert(arguments.end(), remainder.begin(), remainder.end());
                }
            }

            return arguments;
        }

        std::vector<std::string> convert(const ArgumentPrimeNode& argumentPrimeNode) {
            
            std::vector<std::string> arguments;
            arguments.push_back(argumentPrimeNode.name->getText());
            if (argumentPrimeNode.rest != nullptr) {
                std::vector<std::string> remainder = convert(*argumentPrimeNode.rest);
                arguments.insert(arguments.end(), remainder.begin(), remainder.end());
            }

            return arguments;
        }

        AST::CallStatement* convert(const CallStatementNode& callStatementNode) {
            return new AST::CallStatement(convert(*callStatementNode.call));
        }

        AST::WhileLoop* convert(const WhileNode& whileNode) {
            auto condition = convert(*whileNode.expr);
            auto body =  convert(*whileNode.block);
            return new AST::WhileLoop(condition, body);
        }

        AST::IfStatement* convert(const IfStatementNode& ifStatementNode) {
            
            AST::Block* else_block = nullptr;
            if (ifStatementNode.else_block != nullptr) {
                else_block = convert(*ifStatementNode.else_block);
            }
            return new AST::IfStatement(convert(*ifStatementNode.expr), convert(*ifStatementNode.block_if), else_block);
        }

        AST::Block* convert(const ElseNode& elseNode) {
            return convert(*elseNode.block);
        }
    };
}
