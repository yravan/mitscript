#include "interpreter.h"
// #include "assert.h"


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
    Value* value = global_frame_->getLocalVar(global_indices_[current_function_->getName(index)]);
    pushOntoStack(value);
}

void Interpreter::storeGlobal(int index){
    Value* value = popFromStack();
    global_frame_->setLocalVar(global_indices_[current_function_->getName(index)], value);
}

void Interpreter::pushReference(int index){
    Reference* ref = stack_frames_.back()->getReference(index);
    pushOntoStack(ref);
}

void Interpreter::loadReference(){
    Value* value = popFromStack();
    validateValueType<Reference>(value);
    Reference* ref = static_cast<Reference*>(value);
    Value* ref_value = ref->getValue();
    pushOntoStack(ref_value);
}

void Interpreter::storeReference(){
    Value* value = popFromStack();
    validateValueType<Reference>(value);
    Reference* ref = static_cast<Reference*>(value);
    Value* ref_value = popFromStack();
    ref->setValue(ref_value);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
// Record instructions
// ------------------------------------------------------------
void Interpreter::allocRecord(){
    Record* record = heap_->allocate<Record>();
    pushOntoStack(record);
}

void Interpreter::fieldLoad(int index){
    Value* value = popFromStack();
    validateValueType<Record>(value);
    Record* record = static_cast<Record*>(value);
    std::string field(std::move(current_function_->getName(index)));
    Value* record_value = record->getValue(field);
    pushOntoStack(record_value);
}

void Interpreter::fieldStore(int index){
    std::string field(std::move(current_function_->getName(index)));
    Value* record_value = popFromStack();
    Value* value = popFromStack();
    validateValueType<Record>(value);
    Record* record = static_cast<Record*>(value);
    record->setValue(field, record_value);
} 

void Interpreter::indexLoad(){
    Value* index = popFromStack();
    std::string field(std::move(stringCast(index)));
    Value* value = popFromStack();
    validateValueType<Record>(value);
    Record* record = static_cast<Record*>(value);
    Value* record_value = record->getValue(field);
    pushOntoStack(record_value);
}

void Interpreter::indexStore(){
    Value* record_value = popFromStack();

    Value* index = popFromStack();
    std::string field(std::move(stringCast(index)));

    Value* value = popFromStack();
    validateValueType<Record>(value);
    Record* record = static_cast<Record*>(value);
    record->setValue(field, record_value);
}
// ------------------------------------------------------------

// ------------------------------------------------------------
// Closure instructions
// ------------------------------------------------------------
void Interpreter::allocClosure(int num_free_vars){
    Closure* closure = heap_->allocate<Closure>(num_free_vars);
    for (int i = 0; i < num_free_vars; i++) {
        Value* free_var = popFromStack();
        validateValueType<Reference>(free_var);
        Reference* free_var_ref = static_cast<Reference*>(free_var);
        closure->addFreeVar(num_free_vars - i - 1, free_var_ref);
    }
    Value* value = popFromStack();
    validateValueType<Function>(value);
    Function* function = static_cast<Function*>(value);
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
    validateValueType<Closure>(value);
    Closure* closure = static_cast<Closure*>(value);

    Function* function = closure->getFunction();
    if (num_args != function->getParameterCount()) {
        throw RuntimeException(num_args, function->getParameterCount());
    }

    stack_frames_.push_back(heap_->allocate<Frame>(function->getLocalVars().size(), function->getLocalReferenceVars().size(), function->numFreeVars()));
    // set default values to None
    for (int i = 0; i < function->getLocalVars().size(); i++) {
        stack_frames_.back()->setLocalVar(i, none_);
    }
    for (int i =0; i < num_args; i++) {
        stack_frames_.back()->setLocalVar(i, arguments[i]);
    }
    stack_frames_.back()->makeLocalReferences(function->getLocalReferenceVars());
    stack_frames_.back()->addFreeVariables(closure->getFreeVars(), closure->getNumFreeVars());

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
    if (left->getType() == Value::Type::Integer && right->getType() == Value::Type::Integer) {
        Constant::Integer* right_int = static_cast<Constant::Integer*>(right);
        Constant::Integer* left_int = static_cast<Constant::Integer*>(left);
        Constant::Integer* result = heap_->allocate<Constant::Integer>(left_int->getValue() + right_int->getValue());
        pushOntoStack(result);
        return;
    }
    if (!(left->getType() == Value::Type::String) && !(right->getType() == Value::Type::String)) {
        throw IllegalCastException();
    }
    Constant::String* result = heap_->allocate<Constant::String>(left->toString() + right->toString());
    pushOntoStack(result);
}

void Interpreter::sub(){
    Value* rightValue = popFromStack();
    validateValueType<Constant::Integer>(rightValue);
    Constant::Integer* right = static_cast<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    validateValueType<Constant::Integer>(leftValue);
    Constant::Integer* left = static_cast<Constant::Integer*>(leftValue);
    Constant::Integer* result = heap_->allocate<Constant::Integer>(left->getValue() - right->getValue());
    pushOntoStack(result);
}

void Interpreter::mul(){
    Value* rightValue = popFromStack();
    validateValueType<Constant::Integer>(rightValue);
    Constant::Integer* right = static_cast<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    validateValueType<Constant::Integer>(leftValue);
    Constant::Integer* left = static_cast<Constant::Integer*>(leftValue);
    Constant::Integer* result = heap_->allocate<Constant::Integer>(left->getValue() * right->getValue());
    pushOntoStack(result);
}

void Interpreter::div(){
    Value* rightValue = popFromStack();
    validateValueType<Constant::Integer>(rightValue);
    Constant::Integer* right = static_cast<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    validateValueType<Constant::Integer>(leftValue);
    Constant::Integer* left = static_cast<Constant::Integer*>(leftValue);
    if (right->getValue() == 0) {
        throw IllegalArithmeticException();
    }
    Constant::Integer* result = heap_->allocate<Constant::Integer>(left->getValue() / right->getValue());
    pushOntoStack(result);
}

void Interpreter::neg(){
    Value* value = popFromStack();
    validateValueType<Constant::Integer>(value);
    Constant::Integer* int_value = static_cast<Constant::Integer*>(value);
    Constant::Integer* result = heap_->allocate<Constant::Integer>(-int_value->getValue());
    pushOntoStack(result);
}
// ------------------------------------------------------------ 

// ------------------------------------------------------------
// Comparison instructions
// ------------------------------------------------------------
void Interpreter::gt(){
    Value* rightValue = popFromStack();
    validateValueType<Constant::Integer>(rightValue);
    Constant::Integer* right = static_cast<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    validateValueType<Constant::Integer>(leftValue);
    Constant::Integer* left = static_cast<Constant::Integer*>(leftValue);
    if (left->getValue() > right->getValue()) {
        pushOntoStack(true_);
    } else {
        pushOntoStack(false_);
    }
}

void Interpreter::geq(){
    Value* rightValue = popFromStack();
    validateValueType<Constant::Integer>(rightValue);
    Constant::Integer* right = static_cast<Constant::Integer*>(rightValue);
    Value* leftValue = popFromStack();
    validateValueType<Constant::Integer>(leftValue);
    Constant::Integer* left = static_cast<Constant::Integer*>(leftValue);
    if (left->getValue() >= right->getValue()) {
        pushOntoStack(true_);
    } else {
        pushOntoStack(false_);
    }
}

void Interpreter::eq(){
    Value* right = popFromStack();
    Value* left = popFromStack();

    Constant::Boolean* result = false_;
    Value::Type left_type = left->getType();
    Value::Type right_type = right->getType();
    //PrimitiveEqualityMismatched
      //PrimitiveEquality
      if (left_type == Value::Type::Integer && right_type == Value::Type::Integer) {
          Constant::Integer* left_int = static_cast<Constant::Integer*>(left);
          Constant::Integer* right_int = static_cast<Constant::Integer*>(right);
          result = (left_int->getValue() == right_int->getValue()) ? true_ : false_;
      }
      if (left_type == Value::Type::Boolean && right_type == Value::Type::Boolean) {
          Constant::Boolean* left_bool = static_cast<Constant::Boolean*>(left);
          Constant::Boolean* right_bool = static_cast<Constant::Boolean*>(right);
          result = (left_bool->getValue() == right_bool->getValue()) ? true_ : false_;
      }
      if (left_type == Value::Type::String && right_type == Value::Type::String) {
          Constant::String* left_str = static_cast<Constant::String*>(left);
          Constant::String* right_str = static_cast<Constant::String*>(right);
          result = (left_str->getValue() == right_str->getValue()) ? true_ : false_;
      }

      //NoneEquality
      if (left_type == Value::Type::None && right_type == Value::Type::None) {
          result = true_;
      }

      //FunctionEqualityTrue
      if (left_type == Value::Type::Function && right_type == Value::Type::Function) {
          result = false_;
      }

      //RecordEquality
      if (left_type == Value::Type::Record && right_type == Value::Type::Record) {
          result = left == right ? true_ : false_;
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
    validateValueType<Constant::Boolean>(right);
    validateValueType<Constant::Boolean>(left);
    Constant::Boolean* right_bool = static_cast<Constant::Boolean*>(right);
    Constant::Boolean* left_bool = static_cast<Constant::Boolean*>(left);
    if (left_bool->getValue() && right_bool->getValue()) {
        pushOntoStack(true_);
    } else {
        pushOntoStack(false_);
    }
}

void Interpreter::orStatement(){
    Value* right = popFromStack();
    Value* left = popFromStack();
    validateValueType<Constant::Boolean>(right);
    validateValueType<Constant::Boolean>(left);
    Constant::Boolean* right_bool = static_cast<Constant::Boolean*>(right);
    Constant::Boolean* left_bool = static_cast<Constant::Boolean*>(left);
    if (left_bool->getValue() || right_bool->getValue()) {
        pushOntoStack(true_);
    } else {
        pushOntoStack(false_);
    }
}

void Interpreter::notStatement(){
    Value* value = popFromStack();
    validateValueType<Constant::Boolean>(value);
    Constant::Boolean* bool_value = static_cast<Constant::Boolean*>(value);
    if (!bool_value->getValue()) {
        pushOntoStack(true_);
    } else {
        pushOntoStack(false_);
    }
}
// ------------------------------------------------------------

// ------------------------------------------------
void Interpreter::gotoStatement(int offset){
    instruction_pointer_ += offset;
    instruction_pointer_ -= 1;
}

void Interpreter::ifStatement(int offset){
    Value* value = popFromStack();
    validateValueType<Constant::Boolean>(value);
    Constant::Boolean* bool_value = static_cast<Constant::Boolean*>(value);
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
    heap_->addMemory(sizeof(*this));
    #ifdef DEBUG
    DEBUG_PRINT("Executing program");
    heap_->dump();
    #endif
    TrackingAllocator<Frame*> allocator;
    allocator.setHeap(heap_);

    stack_frames_ = TrackingVector<Frame*>(allocator);
    global_indices_ = TrackingUnorderedMap<std::string, int>(allocator);
    native_functions_ = TrackingUnorderedSet<Function*>(allocator);

    current_function_ = program;
    global_frame_ = heap_->allocate<Frame>(program->getNames().size(), program->getLocalReferenceVars().size(), program->numFreeVars());
    stack_frames_.push_back(global_frame_);
    none_ = new Constant::None();
    true_ = new Constant::Boolean(true);
    false_ = new Constant::Boolean(false);
    
    int num_global_var = 0;
    for (const std::string& global_var : program->getNames()) {
        global_indices_[global_var] = num_global_var++;
        heap_->addMemory(global_var.capacity());
    }
    global_frame_->setNumLocalVars(num_global_var);
    for (int i = 0; i < num_global_var; i++) {
        global_frame_->setLocalVar(i, none_);
    }

    initializeNativeFunctions();

    executeFunction(program);
}

void Interpreter::initializeNativeFunctions() {
    printFunction* print_function = new printFunction();
    print_function->setNone(none_);
    NativeFunction* input_function = new inputFunction();
    NativeFunction* intcast_function = new intcastFunction();

    print_function->setHeap(heap_);
    input_function->setHeap(heap_);
    intcast_function->setHeap(heap_);

    native_functions_.insert(print_function);
    native_functions_.insert(input_function);
    native_functions_.insert(intcast_function);

    current_function_->setFunction(0, print_function);
    current_function_->setFunction(1, input_function);
    current_function_->setFunction(2, intcast_function);
}

void Interpreter::executeFunction(Function* function) {
    if (native_functions_.find(function) != native_functions_.end()) {
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
    int max_instructions = function->numInstructions();
    while (instruction_pointer_ < max_instructions) {
        executeInstruction();
    }
    current_function_ = prev_function;
    instruction_pointer_ = prev_instruction_pointer;
}

void Interpreter::garbageCollect() {
    if (heap_->isFull()) {
        #ifdef DEBUG
        DEBUG_PRINT("Garbage collecting");
        heap_->dump();
        #endif
        heap_->gc(stack_frames_.begin(), stack_frames_.end());
        #ifdef DEBUG
        DEBUG_PRINT("Garbage collection complete");
        heap_->dump();
        #endif
    }
}


void Interpreter::pushOntoStack(Value* value){
    stack_frames_.back()->push(value);
}

Value* Interpreter::popFromStack(){
    return stack_frames_.back()->pop();
}

template <typename T>
void Interpreter::validateValueType(Value* value) {
    Value::Type expectedType;

    // Determine the expected type based on T
    if constexpr (std::is_same_v<T, Constant::Integer>) {
        expectedType = Value::Type::Integer;
    } else if constexpr (std::is_same_v<T, Constant::String>) {
        expectedType = Value::Type::String;
    } else if constexpr (std::is_same_v<T, Constant::Boolean>) {
        expectedType = Value::Type::Boolean;
    } else if constexpr (std::is_same_v<T, Constant::None>) {
        expectedType = Value::Type::None;
    } else if constexpr (std::is_same_v<T, Record>) {
        expectedType = Value::Type::Record;
    } else if constexpr (std::is_same_v<T, Reference>) {
        expectedType = Value::Type::Reference;
    } else if constexpr (std::is_same_v<T, Closure>) {
        expectedType = Value::Type::Closure;
    } else if constexpr (std::is_same_v<T, Function>) {
        expectedType = Value::Type::Function;
    } else {
        // assert(false);
    }

    // Check if the type matches
    if (value->getType() != expectedType) {
        throw IllegalCastException();
    }
}

std::string Interpreter::stringCast(Value* v) {
    return v->toString();
}


void Interpreter::executeInstruction(){
    garbageCollect();
    Instruction current_instruction = current_function_->getInstruction(instruction_pointer_);
    // DEBUG_PRINT("Executing instruction: " << current_instruction.toString());

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

