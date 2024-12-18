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

Value* Frame::getLocalVar(int index) {
    if (index >= local_vars_.size() || index < 0) {
        throw RuntimeException("Invalid local variable index");
    }
    return local_vars_[index];
}

void Frame::setLocalVar(int index, Value* value) {
    local_vars_[index] = value;
}

Value* Frame::getGlobalVar(std::string var) {
    if (global_vars_.find(var) == global_vars_.end()) {
        throw RuntimeException("Invalid global variable name");
    }
    return global_vars_[var];
}

void Frame::setGlobalVar(std::string var, Value* value) {
    global_vars_[var] = value;
}

void Frame::makeLocalReferences(std::vector<int> local_reference_vars) {
    for (const int index: local_reference_vars) {
        local_reference_vars_.push_back(new Reference(index, this));
    }
}

void Frame::addFreeVariable(Reference* value) {
    local_reference_vars_.push_back(value);
}

Reference* Frame::getReference(int index) {
    if (index >= local_reference_vars_.size() || index < 0) {
        throw UninitializedVariableException("oops");
    }
    return local_reference_vars_[index];
}


