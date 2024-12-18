#include "types.h"
#include "exceptions.h"
#include "frame.h"

Instruction& Function::getInstruction(uint index) {
    if (index >= instructions.size() || index < 0) {
        throw RuntimeException("Invalid instruction index");
    } 
    return instructions[index];
}

Constant* Function::getConstant(int index) {
    if (index >= constants_.size() || index < 0) {
        throw RuntimeException("Invalid constant index");
    } 
    return constants_[index];
}

Function* Function::getFunction(int index) {
    if (index >= functions_.size() || index < 0) {
        throw RuntimeException("Invalid function index");
    } 
    return functions_[index];
}

std::string Function::getLocalVar(int index) {
    if (index >= local_vars_.size() || index < 0) {
        throw RuntimeException("Invalid local variable index");
    } 
    return local_vars_[index];
}

std::string Function::getName(int index) {
    if (index >= names_.size() || index < 0) {
        throw RuntimeException("Invalid name index");
    } 
    return names_[index];
} 

std::string Function::getReference(int index) {
    if (index >= local_reference_vars_.size() + free_vars_.size() || index < 0) {
        throw RuntimeException("Invalid reference index");
    } 
    if (index < local_reference_vars_.size()) {
        return local_reference_vars_[index];
    }
    return free_vars_[index - local_reference_vars_.size()];
}

uint32_t Function::getParameterCount() {
    return parameter_count_;
} 

int Function::numInstructions() {
    return instructions.size();
}

std::vector<int> Function::getLocalReferenceVars() {
    std::vector<int> indices;
    for (const std::string& name : local_reference_vars_) {
        auto it = std::distance(local_vars_.begin(), std::find(local_vars_.begin(), local_vars_.end(), name));
        indices.push_back(it);
    }
    return indices;
}

Value* Reference::getValue() {
    return frame_->getLocalVar(index_);
}

void Reference::setValue(Value* value) {
    frame_->setLocalVar(index_, value);
}
