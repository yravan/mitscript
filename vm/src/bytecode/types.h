#pragma once
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
  None() = default;
  virtual ~None() {}
  std::string toString() { return name_; }
  Type getType() const { return Type::None; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  size_t getCurrentSize() override {return sizeof(*this);}
  void copy(CollectedHeap* heap) override {
    heap->addObject(this);
  }
};

class Constant::Integer : public Constant {
  int value_;
  std::string name_;
public:
  Integer(int value) : value_(value) {name_ = std::to_string(value);}
  virtual ~Integer() {}
  int getValue() const { return value_; }
  std::string toString() { return name_; }
  Type getType() const { return Type::Integer; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this) + name_.capacity(); }
  size_t getCurrentSize() override {return sizeof(*this) + name_.capacity();}
  void copy(CollectedHeap* heap) override {
    heap->addObject(this);
  }
};

class Constant::String : public Constant {
  std::string value_;
public:
  String(std::string value) : value_(std::move(value)) {}
  const std::string& getValue() const { return value_; }
  virtual ~String() {}
  std::string toString() { return value_; }
  Type getType() const { return Type::String; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this) + value_.capacity(); }
  size_t getCurrentSize() override {return sizeof(*this) + value_.capacity(); }
  void copy(CollectedHeap* heap) override {
    heap->addObject(this);
  }
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
  Type getType() const { return Type::Boolean; }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this); }
  size_t getCurrentSize() override {return sizeof(*this);}
  void copy(CollectedHeap* heap) override {
    heap->addObject(this);
  }
};

class Function : public Value {
  inline static std::string name_ = "FUNCTION";

public:
  std::vector<Function*> functions_;
  std::vector<Constant*> constants_;
  int parameter_count_ = 0;
  std::vector<std::string> local_vars_;
  std::vector<std::string> local_reference_vars_;
  std::vector<std::string> free_vars_;
  std::vector<std::string> names_;
  InstructionList instructions;

  Function() = default;
  Function(std::vector<Function*> functions,
           std::vector<Constant*> constants,
           int parameter_count,
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
  void copy(CollectedHeap* heap) override {
    // #TODO Implement
  }
};

class Reference;
class Frame : public Collectable {
  // The operand stack
  TrackingVector<Value*> stack_;

  // The local variables
  Value** local_vars_;
  uint16_t num_local_vars_;

  // The local reference variables
  Reference** local_reference_vars_;
  uint16_t num_local_reference_vars_;
  uint16_t num_free_vars_;

public: 
  Frame() {
    // stack_.reserve(10);
  } 
  Frame(int num_local_vars, int num_local_reference_vars, int num_free_vars) : num_local_vars_(num_local_vars), num_local_reference_vars_(num_local_reference_vars), num_free_vars_(num_free_vars) {
    local_vars_ = new Value*[num_local_vars];
    local_reference_vars_ = new Reference*[num_local_reference_vars + num_free_vars];
  }
  ~Frame() {
    delete[] local_vars_;
    delete[] local_reference_vars_;
  }
  inline std::string toString() { return "FRAME"; }
  std::string dump();
  void push(Value* value);
  Value* pop();
  void setNumLocalVars(int num_vars) { local_vars_ = new Value*[num_vars]; }
  Value* getLocalVar(int index);
  void setLocalVar(int index, Value* value);
  void makeLocalReferences(const std::vector<int>& local_reference_vars);
  void addFreeVariables(Reference** free_vars, int num_vars);
  Reference* getReference(int index);

  void follow(CollectedHeap& heap) override;
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this) + num_local_vars_ * sizeof(Value*) + num_local_reference_vars_ * sizeof(Reference*); }
  void initializeDynamicMemory(CollectedHeap* heap) override {
    TrackingAllocator<Value*> allocator;
    allocator.setHeap(heap);
    stack_ = TrackingVector<Value*>(allocator);
    stack_.reserve(10);
  }
  size_t getCurrentSize() override {
    size_t size = base_size_bytes_;
    size += stack_.get_allocator().getCurrentMemory();
    return size;
  }
  void copy(CollectedHeap* heap) override {
    TrackingAllocator<Value*> allocator;
    allocator.setHeap(heap);
    TrackingVector<Value*> new_stack(allocator);
    new_stack.reserve(stack_.size());
    for (Value* value : stack_) {
      new_stack.emplace_back(value);
    }
    std::swap(stack_, new_stack);
    heap->addObject(this);
  }
};

class Reference : public Value{
    uint16_t index_;
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
  void copy(CollectedHeap* heap) override {
    heap->addObject(this);
  }
};

class Record : public Value {
  TrackingUnorderedMap<std::string, Value*> map_;
  static Constant::None none_;
  int dynamic_string_memory_bytes_;
public:
  Record() = default;
  ~Record() {heap_->addMemory(-dynamic_string_memory_bytes_);}
  std::string toString() {
    std::stringstream ss;
    ss << "{";
    std::vector<std::string> temp;
    temp.reserve(map_.size());
    for (const auto& kv : map_) {
        temp.emplace_back(kv.first);
    }
    std::sort(temp.begin(), temp.end());
    for (const auto& key : temp) {
        ss << key << ":" << map_[key]->toString() << " ";
    }
    // Copy elements to std::map
    // std::map<std::string, Value*> sortedMap(map_.begin(), map_.end());

    // // Print in lexical order
    // for (const auto& [key, value] : sortedMap) {
    //     ss << key << ":" << value->toString() << " ";
    // }
    ss << "}";
    return ss.str();
  }
  Value* getValue(const std::string& field) { 
    auto it = map_.find(field);
    return (it != map_.end()) ? it->second : &none_;
}
  void setValue(const std::string& field, Value* value) {
        auto [it, inserted] = map_.emplace(field, value);
        if (inserted) {
            heap_->addMemory(field.capacity());
            dynamic_string_memory_bytes_ += field.capacity();
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
    TrackingAllocator<std::pair<const std::string, Value*>> allocator;
    allocator.setHeap(heap);
    map_ = TrackingUnorderedMap<std::string, Value*>(allocator);
  }
  size_t getCurrentSize() override {
    size_t size = sizeof(*this);
    size += map_.get_allocator().getCurrentMemory();
    size += dynamic_string_memory_bytes_;
    return size;
  }
  void copy(CollectedHeap* heap) override {
    TrackingAllocator<std::pair<const std::string, Value*>> allocator;
    allocator.setHeap(heap);
    TrackingUnorderedMap<std::string, Value*> new_map(allocator);
    new_map.reserve(map_.size());
    for (const auto& [field, value] : map_) {
      new_map.emplace(field, value);
    }
    std::swap(map_, new_map);
    this->heap_->addMemory(-dynamic_string_memory_bytes_);
    heap->addObject(this);
  }
};

class Closure : public Value {
  Reference** free_vars_; // Array of references to free variables
  Function* function_;
  inline static const std::string name_ = "FUNCTION";
  uint16_t num_free_vars_;
public:
  ~Closure() { delete[] free_vars_; }
  std::string toString() {return name_;};
  Closure(int num_free_vars) : num_free_vars_(num_free_vars) { free_vars_ = new Reference*[num_free_vars]; }
  int getNumFreeVars() { return num_free_vars_; }
  void addFreeVar(int index, Reference* value) { free_vars_[index] = value; }
  Reference* getFreeVar(int index) { return free_vars_[index]; }
  Reference** getFreeVars() { return free_vars_; }
  void setFunction(Function* function) { function_ = function; }
  Function* getFunction() { return function_; }
  Type getType() const { return Type::Closure; }
  void follow(CollectedHeap& heap) override {
    heap.markSuccessors(function_);
    for (int i = 0; i < num_free_vars_; i++) {
      heap.markSuccessors(free_vars_[i]);
    }
  }
  void calculateBaseSizeBytes() override { base_size_bytes_ = sizeof(*this) + num_free_vars_ * sizeof(Reference*); }
  void initializeDynamicMemory(CollectedHeap* heap) override {}
  size_t getCurrentSize() override {
    return base_size_bytes_;
  }
  void copy(CollectedHeap* heap) override {
    heap->addObject(this);
  }
};
