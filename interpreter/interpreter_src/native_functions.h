#pragma once
#include "interpreter.h"

class NativeFunction: public Function{
public:
    enum FunctionName{PRINT, INPUT, INTCAST};
    FunctionName function;

    NativeFunction(FunctionName name_, std::vector<std::string> formal_arguments_):
        function(name_), Function(nullptr, formal_arguments_, nullptr){}

    void call(Interpreter& interpreter){
        Frame* frame = interpreter.stack.top();
        switch (function)
        {
        case PRINT:
            interpreter.print(frame->map[formal_arguments[0]]);
            break;
        case INPUT:
            interpreter.rval = interpreter.input();
            interpreter.return_flag = true;
            break;
        case INTCAST:
            interpreter.rval = interpreter.intcast(frame->map[formal_arguments[0]]);
            interpreter.return_flag = true;
            break;
        default:
            break;
        }
    }
    
};
