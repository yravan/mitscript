#pragma once

#include <iostream>
#include <string>

#include "Visitor.h"

// This is where you get to define your pretty printer class, which should be
// a subtype of visitor.
class PrettyPrinter : public Visitor {
    public:
        void visit( Expression& expr){
            std::cout << expr.name;
        }
        void visit( Block& node) override{
            std::cout << "{" << std::endl;
            for (Statement * statement: node.statements){
                std::cout << "\t";
                statement->accept(*this);
                std::cout << std::endl;
            }
            std::cout << "}" << std::endl;
        }
        void visit( Global& node) override{
            std::cout << "global" << " " << node.name << ";";
        }
        void visit( Assignment& node) override {
            std::cout << "()";
            node.lhs->accept(*this);  // Use accept to dispatch the correct visit method
            std::cout << ")";
            std::cout << " = ";
            std::cout << "()";
            node.expr->accept(*this);
            std::cout << ")";
            std::cout << ";";
        }
        void visit( CallStatement& node) override {
            node.callExpr->accept(*this);  // Use accept for the call expression
        }

        void visit( IfStatement& node) override {
            std::cout << "if (";
            node.condition->accept(*this);  // Use accept for the condition
            std::cout << ") ";
            node.thenPart->accept(*this);  // Use accept for the then part
            if (node.elsePart != nullptr) {
                std::cout << " else ";
                node.elsePart->accept(*this);  // Use accept for the else part, if it exists
            }
        }

        void visit( WhileLoop& node) override {
            std::cout << "while (";
            node.condition->accept(*this);  // Use accept for the condition
            std::cout << ") ";
            node.body->accept(*this);  // Use accept for the body
        }

        void visit( Return& node) override {
            std::cout << "return ";
            std::cout << "()";
            node.expr->accept(*this);  // Use accept for the expression
            std::cout << ")";
            std::cout << ";";
        }

        void visit( FunctionDeclaration& node) override {
            std::cout << "function(";
            for (size_t i = 0; i < node.arguments.size(); i++) {
                if (i != 0) std::cout << ", ";
                std::cout << node.arguments[i];  // Print argument names directly
            }
            std::cout << ")" << std::endl;
            node.body->accept(*this);  // Use accept for the function body
        }

        void visit( BinaryExpression& node) override {
            std::cout << "(";
            node.left->accept(*this);  // Use accept for the left operand
            std::cout << " " << node.op << " ";
            node.right->accept(*this);  // Use accept for the right operand
            std::cout << ")";
        }

        void visit( UnaryExpression& node) override {
            std::cout << "(";
            if (node.preceding){
                std::cout << node.op << " ";  // Unary operator printed first
                node.operand->accept(*this);  // Use accept for the operand
            }else{
                node.operand->accept(*this);  // Use accept for the operand
                std::cout << node.op << " ";  // Unary operator printed first
            }
            std::cout << ")";
        }

        void visit( FieldDereference& node) override {
            std::cout << "(";
            node.baseExpr->accept(*this);  // Use accept for the base expression
            std::cout << ")";
            std::cout << "." << node.field;  // Print the field name
        }

        void visit( IndexExpression& node) override {
            std::cout << "(";
            node.baseExpr->accept(*this);  // Use accept for the base expression
            std::cout << ")";
            std::cout << "[";
            node.index->accept(*this);  // Use accept for the index expression
            std::cout << "]";
        }

        void visit( Call& node) override {
            node.targetExpr->accept(*this);  // Use accept for the target expression
            std::cout << "(";
            for (size_t i = 0; i < node.arguments.size(); i++) {
                if (i != 0) std::cout << ", ";
                node.arguments[i]->accept(*this);  // Use accept for each argument
            }
            std::cout << ")";
        }

        void visit( Record& node) override {
            std::cout << "{";
            std::cout << std::endl;
            bool first = true;
            for ( auto& field : node.fields) {
                if (!first) std::cout << ", ";
                std::cout << "\n ";
                std::cout << field.first << ": ";  // Print the field name
                field.second->accept(*this);  // Use accept for the field value
                first = false;
            }
            std::cout << "}";
        }

        void visit( IntegerConstant& node) override {
            std::cout << node.value;  // Print integer ant
        }

        void visit( StringConstant& node) override {
            std::cout << "\"" << node.value << "\"";  // Print string ant with quotes
        }

        void visit( NoneConstant& node) override {
            std::cout << "none";  // Print "none"
        }

        void visit( BooleanConstant& node) override {
            std::cout << (node.value ? "true" : "false");  // Print boolean ant
        }

        void visit( Program& node) override {
            node.mainBlock->accept(*this);  // Use accept for the main block of the program
        }
        void visit( AST_node& node) override {
            // Use accept for the main block of the program
        }



};
