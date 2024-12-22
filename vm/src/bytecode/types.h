#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <set>

#include "instructions.h"
#include "exceptions.h"
#include "gc.h"

// Make the strings static (saves time but uses memoryZ)

class Value : public Collectable {
public:
  enum class Type { Integer, String, Boolean, None, Record, Function, Reference, Closure };
  Value() = default;
  virtual ~Value() {}
  virtual std::string toString() = 0;
  virtual Type getType() const = 0;

};

class Constant : public Value {

public:
  virtual ~Constant() = default;
  virtual bool equals(const Constant& other) = 0;
  class None;
  class Integer;
  class String;
  class Boolean;
  void follow(CollectedHeap& heap) override {}
  void initializeDynamicMemory(CollectedHeap* heap) override {}
};

class Constant::None : public Constant {
  inline static const std::string name_ = "None";
public:
  virtual ~None() {}
  std::string toString() { return name_; }
  bool equals (const Constant& other) { return other.getType() == Value::Type::None; }
  Type getType() const { return Type::None; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  size_t getCurrentSize() override {return sizeof(*this);}
};

class Constant::Integer : public Constant {
  int value_;
  std::string name_;
public:
  Integer(int value) : value_(value) {name_ = std::to_string(value);}
  virtual ~Integer() {}
  int getValue() const { return value_; }
  std::string toString() { return name_; }
  bool equals (const Constant& other) { 
    if(other.getType() != Value::Type::Integer) {
      return false;
    }
    return value_ == static_cast<const Integer*>(&other)->getValue();
  }
  Type getType() const { return Type::Integer; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this) + name_.capacity(); }
  size_t getCurrentSize() override {return sizeof(*this) + name_.capacity();}
};

class Constant::String : public Constant {
  std::string value_;
public:
  String(std::string value) : value_(std::move(value)) {}
  const std::string& getValue() const { return value_; }
  virtual ~String() {}
  std::string toString() { return value_; }
  bool equals (const Constant& other) { 
    if (other.getType() != Value::Type::String) {
      return false;
    }
    return value_ == static_cast<const String*>(&other)->getValue();
  }
  Type getType() const { return Type::String; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this) + value_.capacity(); }
  size_t getCurrentSize() override {return sizeof(*this) + value_.capacity(); }
};

class Constant::Boolean : public Constant {
  bool value;
  inline static const std::string true_string_ = "true";
  inline static const std::string false_string_ = "false";
public:
  Boolean(bool value) : value(value) {}
  bool getValue() const { return value; }
  virtual ~Boolean() {}
  std::string toString() { if(value) {return true_string_;} return false_string_; }
  bool equals (const Constant& other) { 
    if (other.getType() != Value::Type::Boolean) {
      return false;
    }
    return value == static_cast<const Boolean*>(&other)->getValue();
  }
  Type getType() const { return Type::Boolean; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  size_t getCurrentSize() override {return sizeof(*this);}
};

class Function : public Value {
  inline static std::string name_ = "FUNCTION";

public:
  std::vector<Function*> functions_;
  std::vector<Constant*> constants_;
  uint32_t parameter_count_ = 0;
  std::vector<std::string> local_vars_;
  std::vector<std::string> local_reference_vars_;
  std::vector<std::string> free_vars_;
  std::vector<std::string> names_;
  InstructionList instructions;

  Function() = default;
  Function(std::vector<Function*> functions,
           std::vector<Constant*> constants,
           uint32_t parameter_count,
           std::vector<std::string> local_vars,
           std::vector<std::string> local_reference_vars,
           std::vector<std::string> free_vars,
           std::vector<std::string> names,
           InstructionList instructions): functions_(std::move(functions)),
                                         constants_(std::move(constants)),
                                         parameter_count_(parameter_count),
                                         local_vars_(std::move(local_vars)),
                                         local_reference_vars_(std::move(local_reference_vars)),
                                         free_vars_(std::move(free_vars)),
                                         names_(std::move(names)),
                                         instructions(std::move(instructions)) {}

  std::string toString();
  Instruction& getInstruction(uint index);
  Type getType() const;

  void setParameterCount(uint32_t count);
  uint32_t getParameterCount();

  Constant* getConstant(int index);
  std::vector<Constant*>& getConstants();
  void addConstant(Constant* constant);

  Function* getFunction(int index);
  int getFunction(Function* function);
  void addFunction(Function* function);
  void setFunction(int index, Function* function);

  const std::string& getLocalVar(int index);
  std::vector<std::string>& getLocalVars();
  int getLocalVar(const std::string& name);
  void addLocalVar(std::string name);

  int getLocalReferenceVar(const std::string& name);
  void addLocalReferenceVar(std::string name);

  std::vector<std::string>& getFreeVars();
  int getFreeVar(const std::string& name);
  void addFreeVar(std::string name);
  int numFreeVars();

  const std::string& getName(int index);
  const std::vector<std::string>& getNames();
  void addName(std::string name);

  int getGlobalVar(const std::string& name);

  bool findLocalVar(const std::string& name);
  bool findGlobalVar(const std::string& name);

  InstructionList& getInstructions();
  void setInstructionList(InstructionList instructions);
  int numInstructions();

  std::vector<int> getLocalReferenceVars();

  void follow(CollectedHeap& heap) override;
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  void initializeDynamicMemory(CollectedHeap* heap) override {}
  size_t getCurrentSize() override {return sizeof(*this);}
};

class Reference;
class Frame : public Collectable {
  // The operand stack
  TrackingVector<Value*> stack_;

  // The local variables
  TrackingVector<Value*> local_vars_;

  // The local reference variables
  TrackingVector<Reference*> local_reference_vars_;

public: 
  Frame() {
    // stack_.reserve(10);
  } 
  inline std::string toString() { return "FRAME"; }
  std::string dump();
  void push(Value* value);
  Value* pop();
  void setNumLocalVars(int num_vars) { local_vars_.resize(num_vars); }
  Value* getLocalVar(int index);
  void setLocalVar(int index, Value* value);
  void makeLocalReferences(const std::vector<int>& local_reference_vars);
  void addFreeVariables(TrackingVector<Reference*>& free_vars);
  Reference* getReference(int index);

  void follow(CollectedHeap& heap) override;
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  void initializeDynamicMemory(CollectedHeap* heap) override {
    TrackingAllocator<Value*> allocator;
    allocator.setHeap(heap);
    stack_ = TrackingVector<Value*>(allocator);
    stack_.reserve(10);
    local_vars_ = TrackingVector<Value*>(allocator);
    local_reference_vars_ = TrackingVector<Reference*>(allocator);
  }
  size_t getCurrentSize() override {
    size_t size = sizeof(*this);
    size += stack_.get_allocator().getCurrentMemory();
    size += local_vars_.get_allocator().getCurrentMemory();
    size += local_reference_vars_.get_allocator().getCurrentMemory();
    return size;
  }
};

class Reference : public Value{
    int index_;
    Frame* frame_;
public:
    Reference() {}
    Reference(int index, Frame* frame) : index_(index), frame_(frame) {}
    std::string toString() {throw RuntimeException("Attempting to print reference");};
    Type getType() const { return Type::Reference; }
    Value* getValue();
    void setValue(Value* value);
    int getIndex() { return index_; }
    void follow(CollectedHeap& heap) override {
        heap.markSuccessors(frame_);
    }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  void initializeDynamicMemory(CollectedHeap* heap) override {}
  size_t getCurrentSize() override {return sizeof(*this);}
};

class Record : public Value {
  TrackingUnorderedMap<TrackingString, Value*> map_;
  TrackingSet<TrackingString> fields_;
public:
  Record() = default;
  std::string toString() {
    std::stringstream ss;
    ss << "{";
    for (const auto& field: fields_){
        ss << field << ":" << map_[field]->toString() << " ";
    }
    ss << "}";
    return ss.str();
  }
  Value* getValue(const std::string& field) { 
    TrackingString temp(field, map_.get_allocator());
    auto it = map_.find(temp);
    return (it != map_.end()) ? it->second : heap_->allocate<Constant::None>();
}
  void setValue(const std::string& field, Value* value) {
        TrackingString temp(field, map_.get_allocator());
        auto [it, inserted] = map_.emplace(temp, value);
        if (inserted) {
            fields_.insert(temp);
        } else {
            it->second = value;
        }
    }
  Type getType() const { return Type::Record; }
  void follow(CollectedHeap& heap) override {
    for (const auto& [field, value] : map_) {
      heap.markSuccessors(value);
    }
  }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  void initializeDynamicMemory(CollectedHeap* heap) override {
    TrackingAllocator<std::pair<const TrackingString, Value*>> allocator;
    allocator.setHeap(heap);
    map_ = TrackingUnorderedMap<TrackingString, Value*>(allocator);
    fields_ = TrackingSet<TrackingString>(allocator);
  }
  size_t getCurrentSize() override {
    size_t size = sizeof(*this);
    size += map_.get_allocator().getCurrentMemory();
    for (const auto& [field, value] : map_) {
      size += field.get_allocator().getCurrentMemory();
    }
    size += fields_.get_allocator().getCurrentMemory();
    for (const auto& field : fields_) {
      size += field.get_allocator().getCurrentMemory();
    }
    return size;
  }
};

class Closure : public Value {
  TrackingVector<Reference*> free_vars_;
  Function* function_;
  inline static const std::string name_ = "FUNCTION";
  int num_free_vars_;
public:
  std::string toString() {return name_;};
  Closure(int num_free_vars) : num_free_vars_(num_free_vars) {}
  int getNumFreeVars() { return free_vars_.size(); }
  void addFreeVar(int index, Reference* value) { free_vars_[index] = value; }
  Reference* getFreeVar(int index) { return free_vars_[index]; }
  TrackingVector<Reference*>& getFreeVars() { return free_vars_; }
  void setFunction(Function* function) { function_ = function; }
  Function* getFunction() { return function_; }
  Type getType() const { return Type::Closure; }
  void follow(CollectedHeap& heap) override {
    heap.markSuccessors(function_);
    for (Reference* ref : free_vars_) {
      heap.markSuccessors(ref);
    }
  }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  void initializeDynamicMemory(CollectedHeap* heap) override {
    TrackingAllocator<Reference*> allocator;
    allocator.setHeap(heap);
    free_vars_ = TrackingVector<Reference*>(allocator);
    free_vars_.resize(num_free_vars_);
  }
  size_t getCurrentSize() override {
    size_t size = sizeof(*this);
    size += free_vars_.get_allocator().getCurrentMemory();
    return size;
  }
};
