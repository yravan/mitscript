#pragma once
#include "types.h"
#include <iostream>

class NativeFunction : public Function {
protected:
    Frame* frame_;

public:
    NativeFunction() {}
    virtual ~NativeFunction() {}
    std::string toString() { return "NATIVE_FUNCTION"; }
    void setFrame(Frame* frame) { frame_ = frame; }
    virtual void execute() = 0;
};

class printFunction : public NativeFunction { 

public:
    printFunction() {local_vars_.push_back("x"); parameter_count_ = 1;}
    void execute() override {
        std::cout << frame_->getLocalVar(0)->toString() << std::endl;
        frame_->push(new Constant::None());
    }
};

class inputFunction : public NativeFunction { 

public:
    inputFunction() {}
    void execute() override {
        std::string input;
        std::cin >> input;
        frame_->push(new Constant::String(input));
    }
};

class intcastFunction : public NativeFunction {

public:
    intcastFunction() {local_vars_.push_back("x"); parameter_count_ = 1;} 
    void execute() override {
        Value* value = frame_->getLocalVar(0);
        if (!dynamic_cast<Constant::String*>(value)) {
            throw IllegalCastException();
        }
        Constant::String* str = static_cast<Constant::String*>(value);
        int num = atoi((str->getValue()).c_str());
        if (num == 0 && str->getValue() != "0"){
            throw IllegalCastException();
        }
        frame_->push(new Constant::Integer(num));
    }
};



