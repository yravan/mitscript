#pragma once
#include "types.h"

class Frame{
  // The operand stack
  std::vector<Value *> stack_;

  // The local variables
  std::vector<Value *> local_vars_;

  std::map<std::string, Value *> global_vars_;

  // The local reference variables
  std::vector<Reference*> local_reference_vars_;

public: 
  std::string toString() { return "FRAME"; }
  std::string dump() {
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
    ss << "Global Variables:\n";
    for (auto& [name, value] : global_vars_) {
      ss << "  " << name << ": " << value->toString() << "\n";
    }
    ss << "Local Reference Variables:\n";
    for (Value* value : local_reference_vars_) {
      ss << "  " << value->toString() << "\n";
    }
    return ss.str();
  }
  void push(Value* value);
  Value* pop();
  void setNumLocalVars(int num_vars) { local_vars_.resize(num_vars); }
  Value* getLocalVar(int index);
  void setLocalVar(int index, Value* value);
  void setGlobalVar(std::string var, Value* value);
  Value* getGlobalVar(std::string var);
  void makeLocalReferences(std::vector<int> local_reference_vars);
  void addFreeVariable(Reference* value);
  Reference* getReference(int index);


};
