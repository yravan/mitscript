#ifndef CST
#define CST
#include <iostream>
#include "antlr4-runtime.h"

void reportError(antlr4::Token &token) {
  std::stringstream str;
  str << "Unexpected Symbol (" << token.getLine() << ","
            << token.getCharPositionInLine() << "): " << token.getText()
            << "\n";
  throw std::runtime_error(str.str());
}

class CSTNode {
  public:
    virtual std::string to_string() = 0;
};

// ConstantNode class
class ConstantNode : public CSTNode {
public:
    ConstantNode(antlr4::Token * token_ ): value(token_) {}

    std::string to_string() {
        std::stringstream ss;
        ss << "Constant(" << value->getText() << ")";
        return ss.str();
    }

private:
    antlr4::Token * value;
};

// UnitNode class
class UnitNode : public CSTNode {
public:
    UnitNode(bool minus_, ConstantNode* constant_ ): minus(minus_), constant(constant_) {}

    std::string to_string() {
        std::stringstream ss;
        if (minus) {
            ss << "Unit(" << '-' << "," << constant->to_string() << ")";
        } else {
            ss << "Unit(" << constant->to_string() << ")";
        }
        return ss.str();
    }

private:
    bool minus;
    ConstantNode* constant;
};

// ProductPrimeNode class
class ProductPrimeNode : public CSTNode {
public:
    ProductPrimeNode(antlr4::Token * op_token_, UnitNode* unit_, ProductPrimeNode* rest_ ): op_token(op_token_), unit(unit_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "ProductPrime(" << op_token->getText() << "," << unit->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "ProductPrime(" << op_token->getText() << "," << unit->to_string() << ")";
        }
        return ss.str();
    }

private:
    antlr4::Token * op_token;
    UnitNode* unit;
    ProductPrimeNode* rest;
};

// ProductNode class
class ProductNode : public CSTNode {
public:
    ProductNode(UnitNode * unit_, ProductPrimeNode* rest_ ): unit(unit_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "ProductNode(" << unit->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "ProductNode(" << unit->to_string() << ")";
        }
        return ss.str();
    }

private:
    UnitNode* unit;
    ProductPrimeNode* rest;
};

// ArithmeticPrimeNode class
class ArithmeticPrimeNode : public CSTNode {
public:
    ArithmeticPrimeNode(antlr4::Token * op_token_, ProductNode * prod_, ArithmeticPrimeNode* rest_ ): op_token(op_token_), prod(prod_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "ArithmeticPrime(" << op_token->getText() << "," << prod->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "ArithmeticPrime(" << op_token->getText() << "," << prod->to_string() << ")";
        }
        return ss.str();
    }

private:
    antlr4::Token * op_token;
    ProductNode* prod;
    ArithmeticPrimeNode* rest;
};

// ArithmeticNode class
class ArithmeticNode : public CSTNode {
public:
    ArithmeticNode(ProductNode * prod_, ArithmeticPrimeNode* rest_ ): prod(prod_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "Arithmetic(" << prod->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "Arithmetic(" << prod->to_string() << ")";
        }
        return ss.str();
    }

private:
    ProductNode* prod;
    ArithmeticPrimeNode* rest;
};


class PredicateNode: public CSTNode {
public:
    PredicateNode(ArithmeticNode * arith_1_, antlr4::Token * cop_token_, ArithmeticNode * arith_2_ ): arith_1(arith_1_), cop_token(cop_token_), arith_2(arith_2_) {}

    std::string to_string() {
        std::stringstream ss;
        if (cop_token != NULL && arith_2 != NULL) {
            ss << "Predicate(" << arith_1->to_string() << ',' << cop_token->getText() <<',' << arith_2->to_string() << ")";
        } else {
            ss << "Predicate(" << arith_1->to_string() << ")";
        }
        return ss.str();
    }

private:
    ArithmeticNode* arith_1;
    antlr4::Token * cop_token;
    ArithmeticNode* arith_2;
};


class BoolUnitNode: public CSTNode {
public:
    BoolUnitNode(bool negate_, PredicateNode * predicate_ ): negate(negate_), predicate(predicate_) {}

    std::string to_string() {
        std::stringstream ss;
        if (negate) {
            ss << "BoolUnit(" <<'!' <<',' << predicate->to_string() << ")";
        } else {
            ss << "BoolUnit(" << predicate->to_string() << ")";
        }
        return ss.str();
    }

private:
    bool negate ;
    PredicateNode* predicate;
};


// ConjunctionPrimeNode class
class ConjunctionPrimeNode : public CSTNode {
public:
    ConjunctionPrimeNode(antlr4::Token * op_token_, BoolUnitNode * bool_unit_, ConjunctionPrimeNode* rest_ ): op_token(op_token_), bool_unit(bool_unit_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "ConjunctionPrime(" << op_token->getText() << "," << bool_unit->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "ConjunctionPrime(" << op_token->getText() << "," << bool_unit->to_string() << ")";
        }
        return ss.str();
    }

private:
    antlr4::Token * op_token;
    BoolUnitNode* bool_unit;
    ConjunctionPrimeNode* rest;
};
// ConjunctionNode class
class ConjunctionNode : public CSTNode {
public:
    ConjunctionNode(BoolUnitNode * bool_unit_, ConjunctionPrimeNode* rest_ ): bool_unit(bool_unit_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "Conjunction(" << bool_unit->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "Conjunction(" << bool_unit->to_string() << ")";
        }
        return ss.str();
    }

private:
    BoolUnitNode* bool_unit;
    ConjunctionPrimeNode* rest;
};


// BooleanPrimeNode class
class BooleanPrimeNode : public CSTNode {
public:
    BooleanPrimeNode(antlr4::Token * op_token_, ConjunctionNode * conjunction_, BooleanPrimeNode* rest_ ): op_token(op_token_), conjunction(conjunction_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "BooleanPrime(" << op_token->getText() << "," << conjunction->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "BooleanPrime(" << op_token->getText() << "," << conjunction->to_string() << ")";
        }
        return ss.str();
    }

private:
    antlr4::Token * op_token;
    ConjunctionNode* conjunction;
    BooleanPrimeNode* rest;
};
// BooleanNode class
class BooleanNode : public CSTNode {
public:
    BooleanNode(ConjunctionNode * conjunction_, BooleanPrimeNode* rest_ ): conjunction(conjunction_), rest(rest_) {}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL) {
            ss << "Boolean(" << conjunction->to_string() << ',' << rest->to_string() << ")";
        } else {
            ss << "Boolean(" << conjunction->to_string() << ")";
        }
        return ss.str();
    }

private:
    ConjunctionNode* conjunction;
    BooleanPrimeNode* rest;
};


// ProgramNode class
class RecordPrimeNode : public CSTNode {
public:
    RecordNode(antlr4::Token* name_, ExpressionNode expr_, RecordPrimeNode * rest_): name_(name), expr(expr_), rest(rest_){}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL){
            ss << "RecordPrime(" << name->getText() << ',' << expr->getText() << ',' << rest->to_string() << ')';
        }
        else{
            ss << "RecordPrime(" << name->getText() << ',' << expr->getText() << ')';
        }
        return ss.str();
    }

private:
    antlr4::Token* name;
    ExpressionNode expr;
    RecordPrimeNode * rest;
};
// ProgramNode class
class RecordNode : public CSTNode {
public:
    RecordNode(RecordPrimeNode * inside_): inside(inside_){}

    std::string to_string() {
        std::stringstream ss;
        if (inside != NULL){
            ss << "Record(" << inside->to_string() << ")";
        }
        else{
            ss << "Record(" << ")";
        }
        return ss.str();
    }

private:
    RecordPrimeNode * inside;
};

// ProgramNode class
class LHSPrimeNode : public CSTNode {
public:
    LHSNode(antlr4::Token* name_, ExpressionNode expr_, LHSPrimeNode * rest_): name_(name), expr(expr_), rest(rest_){}

    std::string to_string() {
        std::stringstream ss;
        if (rest != NULL){
            if (name != NULL){
                ss << "LHSPrime(" << name->getText() << ',' << rest->to_string() << ')';

            }
            if (expr != NULL){
                ss << "LHSPrime(" << expr->getText() << ',' << rest->to_string() << ')';

            }
        }
        else{
            ss << "LHSPrime(" << name->getText() << ',' << expr->getText() << ')';
        }
        return ss.str();
    }

private:
    antlr4::Token* name;
    ExpressionNode expr;
    LHSPrimeNode * rest;
};
// ProgramNode class
class LHSNode : public CSTNode {
public:
    LHSNode(antlr4::Token* name, LHSPrimeNode * inside_): name(name_), inside(inside_){}

    std::string to_string() {
        std::stringstream ss;
        if (inside != NULL){
            ss << "LHS(" << name->getText() << ',' << inside->to_string() << ")";
        }
        else{
            ss << "LHS(" << name->getText() << ")";
        }
        return ss.str();
    }

private:
    antlr4::Token* name;
    LHSPrimeNode * inside;
};



// ProgramNode class
class ExpressionNode : public CSTNode {
public:
    ExpressionNode(BooleanNode * fun_, BooleanNode* boolean_, RecordNode * record_ ): fun(fun_), boolean(boolean_), record(record_) {}

    std::string to_string() {
        std::stringstream ss;
        if (fun != NULL){
            ss << "Expression(" << fun->to_string() << ")";
        }
        if (boolean != NULL){
            ss << "Expression(" << boolean->to_string() << ")";
        }
        if (record != NULL){
            ss << "Expression(" << record->to_string() << ")";
        }
        return ss.str();
    }

private:
    BooleanNode * fun;
    BooleanNode * boolean;
    RecordNode * record;
};



// ProgramNode class
class CallStatementNode : public CSTNode {
public:
    CallStatementNode(CalltNode * call_ ): call(call_) {}

    std::string to_string() {
        std::stringstream ss;
        ss << "CallStatement(" << call->to_string() << ")";
        return ss.str();
    }

private:
    CalltNode * call;
};

// ProgramNode class
class GlobalNode : public CSTNode {
public:
    GlobalNode(antlr4::Token * name_ ): name(name_) {}

    std::string to_string() {
        std::stringstream ss;
        ss << "Global(" << name->getText() << ")";
        return ss.str();
    }

private:
    antlr4::Token* name;
};

// ProgramNode class
class ProgramNode : public CSTNode {
public:
    ProgramNode(BooleanNode * node ): child(node) {}

    std::string to_string() {
        std::stringstream ss;
        ss << "Program(" << child->to_string() << ")";
        return ss.str();
    }

private:
    BooleanNode* child;
};
#endif
