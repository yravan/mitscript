#include "types.h"
#include "exceptions.h"
// #include "frame.h"


std::string Function::toString() {
    return name_;
}

Instruction& Function::getInstruction(uint index) {
    if (index >= instructions.size() || index < 0) {
        throw RuntimeException("Invalid instruction index");
    }
    return instructions[index];
}

void Function::setParameterCount(uint32_t count) {
    parameter_count_ = count;
}

uint32_t Function::getParameterCount() {
    return parameter_count_;
}

Value::Type Function::getType() const {
    return Type::Function;
}

Constant* Function::getConstant(int index) {
    if (index >= constants_.size() || index < 0) {
        throw RuntimeException("Invalid constant index");
    }
    return constants_[index];
}

std::vector<Constant*>& Function::getConstants() {
    return constants_;
}

void Function::addConstant(Constant* constant) {
    constants_.push_back(constant);
}

Function* Function::getFunction(int index) {
    if (index >= functions_.size() || index < 0) {
        throw RuntimeException("Invalid function index");
    }
    return functions_[index];
}

int Function::getFunction(Function* function) {
    return std::distance(functions_.begin(), std::find(functions_.begin(), functions_.end(), function));
}

void Function::addFunction(Function* function) {
    functions_.push_back(function);
}

void Function::setFunction(int index, Function* function) {
    functions_[index] = function;
}

const std::string& Function::getLocalVar(int index) {
    if (index >= local_vars_.size() || index < 0) {
        throw RuntimeException("Invalid local variable index");
    }
    return local_vars_[index];
}

std::vector<std::string>& Function::getLocalVars() {
    return local_vars_;
}

int Function::getLocalVar(const std::string& name) {
    return std::distance(local_vars_.begin(), std::find(local_vars_.begin(), local_vars_.end(), name));
}

void Function::addLocalVar(std::string name) {
    if (std::find(local_vars_.begin(), local_vars_.end(), name) == local_vars_.end()) {
        local_vars_.push_back(name);
    }
}

int Function::getLocalReferenceVar(const std::string& name) {
    return std::distance(local_reference_vars_.begin(),
                         std::find(local_reference_vars_.begin(), local_reference_vars_.end(), name));
}

void Function::addLocalReferenceVar(std::string name) {
    if (std::find(local_reference_vars_.begin(), local_reference_vars_.end(), name) == local_reference_vars_.end()) {
        local_reference_vars_.push_back(name);
    }
}

std::vector<std::string>& Function::getFreeVars() {
    return free_vars_;
}

int Function::getFreeVar(const std::string& name) {
    return std::distance(free_vars_.begin(),
                         std::find(free_vars_.begin(), free_vars_.end(), name)) + local_reference_vars_.size();
}

void Function::addFreeVar(std::string name) {
    if (std::find(free_vars_.begin(), free_vars_.end(), name) == free_vars_.end()) {
        free_vars_.push_back(name);
    }
}

int Function::numFreeVars() {
    return free_vars_.size();
}

const std::string& Function::getName(int index) {
    if (index >= names_.size() || index < 0) {
        throw RuntimeException("Invalid name index");
    }
    return names_[index];
}

const std::vector<std::string>& Function::getNames() {
    return names_;
}

void Function::addName(std::string name) {
    if (std::find(names_.begin(), names_.end(), name) == names_.end()) {
        names_.push_back(name);
    }
}

int Function::getGlobalVar(const std::string& name) {
    return std::distance(names_.begin(), std::find(names_.begin(), names_.end(), name));
}

bool Function::findLocalVar(const std::string& name) {
    return std::find(local_vars_.begin(), local_vars_.end(), name) != local_vars_.end();
}

bool Function::findGlobalVar(const std::string& name) {
    return std::find(names_.begin(), names_.end(), name) != names_.end();
}

InstructionList& Function::getInstructions() {
    return instructions;
}

void Function::setInstructionList(InstructionList instructions) {
    this->instructions = instructions;
}

int Function::numInstructions() {
    return instructions.size();
}

void Function::makeLocalReferenceIndices() {
    for (const std::string& name : local_reference_vars_) {
        auto it = std::distance(local_vars_.begin(), std::find(local_vars_.begin(), local_vars_.end(), name));
        local_reference_indices_.push_back(it);
    }
}

std::vector<int>& Function::getLocalReferenceVars() {
    return local_reference_indices_;
}

void Function::follow(CollectedHeap& heap) {
    for (Constant* constant : constants_) {
        heap.markSuccessors(constant);
    }
    for (Function* function : functions_) {
        heap.markSuccessors(function);
    }
}

Constant::None Record::none_;

Value* Reference::getValue() {
    return frame_->getLocalVar(index_);
}

void Reference::setValue(Value* value) {
    frame_->setLocalVar(index_, value);
}
