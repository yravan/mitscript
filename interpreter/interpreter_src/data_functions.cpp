#include "interpreter.h"
#include "native_functions.h"


Value* Interpreter::variableRead(const AST::Expression& expr){
    DEBUG_PRINT("Entering variableRead" );
    DEBUG_PRINT("Exiting variableRead" );
    DEBUG_PRINT(typeid(lookup_read(stack.top(), expr.name)).name());
    return lookup_read(stack.top(), expr.name);
}

Record* Interpreter::record(const AST::Record& expr){
    DEBUG_PRINT("Entering record" );
    Record* rec = new Record();
    for (const auto& pair: expr.fields){
        pair.second->accept(*this);
        rec->map[pair.first] = rval;
    }
    return rec;
    DEBUG_PRINT("Exiting record" );
}


Value * Interpreter::fieldRead(const AST::FieldDereference& expr){
    DEBUG_PRINT("Entering fieldRead" );
    expr.baseExpr->accept(*this);
    Value* baseExpr = rval;
    if (typeid(*baseExpr) != typeid(Record)){
        throw IllegalCastException(".", baseExpr);
    }
    Record * record = dynamic_cast<Record*>(baseExpr);
    std::string field = expr.field;

    //FieldRead
    if (record->map.count(field)){
        return record->map[field];
    }
    //FieldReadFail
    return none_value;
    DEBUG_PRINT("Exiting fieldRead" );
}


Value * Interpreter::indexRead(const AST::IndexExpression& expr){
    DEBUG_PRINT("Entering indexRead" );
    expr.baseExpr->accept(*this);
    Value* baseExpr = rval;
    if (typeid(*baseExpr) != typeid(Record)){
        throw IllegalCastException(".", baseExpr);
    }
    Record * record = dynamic_cast<Record*>(baseExpr);

    expr.index->accept(*this);
    Value* index = rval;
    std::string field = string_cast(index);

    //IndexRead
    if (record->map.count(field)){
        return record->map[field];
    }
    //IndexReadFail
    return none_value;
    DEBUG_PRINT("Exiting indexRead" );
}

Function* Interpreter::function(const AST::FunctionDeclaration& expr){
    DEBUG_PRINT("Entering function" );
    Frame* calling_frame = stack.top();
    std::vector<std::string> formal_arguments = expr.arguments;
    AST::Block* body = expr.body;
    return new Function(calling_frame, formal_arguments, body);
    DEBUG_PRINT("Exiting function" );
}


void Interpreter::globals(const AST::Statement& s){
    DEBUG_PRINT("Entering globals" );
    DEBUG_PRINT("Printed" );
    if (typeid(s) == typeid(AST::Assignment) || typeid(s) == typeid(AST::Return)){
        // do nothing
    }
    if (typeid(s) == typeid(AST::IfStatement)){
        DEBUG_PRINT("Entering globals 1" );
        AST::IfStatement statement = dynamic_cast<const AST::IfStatement&>(s);
        globals(*statement.thenPart);
        if (statement.elsePart != nullptr){
            globals(*statement.elsePart);
        }
    }
    if (typeid(s) == typeid(AST::WhileLoop)){
        DEBUG_PRINT("Entering globals 2" );
        AST::WhileLoop statement = dynamic_cast<const AST::WhileLoop&>(s);
        globals(*statement.body);
    }
    if (typeid(s) == typeid(AST::Block)){
        DEBUG_PRINT("Entering globals 3" );
        AST::Block statement = dynamic_cast<const AST::Block&>(s);
        if (statement.statements.size() > 0){
            for (const auto& state: statement.statements){
                globals(*state);
            }
        }
    }
    if (typeid(s) == typeid(AST::Global)){
        DEBUG_PRINT("Entering globals 4" );
        // add this variable to global set 
        AST::Global statement = dynamic_cast<const AST::Global&>(s);
        Frame* cur_frame = stack.top();
        cur_frame->global_variables.insert(statement.name);
    }
    DEBUG_PRINT("Exiting globals" );
}

void Interpreter::assigns(const AST::Statement& s){
    DEBUG_PRINT("Entering assigns" );
    if (typeid(s) == typeid(AST::Assignment)){
        AST::Assignment statement = dynamic_cast<const AST::Assignment&>(s);
        if (typeid(*statement.lhs) == typeid(AST::Expression)){
            //VarAssignment
            Frame* cur_frame = stack.top();
            std::string variable = statement.lhs->name;
            cur_frame->map[variable] = none_value;
        }
        // do nothing
    }
    if (typeid(s) == typeid(AST::Global) || typeid(s) == typeid(AST::Return)){
        // do nothing
    }
    if (typeid(s) == typeid(AST::IfStatement)){
        AST::IfStatement statement = dynamic_cast<const AST::IfStatement&>(s);
        assigns(*statement.thenPart);
        if (statement.elsePart != nullptr){
            assigns(*statement.elsePart);
        }
    }
    if (typeid(s) == typeid(AST::WhileLoop)){
        AST::WhileLoop statement = dynamic_cast<const AST::WhileLoop&>(s);
        assigns(*statement.body);
    }
    if (typeid(s) == typeid(AST::Block)){
        AST::Block statement = dynamic_cast<const AST::Block&>(s);
        for (const auto& state: statement.statements){
            assigns(*state);
        }
    }
    DEBUG_PRINT("Exiting assigns" );
}

Value* Interpreter::functionCall(const AST::Call& expr){
    DEBUG_PRINT("Entering functionCall" );
    expr.targetExpr->accept(*this);
    Value* targetExpr = rval;
    if (!dynamic_cast<Function*>(targetExpr)){
        throw IllegalCastException("()", targetExpr);
    }
    Function* function = dynamic_cast<Function*>(targetExpr);
    std::vector<Value*> arguments;
    for (const auto& arg: expr.arguments){
        arg->accept(*this);
        arguments.push_back(rval);
    }
    if (arguments.size() !=  function->formal_arguments.size()){
        throw RuntimeException(arguments.size(), function->formal_arguments.size());
    }

    Frame* new_frame = new Frame(function->function_frame, global_stack_frame);
    stack.push(new_frame);
    if (typeid(*function) != typeid(NativeFunction)){
        globals(*(function->body));
        assigns(*(function->body));
    }
    for (size_t i = 0; i < arguments.size(); ++i) {
        auto param = function->formal_arguments[i];
        new_frame->map[param] = arguments[i];
    }
    DEBUG_PRINT(new_frame->to_string());

    if (typeid(*function) == typeid(NativeFunction)){
        NativeFunction* native_function = dynamic_cast<NativeFunction*>(function);
        native_function->call(*this);
    }else{
        function->body->accept(*this);
    }

    stack.pop();

    if (return_flag){
        return_flag = false;
        return rval;
    }
    else{
        return none_value;
    }
    DEBUG_PRINT("Exiting functionCall" );
}

