#include "interpreter.h"
#include "assert.h"


// ------------------------------------------------------------
// Load and store instructions
// ------------------------------------------------------------
void Interpreter::loadConst(int index){
    Constant* constant = current_function_->getConstant(index);
    pushOntoStack(constant);
}
void Interpreter::loadFunc(int index){
    Function* function = current_function_->getFunction(index);
    pushOntoStack(function);
}

void Interpreter::loadLocal(int index){
    Value* value = stack_frames_.back()->getLocalVar(index);
    pushOntoStack(value);
}

void Interpreter::storeLocal(int index){
    Value* value = popFromStack();
    stack_frames_.back()->setLocalVar(index, value);
}

void Interpreter::loadGlobal(int index){
    Value* value = global_frame_->getGlobalVar(current_function_->getName(index));
    pushOntoStack(value);
}

void Interpreter::storeGlobal(int index){
    Value* value = popFromStack();
    global_frame_->setGlobalVar(current_function_->getName(index), value);
}

void Interpreter::pushReference(int index){
    Reference* ref = stack_frames_.back()->getReference(index);
    pushOntoStack(ref);
}

void Interpreter::loadReference(){
    Value* value = popFromStack();
    Reference* ref = validateValueType<Reference*>(value);
    Value* ref_value = ref->getValue();
    pushOntoStack(ref_value);
}

void Interpreter::storeReference(){
    Value* value = popFromStack();
    Reference* ref = validateValueType<Reference*>(value);
    Value* ref_value = popFromStack();
    ref->setValue(ref_value);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
// Record instructions
// ------------------------------------------------------------
void Interpreter::allocRecord(){
    Record* record = new Record();
    pushOntoStack(record);
}

void Interpreter::fieldLoad(int index){
    Value* value = popFromStack();
    Record* record = validateValueType<Record*>(value);
    std::string field = current_function_->getName(index);
    Value* record_value = record->getValue(field);
    pushOntoStack(record_value);
}

void Interpreter::fieldStore(int index){
    std::string field = current_function_->getName(index);
    Value* record_value = popFromStack();
    Value* value = popFromStack();
    Record* record = validateValueType<Record*>(value);
    record->setValue(field, record_value);
} 

void Interpreter::indexLoad(){
    Value* index = popFromStack();
    std::string field = stringCast(index);
    Value* value = popFromStack();
    Record* record = validateValueType<Record*>(value);
    Value* record_value = record->getValue(field);
    pushOntoStack(record_value);
}

void Interpreter::indexStore(){
    Value* record_value = popFromStack();

    Value* index = popFromStack();
    std::string field = stringCast(index);

    Value* value = popFromStack();
    Record* record = validateValueType<Record*>(value);
    record->setValue(field, record_value);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
// Closure instructions
// ------------------------------------------------------------
void Interpreter::allocClosure(int num_free_vars){
    Closure* closure = new Closure(num_free_vars);
    for (int i = 0; i < num_free_vars; i++) {
        Value* free_var = popFromStack();
        Reference* free_var_ref = validateValueType<Reference*>(free_var);
        closure->addFreeVar(num_free_vars - i - 1, free_var_ref);
    }
    Value* value = popFromStack();
    Function* function = validateValueType<Function*>(value);
    closure->setFunction(function);
    pushOntoStack(closure);
}

void Interpreter::call(int num_args){
    std::vector<Value*> arguments;
    arguments.resize(num_args);
    for (int i =0; i < num_args; i++) {
        Value* arg = popFromStack();
        arguments[num_args - i - 1] = arg;
    }
    Value* value = popFromStack();
    Closure* closure = validateValueType<Closure*>(value);

    Function* function = closure->getFunction();
    if (num_args != function->getParameterCount()) {
        throw RuntimeException(num_args, function->getParameterCount());
    }

    Frame* frame = new Frame();
    frame->setNumLocalVars(function->getLocalVars().size());
    stack_frames_.push_back(frame);
    // set default values to None
    for (int i = 0; i < function->getLocalVars().size(); i++) {
        stack_frames_.back()->setLocalVar(i, new Constant::None());
    }
    for (int i =0; i < num_args; i++) {
        frame->setLocalVar(i, arguments[i]);
    }
    frame->makeLocalReferences(function->getLocalReferenceVars());
    for (int i = 0; i < closure->getNumFreeVars(); i++) {
        frame->addFreeVariable(closure->getFreeVar(i));
    }

    executeFunction(function);

    Value* return_value = popFromStack();
    stack_frames_.pop_back();
    pushOntoStack(return_value);
}

void Interpreter::returnStatement(){
    Value* return_value = popFromStack();
    pushOntoStack(return_value);
    instruction_pointer_ = current_function_->numInstructions();
}

// ------------------------------------------------------------
// Arithmetic instructions
// ------------------------------------------------------------
void Interpreter::add(){
    Value* right = popFromStack();
    Value* left = popFromStack();
    if (dynamic_cast<Constant::Integer*>(right) && dynamic_cast<Constant::Integer*>(left)) {
        Constant::Integer* right_int = static_cast<Constant::Integer*>(right);
        Constant::Integer* left_int = static_cast<Constant::Integer*>(left);
        Constant::Integer* result = new Constant::Integer(left_int->getValue() + right_int->getValue());
        pushOntoStack(result);
        return;
    }
    if (!dynamic_cast<Constant::String*>(right) && !dynamic_cast<Constant::String*>(left)) {
        throw IllegalCastException();
    }
    Constant::String* result = new Constant::String(stringCast(left) + stringCast(right));
    pushOntoStack(result);
}

void Interpreter::sub(){
    Value* rightValue = popFromStack();
    Constant::Integer* right = validateValueType<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    Constant::Integer* left = validateValueType<Constant::Integer*>(leftValue);
    Constant::Integer* result = new Constant::Integer(left->getValue() - right->getValue());
    pushOntoStack(result);
}

void Interpreter::mul(){
    Value* rightValue = popFromStack();
    Constant::Integer* right = validateValueType<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    Constant::Integer* left = validateValueType<Constant::Integer*>(leftValue);
    Constant::Integer* result = new Constant::Integer(left->getValue() * right->getValue());
    pushOntoStack(result);
}

void Interpreter::div(){
    Value* rightValue = popFromStack();
    Constant::Integer* right = validateValueType<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    Constant::Integer* left = validateValueType<Constant::Integer*>(leftValue);
    if (right->getValue() == 0) {
        throw IllegalArithmeticException();
    }
    Constant::Integer* result = new Constant::Integer(left->getValue() / right->getValue());
    pushOntoStack(result);
}

void Interpreter::neg(){
    Value* value = popFromStack();
    Constant::Integer* int_value = validateValueType<Constant::Integer*>(value);
    Constant::Integer* result = new Constant::Integer(-int_value->getValue());
    pushOntoStack(result);
}
// ------------------------------------------------------------ 

// ------------------------------------------------------------
// Comparison instructions
// ------------------------------------------------------------
void Interpreter::gt(){
    Value* rightValue = popFromStack();
    Constant::Integer* right = validateValueType<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    Constant::Integer* left = validateValueType<Constant::Integer*>(leftValue);
    Constant::Boolean* result = new Constant::Boolean(left->getValue() > right->getValue());
    pushOntoStack(result);
}

void Interpreter::geq(){
    Value* rightValue = popFromStack();
    Constant::Integer* right = validateValueType<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    Constant::Integer* left = validateValueType<Constant::Integer*>(leftValue);
    Constant::Boolean* result = new Constant::Boolean(left->getValue() >= right->getValue());
    pushOntoStack(result);
}

void Interpreter::eq(){
    Value* right = popFromStack();
    Value* left = popFromStack();

    Constant::Boolean* result = new Constant::Boolean(false);
    //PrimitiveEqualityMismatched
      //PrimitiveEquality
      if (dynamic_cast<Constant::Integer*>(left) && dynamic_cast<Constant::Integer*>(right)) {
          Constant::Integer* left_int = static_cast<Constant::Integer*>(left);
          Constant::Integer* right_int = static_cast<Constant::Integer*>(right);
          result = new Constant::Boolean(left_int->getValue() == right_int->getValue());
      }
      if (dynamic_cast<Constant::Boolean*>(left) && dynamic_cast<Constant::Boolean*>(right)) {
          Constant::Boolean* left_bool = static_cast<Constant::Boolean*>(left);
          Constant::Boolean* right_bool = static_cast<Constant::Boolean*>(right);
          result = new Constant::Boolean(left_bool->getValue() == right_bool->getValue());
      }
      if (dynamic_cast<Constant::String*>(left) && dynamic_cast<Constant::String*>(right)) {
          Constant::String* left_str = static_cast<Constant::String*>(left);
          Constant::String* right_str = static_cast<Constant::String*>(right);
          result = new Constant::Boolean(left_str->getValue() == right_str->getValue());
      }

      //NoneEquality
      if (dynamic_cast<Constant::None*>(left) && dynamic_cast<Constant::None*>(right)) {
          result = new Constant::Boolean(true);
      }

      //FunctionEqualityTrue
      if (dynamic_cast<Function*>(left) && dynamic_cast<Function*>(right)) {
          result = new Constant::Boolean(false);
      }

      //RecordEquality
      if (dynamic_cast<Record*>(left) && dynamic_cast<Record*>(right)) {
          result = new Constant::Boolean(left == right);
      }
      // DEBUG_PRINT("Equality not implemented for type: " << typeid(*left).name());
      // assert(false);

    pushOntoStack(result);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
// Logical instructions
// ------------------------------------------------------------
void Interpreter::andStatement(){
    Value* right = popFromStack();
    Value* left = popFromStack();
    Constant::Boolean* right_bool = validateValueType<Constant::Boolean*>(right);
    Constant::Boolean* left_bool = validateValueType<Constant::Boolean*>(left);
    Constant::Boolean* result = new Constant::Boolean(left_bool->getValue() && right_bool->getValue());
    pushOntoStack(result);
}

void Interpreter::orStatement(){
    Value* right = popFromStack();
    Value* left = popFromStack();
    Constant::Boolean* right_bool = validateValueType<Constant::Boolean*>(right);
    Constant::Boolean* left_bool = validateValueType<Constant::Boolean*>(left);
    Constant::Boolean* result = new Constant::Boolean(left_bool->getValue() || right_bool->getValue());
    pushOntoStack(result);
}

void Interpreter::notStatement(){
    Value* value = popFromStack();
    Constant::Boolean* bool_value = validateValueType<Constant::Boolean*>(value);
    Constant::Boolean* result = new Constant::Boolean(!bool_value->getValue());
    pushOntoStack(result);
}
// ------------------------------------------------------------

// ------------------------------------------------
void Interpreter::gotoStatement(int offset){
    instruction_pointer_ += offset;
    instruction_pointer_ -= 1;
}

void Interpreter::ifStatement(int offset){
    Value* value = popFromStack();
    Constant::Boolean* bool_value = validateValueType<Constant::Boolean*>(value);
    if (bool_value->getValue()) {
        gotoStatement(offset);
    }
}

void Interpreter::dup(){
    Value* value = popFromStack();
    pushOntoStack(value);
    pushOntoStack(value);
}

void Interpreter::swap(){
    Value* value1 = popFromStack();
    Value* value2 = popFromStack();
    pushOntoStack(value1);
    pushOntoStack(value2);
}

void Interpreter::pop(){
    // DEBUG_PRINT("Popping value from stack");
    popFromStack();
}


void Interpreter::executeProgram(Function* program) {
    current_function_ = program;
    global_frame_ = new Frame();
    stack_frames_.push_back(global_frame_);

    program->setFunction(0, new printFunction());
    program->setFunction(1, new inputFunction());
    program->setFunction(2, new intcastFunction());
    executeFunction(program);
}

void Interpreter::executeFunction(Function* function) {
    if (dynamic_cast<NativeFunction*>(function)) {
        NativeFunction* native_function = static_cast<NativeFunction*>(function);
        native_function->setFrame(stack_frames_.back());
        native_function->execute();
        return;
    }
    auto prev_function = current_function_;
    auto prev_instruction_pointer = instruction_pointer_;
    current_function_ = function;
    // Set the instruction pointer to the start of the function
    instruction_pointer_ = 0;
    // Execute the function
    while (instruction_pointer_ < function->numInstructions()) {
        executeInstruction();
    }
    current_function_ = prev_function;
    instruction_pointer_ = prev_instruction_pointer;
}


void Interpreter::pushOntoStack(Value* value){
    stack_frames_.back()->push(value);
}

Value* Interpreter::popFromStack(){
    return stack_frames_.back()->pop();
}

template <typename T>
T Interpreter::validateValueType(Value* value){
    T temp = dynamic_cast<T>(value);
    if (temp == nullptr) {
        throw IllegalCastException();
    }
    return temp;
}

std::string Interpreter::stringCast(Value* v) {
    return v->toString();
}


void Interpreter::executeInstruction(){
    Instruction current_instruction = current_function_->getInstruction(instruction_pointer_);

    switch (current_instruction.operation) {
        case Operation::LoadConst:
            loadConst(current_instruction.operand0.value());
            break;
        case Operation::LoadFunc:
            loadFunc(current_instruction.operand0.value());
            break;
        case Operation::LoadLocal:  
            loadLocal(current_instruction.operand0.value());
            break;
        case Operation::StoreLocal:
            storeLocal(current_instruction.operand0.value());
            break;
        case Operation::LoadGlobal:
            loadGlobal(current_instruction.operand0.value());
            break;
        case Operation::StoreGlobal:
            storeGlobal(current_instruction.operand0.value());
            break;
        case Operation::PushReference:
            pushReference(current_instruction.operand0.value());
            break;
        case Operation::LoadReference: 
            loadReference(); 
            break;
        case Operation::StoreReference:
            storeReference();
            break;
        case Operation::AllocRecord:
            allocRecord();
            break;
        case Operation::FieldLoad:
            fieldLoad(current_instruction.operand0.value());
            break;
        case Operation::FieldStore:
            fieldStore(current_instruction.operand0.value());
            break;
        case Operation::IndexLoad:
            indexLoad();
            break;
        case Operation::IndexStore: 
            indexStore();
            break;
        case Operation::AllocClosure:
            allocClosure(current_instruction.operand0.value());
            break;
        case Operation::Call:
            call(current_instruction.operand0.value());
            break;
        case Operation::Return:
            returnStatement();
            break;
        case Operation::Add:
            add();
            break;
        case Operation::Sub:
            sub();
            break;
        case Operation::Mul:
            mul();
            break;
        case Operation::Div:
            div();
            break;  
        case Operation::Neg:
            neg();
            break;
        case Operation::Gt:
            gt(); 
            break;
        case Operation::Geq:
            geq();
            break;
        case Operation::Eq:
            eq();
            break;
        case Operation::And:
            andStatement();
            break;
        case Operation::Or:
            orStatement();
            break;
        case Operation::Not:
            notStatement();
            break;
        case Operation::Goto:
            gotoStatement(current_instruction.operand0.value());
            break;
        case Operation::If:
            ifStatement(current_instruction.operand0.value());
            break;
        case Operation::Dup:
            dup();
            break;
        case Operation::Swap:
            swap();
            break;
        case Operation::Pop:
            pop();
            break;
    }

    instruction_pointer_ += 1;

}

