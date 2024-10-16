#pragma once
#include "debug.h"
#include <string>
#include <exception>

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
        return "UninitializedVariableException: " + variable + " is not present in any appropriate stack frame.";
    }
};

// IllegalCastException: occurs when an invalid operation is applied to a value
class IllegalCastException : public InterpreterException {
    std::string operation;
    Value* val;
public:
    IllegalCastException(const std::string& op, Value * v) : operation(op), val(v) {}

    std::string message() override {
        return "IllegalCastException: cannot apply operation '" + operation + "' to '" + val->to_string() + "'.";
    }
};

// IllegalArithmeticException: occurs when division by zero is attempted
class IllegalArithmeticException : public InterpreterException {
public:
    std::string message() override {
        return "IllegalArithmeticException: divide by zero.";
    }
};

// RuntimeException: argument count mismatch
class ArgumentCountMismatchException : public InterpreterException {
    int expected;
    int actual;
public:
    ArgumentCountMismatchException(int expectedArgs, int actualArgs) : expected(expectedArgs), actual(actualArgs) {}

    std::string message() override {
        return "RuntimeException: argument count mismatch (" + std::to_string(actual) + " instead of " + std::to_string(expected) + ").";
    }
};

// RuntimeException: generic runtime exception for illegal operations
class RuntimeException : public InterpreterException {
    std::string error;
public:
    RuntimeException(const std::string& err) : error(err) {}

    std::string message() override {
        return "RuntimeException: " + error;
    }
};
