#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include <assert.h>
#include "gc.h"
// #include "bytecode/frame.h"
#include "bytecode/types.h"
#include "bytecode/instructions.h"
#include "bytecode/exceptions.h"
#include "bytecode/native_functions.h"

class Interpreter {
private:

    Frame* global_frame_;    
    TrackingVector<Frame*> stack_frames_;
    int instruction_pointer_;
    Function* current_function_;
    TrackingUnorderedMap<std::string, int> global_indices_;
    TrackingUnorderedSet<Function*> native_functions_;
    CollectedHeap* heap_;
    int max_memory_bytes_;

    Constant::None* none_;
    Constant::Boolean* true_;
    Constant::Boolean* false_;


public:
    Interpreter() {assert(false);}
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


};




