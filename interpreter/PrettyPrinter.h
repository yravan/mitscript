#pragma once
/*
Written by me
*/
#include <iostream>
#include <string>

#include "Visitor.h"

// This is where you get to define your pretty printer class, which should be
// a subtype of visitor.
void AST::Program::accept(Visitor &v){v.visit(*this);}
void AST::Expression::accept(Visitor &v){v.visit(*this);}
// void AST::Statement::accept(Visitor &v){v.visit(*this);}
void AST::Assignment::accept(Visitor &v){v.visit(*this);}
void AST::Block::accept(Visitor &v){v.visit(*this);}
void AST::Global::accept(Visitor &v){v.visit(*this);}
void AST::CallStatement::accept(Visitor &v){v.visit(*this);}
void AST::IfStatement::accept(Visitor &v){v.visit(*this);}
void AST::WhileLoop::accept(Visitor &v){v.visit(*this);}
void AST::Return::accept(Visitor &v){v.visit(*this);}
void AST::FunctionDeclaration::accept(Visitor &v){v.visit(*this);}
void AST::BinaryExpression::accept(Visitor &v){v.visit(*this);}
void AST::UnaryExpression::accept(Visitor &v){v.visit(*this);}
void AST::FieldDereference::accept(Visitor &v){v.visit(*this);}
void AST::IndexExpression::accept(Visitor &v){v.visit(*this);}
void AST::Call::accept(Visitor &v){v.visit(*this);}
void AST::Record::accept(Visitor &v){v.visit(*this);}
void AST::IntegerConstant::accept(Visitor &v){v.visit(*this);}
void AST::StringConstant::accept(Visitor &v){v.visit(*this);}
void AST::NoneConstant::accept(Visitor &v){v.visit(*this);}
void AST::BooleanConstant::accept(Visitor &v){v.visit(*this);}


class PrettyPrinter : public Visitor {
    int indent_level = 0;
    public:
        void indent(){
            for (int i = 0; i < indent_level; i ++){
                std::cout << "\t";
            }
        }
        void visit(const AST::Expression& expr){
            std::cout << expr.name;
        }
        void visit(const AST::Block& node) override{
            std::cout << "{" << std::endl;
            indent_level += 1;
            for (AST::Statement * statement: node.statements){
                statement->accept(*this);
                std::cout << std::endl;
            }
            indent_level -= 1;
            indent();
            std::cout << "}" ;
        }
        void visit(const AST::Global& node) override{
            indent();
            std::cout << "global" << " " << node.name << ";";
        }
        void visit(const AST::Assignment& node) override {
            indent();
            node.lhs->accept(*this);  // Use accept to dispatch the correct visit method
            std::cout << " = ";
            node.expr->accept(*this);
            std::cout << ";";
        }
        void visit(const AST::CallStatement& node) override {
            indent();
            node.callExpr->accept(*this);  // Use accept for the call expression
            std::cout << ';' << std::endl;
        }

        void visit(const AST::IfStatement& node) override {
            indent();
            std::cout << "if (";
            node.condition->accept(*this);  // Use accept for the condition
            std::cout << ") ";
            node.thenPart->accept(*this);  // Use accept for the then part
            if (node.elsePart != nullptr) {
                std::cout << " else ";
                node.elsePart->accept(*this);  // Use accept for the else part, if it exists
            }
        }

        void visit(const AST::WhileLoop& node) override {
            indent();
            std::cout << "while (";
            node.condition->accept(*this);  // Use accept for the condition
            std::cout << ") ";
            node.body->accept(*this);  // Use accept for the body
        }

        void visit(const AST::Return& node) override {
            indent();
            std::cout << "return ";
            node.expr->accept(*this);  // Use accept for the expression
            std::cout << ";";
        }

        void visit(const AST::FunctionDeclaration& node) override {
            std::cout << "function(";
            for (size_t i = 0; i < node.arguments.size(); i++) {
                if (i != 0) std::cout << ", ";
                std::cout << node.arguments[i];  // Print argument names directly
            }
            std::cout << ")";
            node.body->accept(*this);  // Use accept for the function body
        }

        void visit(const AST::BinaryExpression& node) override {
            std::cout << "("; 
            node.left->accept(*this);  // Use accept for the left operand
            std::cout << " " << node.op << " ";
            node.right->accept(*this);  // Use accept for the right operand
            std::cout << ")";
        }

        void visit(const AST::UnaryExpression& node) override {
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

        void visit(const AST::FieldDereference& node) override {
            node.baseExpr->accept(*this);  // Use accept for the base expression
            std::cout << "." << node.field;  // Print the field name
        }

        void visit(const AST::IndexExpression& node) override {
            node.baseExpr->accept(*this);  // Use accept for the base expression
            std::cout << "[";
            node.index->accept(*this);  // Use accept for the index expression
            std::cout << "]";
        }

        void visit(const AST::Call& node) override {
            node.targetExpr->accept(*this);  // Use accept for the target expression
            std::cout << "(";
            for (size_t i = 0; i < node.arguments.size(); i++) {
                if (i != 0) std::cout << ", ";
                node.arguments[i]->accept(*this);  // Use accept for each argument
            }
            std::cout << ")";
        }

        void visit(const AST::Record& node) override {
            std::cout << "{";
            std::cout << std::endl;
            indent_level += 1;
            bool first = true;
            for ( auto& field : node.fields) {
                indent();
                std::cout << field.first << ": ";  // Print the field name
                field.second->accept(*this);  // Use accept for the field value
                std::cout << ";" << std::endl;
            }
            indent_level -= 1;
            indent();
            std::cout << "}";
        }

        void visit(const AST::IntegerConstant& node) override {
            std::cout << node.value;  // Print integer ant
        }

        void visit(const AST::StringConstant& node) override {
            std::cout << node.value ;  // Print string ant with quotes
        }

        void visit(const AST::NoneConstant& node) override {
            std::cout << "none";  // Print "none"
        }

        void visit(const AST::BooleanConstant& node) override {
            std::cout << (node.value ? "true" : "false");  // Print boolean ant
        }

        void visit(const AST::Program& node) override {
            node.mainBlock->accept(*this);  // Use accept for the main block of the program
            std :: cout << std::endl;
        }



};
