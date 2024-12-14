#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "instructions.h"
#include "exceptions.h"

class Value {
public:
  Value() = default;
  virtual ~Value() {}
  virtual std::string toString() = 0;
};

class Constant : public Value {
  virtual ~Constant() {}

public:
  class None;
  class Integer;
  class String;
  class Boolean;
};

class Constant::None : public Constant {
public:
  virtual ~None() {}
  std::string toString() { return "None"; }
};

class Constant::Integer : public Constant {
  int value;
public:
  Integer(int value) : value(value) {}
  virtual ~Integer() {}
  int getValue() const { return value; }
  std::string toString() { return std::to_string(value); }
};

class Constant::String : public Constant {
  std::string value;
public:
  String(std::string value) : value(value) {}
  std::string getValue() const { return value; }
  virtual ~String() {}
  std::string toString() { return value; }
};

class Constant::Boolean : public Constant {
  bool value;
public:
  Boolean(bool value) : value(value) {}
  bool getValue() const { return value; }
  virtual ~Boolean() {}
  std::string toString() { return value ? "true" : "false"; }
};

class Function : public Value{

public:
  // List of functions defined within this function (but not functions defined
  // inside of nested functions)
  std::vector<Function *> functions_;

  // List of constants used by the inclassions within this function (but not
  // nested functions)
  std::vector<Constant *> constants_;

  // The number of parameters to the function
  uint32_t parameter_count_;

  // List of local variables
  // The first parameter_count_ variables are the function's parameters
  // in their order as given in the paraemter list
  std::vector<std::string> local_vars_;

  // List of local variables accessed by reference (LocalReference)
  std::vector<std::string> local_reference_vars_;

  // List of the names of non-global and non-local variables accessed by the
  // function
  std::vector<std::string> free_vars_;

  // List of global variable and field names used inside the function
  std::vector<std::string> names_;

  InstructionList instructions;

  Function() {}
  Function(std::vector<Function*> functions,
          std::vector<Constant*> constants,
          uint32_t parameter_count,
          std::vector<std::string> local_vars,
          std::vector<std::string> local_reference_vars,
          std::vector<std::string> free_vars,
          std::vector<std::string> names,
          InstructionList instructions)
      : functions_(std::move(functions)),
        constants_(std::move(constants)),
        parameter_count_(parameter_count),
        local_vars_(std::move(local_vars)),
        local_reference_vars_(std::move(local_reference_vars)),
        free_vars_(std::move(free_vars)),
        names_(std::move(names)),
        instructions(std::move(instructions)) {}

  std::string toString() {return "FUNCTION";};
  Instruction& getInstruction(uint index);
  Constant* getConstant(int index);
  Function* getFunction(int index);
  std::string getLocalVar(int index);
  std::string getName(int index);
  std::string getReference(int index);
  uint32_t getParameterCount();
  int numInstructions();
  std::vector<std::string> getLocalReferenceVars();
  void setFunction(int index, Function* function) { functions_[index] = function; }

};

class Frame;

class Reference : public Value{
    std::string name_;
    Frame* frame_;
public:
    Reference() {}
    Reference(std::string name, Frame* frame) : name_(name), frame_(frame) {}
    std::string toString() {throw RuntimeException("Attempting to print reference");};
    Value* getValue();
    void setValue(Value* value);
    std::string getName() { return name_; }
};

class Record : public Value {
  std::map<std::string, Value *> map_;
public:
  Record() = default;
  std::string toString() {
    std::stringstream ss;
    ss << "{";
    for (const auto& pair: map_){
        ss << pair.first << ":" << pair.second->toString() << " ";
    }
    ss << "}";
    return ss.str();
  }
  Value* getValue(std::string field) { 
    if (map_.find(field) == map_.end()) {
      return new Constant::None();
    }
    return map_[field]; 
  }
  void setValue(std::string field, Value* value) { map_[field] = value; }
};

class Closure : public Value {
  std::vector<Reference *> free_vars_;
  Function* function_;
public:
  std::string toString() {return "FUNCTION";};
  Closure(int num_free_vars) {
    free_vars_.resize(num_free_vars);
  }
  int getNumFreeVars() { return free_vars_.size(); }
  void addFreeVar(int index, Reference* value) { free_vars_[index] = value; }
  Reference* getFreeVar(int index) { return free_vars_[index]; }
  void setFunction(Function* function) { function_ = function; }
  Function* getFunction() { return function_; }
};
