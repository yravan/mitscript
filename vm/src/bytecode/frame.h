#pragma once
#include "types.h"

class Frame : public Value {
  // The operand stack
  std::vector<Value *> stack_;

  // The local variables
  std::map<std::string, Value *> local_vars_;

  // The local reference variables
  std::map<std::string, Reference *> local_reference_vars_;

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
    for (auto& [name, value] : local_vars_) {
      ss << "  " << name << ": " << value->toString() << "\n";
    }
    ss << "Local Reference Variables:\n";
    for (auto& [name, value] : local_reference_vars_) {
      ss << "  " << name << ": " << value->toString() << "\n";
    }
    return ss.str();
  }
  void push(Value* value);
  Value* pop();
  Value* getLocalVar(std::string var);
  void setLocalVar(std::string var, Value* value);
  void makeLocalReferences(std::vector<std::string> local_reference_vars);
  void addFreeVariable(Reference* value);
  Reference* getReference(std::string var);


};
