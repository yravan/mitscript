#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

#include "instructions.h"
#include "exceptions.h"

class Value {
public:
  Value() = default;
  virtual ~Value() {}
  virtual std::string toString() = 0;
};

class Constant : public Value {

public:
  virtual ~Constant() = default;
  virtual bool equals(const Constant& other) = 0;
  class None;
  class Integer;
  class String;
  class Boolean;
};

class Constant::None : public Constant {
public:
  virtual ~None() {}
  std::string toString() { return "None"; }
  int getValue() const { return 0; }
  bool equals (const Constant& other) { return dynamic_cast<const None*>(&other) != nullptr; }
};

class Constant::Integer : public Constant {
  int value;
public:
  Integer(int value) : value(value) {}
  virtual ~Integer() {}
  int getValue() const { return value; }
  std::string toString() { return std::to_string(value); }
  bool equals (const Constant& other) { 
    if (dynamic_cast<const Integer*>(&other) == nullptr) {
      return false;
    }
    return value == dynamic_cast<const Integer*>(&other)->getValue();
  }
};

class Constant::String : public Constant {
  std::string value;
public:
  String(std::string value) : value(value) {}
  std::string getValue() const { return value; }
  virtual ~String() {}
  std::string toString() { return value; }
  bool equals (const Constant& other) { 
    if (dynamic_cast<const String*>(&other) == nullptr) {
      return false;
    }
    return value == dynamic_cast<const String*>(&other)->getValue();
  }
};

class Constant::Boolean : public Constant {
  bool value;
public:
  Boolean(bool value) : value(value) {}
  bool getValue() const { return value; }
  virtual ~Boolean() {}
  std::string toString() { return value ? "true" : "false"; }
  bool equals (const Constant& other) { 
    if (dynamic_cast<const Boolean*>(&other) == nullptr) {
      return false;
    }
    return value == dynamic_cast<const Boolean*>(&other)->getValue();
  }
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
  uint32_t parameter_count_ = 0;

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
  std::vector<Constant*> getConstants() { return constants_; }
  void addConstant(Constant* constant) { constants_.push_back(constant);}

  Function* getFunction(int index);
  int getFunction(Function* function) {
    return std::distance(functions_.begin(), std::find(functions_.begin(), functions_.end(), function));
  }
  void addFunction(Function* function) { functions_.push_back(function);}
  void setFunction(int index, Function* function) { functions_[index] = function; }

  std::string getLocalVar(int index);
  std::vector<std::string> getLocalVars() { return local_vars_; }
  int getLocalVar(std::string name) {
    return std::distance(local_vars_.begin(), std::find(local_vars_.begin(), local_vars_.end(), name));
  }
  int getLocalReferenceVar(std::string name) {
    return std::distance(local_reference_vars_.begin(), std::find(local_reference_vars_.begin(), local_reference_vars_.end(), name));
  }
  void addLocalVar(std::string name) { 
    if(std::find(local_vars_.begin(), local_vars_.end(), name) == local_vars_.end()) {
      local_vars_.push_back(name);
    }
  }
  void addLocalReferenceVar(std::string name) { 
    if (std::find(local_reference_vars_.begin(), local_reference_vars_.end(), name) == local_reference_vars_.end()) {
      local_reference_vars_.push_back(name);
    }
  }
  void addFreeVar(std::string name) {
    if (std::find(free_vars_.begin(), free_vars_.end(), name) == free_vars_.end()) {
      free_vars_.push_back(name);
    }
  }
  int numFreeVars() { return free_vars_.size(); }
  std::vector<std::string> getFreeVars() { return free_vars_; }

  std::string getName(int index);
  std::vector<std::string> getNames() { return names_; }
  void addName(std::string name) { 
    if (std::find(names_.begin(), names_.end(), name) == names_.end()) {
      names_.push_back(name);
    }
  }

  std::string getReference(int index);
  uint32_t getParameterCount();
  void setParameterCount(uint32_t count) { parameter_count_ = count; }
  int numInstructions();
  InstructionList& getInstructions() { return instructions; }
  std::vector<int> getLocalReferenceVars();

  bool findLocalVar(std::string name) {
    return std::find(local_vars_.begin(), local_vars_.end(), name) != local_vars_.end();
  }
  bool findGlobalVar(std::string name) {
    return std::find(names_.begin(), names_.end(), name) != names_.end();
  }
  int getGlobalVar(std::string name) {
    return std::distance(names_.begin(), std::find(names_.begin(), names_.end(), name));
  }
  int getFreeVar(std::string name) {
    return std::distance(free_vars_.begin(), std::find(free_vars_.begin(), free_vars_.end(), name)) + local_reference_vars_.size();
  }
  
  void setInstructionList(InstructionList instructions) { this->instructions = instructions; }

};

class Frame;

class Reference : public Value{
    int index_;
    Frame* frame_;
public:
    Reference() {}
    Reference(int index, Frame* frame) : index_(index), frame_(frame) {}
    std::string toString() {throw RuntimeException("Attempting to print reference");};
    Value* getValue();
    void setValue(Value* value);
    int getIndex() { return index_; }
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
