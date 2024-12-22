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
    for (Value* value : local_vars_) {
      ss << "  " << value->toString() << "\n";
    }
    ss << "Local Reference Variables:\n";
    for (Reference* value : local_reference_vars_) {
      ss << "  " << value->toString() << "\n";
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
        local_reference_vars_.emplace_back(heap_->allocate<Reference>(local_reference_vars[i], this));
    }
}

void Frame::addFreeVariables(TrackingVector<Reference*>& free_vars) {
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

void Frame::follow(CollectedHeap& heap) {
    for (Value* value : stack_) {
        heap.markSuccessors(value);
    }
    for (Value* value : local_vars_) {
        heap.markSuccessors(value);
    }
    for (Reference* ref : local_reference_vars_) {
        heap.markSuccessors(ref);
    }
}

