
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
    UnitNode(bool minus_, ConstantNode* constant_);

    std::string to_string() override;

private:
    bool minus;
    ConstantNode* constant;
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

// ArithmeticPrimeNode class
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

// ProgramNode class
class ProgramNode : public CSTNode {
public:
    ProgramNode(ArithmeticNode* node);

    std::string to_string() override;

private:
    ArithmeticNode* child;
};
