#pragma once
#include "debug.h"
#include <string>
#include <exception>

class Value;
class InterpreterException: public std::exception {
public:
    virtual std::string message() = 0;
};

// UninitializedVariableException: occurs when a variable is not found in any stack frame
class UninitializedVariableException : public InterpreterException {
    std::string variable;
public:
    UninitializedVariableException(const std::string& var) : variable(var) {}

    std::string message() override {
        return "UninitializedVariableException: " + variable;
    }
};

// IllegalCastException: occurs when an invalid operation is applied to a value
class IllegalCastException : public InterpreterException {
    std::string operation;
    Value* val;
public:
    IllegalCastException() {}
    IllegalCastException(const std::string& op, Value * v) : operation(op), val(v) {}

    std::string message() override {
        return "IllegalCastException";
    }
};

// IllegalArithmeticException: occurs when division by zero is attempted
class IllegalArithmeticException : public InterpreterException {
public:
    std::string message() override {
        return "IllegalArithmeticException: divide by zero";
    }
};


// RuntimeException: generic runtime exception for illegal operations
class RuntimeException : public InterpreterException {
    std::string error;
    bool argument_count_mismatch;
    int actual_arguments = -1;
    int expected_arguments = -1;
public:
    RuntimeException(const std::string& err) : error(err) {}
    RuntimeException(int actual, int expected) : actual_arguments(actual), expected_arguments(expected) {}

    std::string message() {
        if (actual_arguments >= 0){
            return "RuntimeException: argument count mismatch (" + std::to_string(actual_arguments) + " instead of " + std::to_string(expected_arguments) + ")";
        }
        return "RuntimeException: " + error;
    }
};

class InsufficientStackException : public InterpreterException {
public:
    std::string message() override {
        return "InsufficientStackException: stack underflow";
    }
};
