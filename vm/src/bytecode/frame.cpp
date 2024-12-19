#include "frame.h"
#include "exceptions.h"

void Frame::push(Value* value) {
    stack_.emplace_back(value);
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

void Frame::makeLocalReferences(const std::vector<int>& local_reference_vars) {
    local_reference_vars_.reserve(local_reference_vars.size());
    for (int i = 0; i < local_reference_vars.size(); i++) {
        local_reference_vars_.emplace_back(new Reference(local_reference_vars[i], this));
    }
}

void Frame::addFreeVariables(std::vector<Reference*>& free_vars) {
    local_reference_vars_.insert(local_reference_vars_.end(),
                                 std::make_move_iterator(free_vars.begin()),
                                 std::make_move_iterator(free_vars.end()));
}

Reference* Frame::getReference(int index) {
    if (index >= local_reference_vars_.size() || index < 0) {
        throw UninitializedVariableException("oops");
    }
    return local_reference_vars_[index];
}


