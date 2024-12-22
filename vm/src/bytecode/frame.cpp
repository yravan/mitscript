#include "types.h"
#include "exceptions.h"

std::string Frame::dump() {
    std::stringstream ss;
    ss << "Frame:\n";
    ss << "Stack:\n";
    for (Value* value : stack_) {
      ss << "  " << value->toString() << "\n";
    }
    ss << "Local Variables:\n";
    for (int i = 0; i < num_local_vars_; i++) {
      ss << "  " << local_vars_[i]->toString() << "\n";
    }
    ss << "Local Reference Variables:\n";
    for (int i = 0; i < num_local_reference_vars_ + num_free_vars_; i++) {
      ss << "  " << local_reference_vars_[i]->toString() << "\n";
    }
    return ss.str();
  }

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
    if (index >= num_local_vars_ || index < 0) {
        throw RuntimeException("Invalid local variable index");
    }
    return local_vars_[index];
}

void Frame::setLocalVar(int index, Value* value) {
    local_vars_[index] = value;
}

void Frame::makeLocalReferences(const std::vector<int>& local_reference_vars) {
    for (int i = 0; i < local_reference_vars.size(); i++) {
        local_reference_vars_[i] = heap_->allocate<Reference>(local_reference_vars[i], this);
    }
}

void Frame::addFreeVariables(Reference** free_vars, int num_vars) {
    for (int i = 0; i < num_vars; i++) {
        local_reference_vars_[num_local_reference_vars_ + i] = free_vars[i];
    }
}

Reference* Frame::getReference(int index) {
    if (index >= num_local_reference_vars_ + num_free_vars_ || index < 0) {
        throw UninitializedVariableException("oops");
    }
    return local_reference_vars_[index];
}

void Frame::follow(CollectedHeap& heap) {
    for (Value* value : stack_) {
        heap.markSuccessors(value);
    }
    for (int i = 0; i < num_local_vars_; i++) {
        heap.markSuccessors(local_vars_[i]);
    }
    for (int i = 0; i < num_local_reference_vars_ + num_free_vars_; i++) {
        heap.markSuccessors(local_reference_vars_[i]);
    }
}

