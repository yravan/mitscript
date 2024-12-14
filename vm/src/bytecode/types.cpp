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

std::vector<std::string> Function::getLocalReferenceVars() {
    return local_reference_vars_;
}

Value* Reference::getValue() {
    return frame_->getLocalVar(name_);
}

void Reference::setValue(Value* value) {
    frame_->setLocalVar(name_, value);
}
