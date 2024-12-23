#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include "gc.h"
#include "bytecode/types.h"
#include "bytecode/instructions.h"
#include "bytecode/exceptions.h"
#include "bytecode/native_functions.h"



class Interpreter {
private:

    Frame* global_frame_;    
    TrackingVector<Frame*> stack_frames_;
    int instruction_pointer_ = 0;
    Function* current_function_;
    TrackingUnorderedMap<std::string, int> global_indices_;
    TrackingUnorderedSet<Function*> native_functions_;
    CollectedHeap* heap_;
    int max_memory_bytes_ = 4*MEGABYTE_TO_BYTE;

    Constant::None* none_;
    Constant::Boolean* true_;
    Constant::Boolean* false_;


public:
    Interpreter() {}
    Interpreter(CollectedHeap* heap) : heap_(heap) {}

    void setMemoryLimit(int max_memory_bytes) {
        max_memory_bytes_ = max_memory_bytes;
    }

    inline void pushOntoStack(Value* value);
    inline Value* popFromStack();

    inline std::string stringCast(Value* value);

    template <typename T>
    void validateValueType(Value* value);

    void executeProgram(Function* program);
    void initializeNativeFunctions();
    void executeFunction(Function* function);
    void executeInstruction();
    void garbageCollect();

    void loadConst(int index);
    void loadFunc(int index);
    void loadLocal(int index);
    void storeLocal(int index);
    void loadGlobal(int index);
    void storeGlobal(int index);

    void pushReference(int index);
    void loadReference();
    void storeReference();

    void allocRecord();
    void fieldLoad(int index);
    void fieldStore(int index);
    void indexLoad();
    void indexStore();

    void allocClosure(int num_free_vars);
    void call(int num_args); 
    void returnStatement();

    void add();
    void sub();
    void mul();
    void div();
    void neg();

    void gt();
    void geq();
    void eq();

    void andStatement();
    void orStatement();
    void notStatement();

    void gotoStatement(int offset);
    void ifStatement(int offset);

    void dup();
    void swap();
    inline void pop();


  class LRUCache {
    class Node {
    public:
        Constant::Integer* value = nullptr;
        int key = 0;
        Node* prev = nullptr;
        Node* next = nullptr;
    };
    std::unordered_map<int, Node*> cache_;
    int capacity_;
    Node* head_;
    Node* tail_;
  public:
    LRUCache(): capacity_(10) {
        head_ = new Node();
        tail_ = new Node();
        tail_->prev = head_;
        head_->next = tail_;
        cache_.reserve(capacity_);
    }
    LRUCache(int capacity) : capacity_(capacity) {
        head_ = new Node();
        tail_ = new Node();
        tail_->prev = head_;
        head_->next = tail_;
        cache_.reserve(capacity_);
    }

    Constant::Integer* get(int key, CollectedHeap* heap_) {
        if (cache_.find(key) != cache_.end()) {
            Node* node = cache_[key];
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->next = head_->next;
            node->next->prev = node;
            head_->next = node;
            node->prev = head_;
            return node->value;
        }
        Constant::Integer* value = heap_->allocate<Constant::Integer>(key);
        if (capacity_ == cache_.size()) {
          cache_.erase(tail_->prev->key);
          Node* node = tail_->prev;
          node->prev->next = tail_;
          tail_->prev = node->prev;
          node->next = head_->next;
          node->next->prev = node;
          head_->next = node;
          node->prev = head_;
          node->value = value;
          node->key = key;
          cache_[key] = node;
        } else {
          Node* node = new Node();
          node->value = value;
          node->key = key;
          node->next = head_->next;
          node->next->prev = node;
          head_->next = node;
          node->prev = head_;
          cache_[key] = node;
        }
        return value;
    }
    void mark() {
        for (auto& [key, node] : cache_) {
            node->value->marked_=true;
        }
    }
};
private:
  LRUCache integer_cache_;

};




