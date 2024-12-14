#include "frame.h"
#include "exceptions.h"

void Frame::push(Value* value) {
    stack_.push_back(value);
}

Value* Frame::pop() {
    if (stack_.empty()) {
        throw InsufficientStackException();
    }
    Value* value = stack_.back();
    stack_.pop_back();
    return value;
}

Value* Frame::getLocalVar(std::string var) {
    if (local_vars_.find(var) != local_vars_.end()) {
        return local_vars_[var];
    }
    throw UninitializedVariableException(var);
}

void Frame::setLocalVar(std::string var, Value* value) {
    local_vars_[var] = value;
}

void Frame::makeLocalReferences(std::vector<std::string> local_reference_vars) {
    for (const std::string& var: local_reference_vars) {
        local_reference_vars_[var] = new Reference(var, this);
    }
}

void Frame::addFreeVariable(Reference* value) {
    local_reference_vars_[value->getName()] = value;
}

Reference* Frame::getReference(std::string var) {
    if (local_reference_vars_.find(var) != local_reference_vars_.end()) {
        return dynamic_cast<Reference*>(local_reference_vars_[var]);
    }
    throw UninitializedVariableException(var);
}


