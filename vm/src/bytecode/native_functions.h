#pragma once
#include "types.h"
// #include "frame.h"
#include <iostream>

class NativeFunction : public Function {
protected:
    Frame* frame_;
    Constant::None* none_;

public:
    NativeFunction() {}
    void setNone(Constant::None* none) { none_ = none; }
    virtual ~NativeFunction() {}
    void setFrame(Frame* frame) { frame_ = frame; }
    virtual Value* execute() = 0;
    void setHeap(CollectedHeap* heap) { heap_ = heap; }
};

class printFunction : public NativeFunction { 
  Constant::None* none_;
public:
    printFunction() {local_vars_.push_back("x"); parameter_count_ = 1;}
    void setNone(Constant::None* none) { none_ = none; }
    void execute() override {
        std::cout << frame_->getLocalVar(0)->toString() << std::endl;
        frame_->push(none_);
    }
};

class inputFunction : public NativeFunction { 

public:
    inputFunction() {}
    Value* execute() override {
        std::string input;
        std::cin >> input;
        return heap_->allocate<Constant::String>(input);
    }
};

class intcastFunction : public NativeFunction {

public:
    intcastFunction() {local_vars_.push_back("x"); parameter_count_ = 1;} 
    Value* execute() override {
        Value* value = frame_->getLocalVar(0);
        if (!(value->getType() == Value::Type::String)){
            throw IllegalCastException();
        }
        Constant::String* str = static_cast<Constant::String*>(value);
        int num = atoi((str->getValue()).c_str());
        if (num == 0 && str->getValue() != "0"){
            throw IllegalCastException();
        }
        return heap_->allocate<Constant::Integer>(num);
    }
};



