#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include "bytecode/frame.h"
#include "bytecode/types.h"
#include "bytecode/instructions.h"
#include "bytecode/exceptions.h"
#include "bytecode/native_functions.h"

class Interpreter {
private:

    Frame* current_frame_;
    int instruction_pointer_;
    Function* current_function_;
    std::unordered_map<std::string, Value *> global_vars_;
    std::unordered_set<Function*> native_functions_;


public:

    inline void pushOntoStack(Value* value);
    inline Value* popFromStack();

    inline std::string stringCast(Value* value);

    template <typename T>
    void validateValueType(Value* value);

    void executeProgram(Function* program);
    void executeFunction(Function* function);
    void executeInstruction();

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




