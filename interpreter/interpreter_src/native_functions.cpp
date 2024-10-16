#include "native_functions.h"
#include <iostream>

void Interpreter::create_native_functions(){
    Frame* program_frame = stack.top();
    program_frame->map["print"] = new NativeFunction(NativeFunction::PRINT, {"s"});
    program_frame->map["intcast"] = new NativeFunction(NativeFunction::INTCAST, {"s"});
    program_frame->map["input"] = new NativeFunction(NativeFunction::INPUT, {});
}


void Interpreter::print(std::string variable){
    print(lookup_read(stack.top(), variable));
}
void Interpreter::print(Value* value){
    DEBUG_PRINT("Entering print(" + value->to_string() + ")");
    std::string value_str = string_cast(value);
    std::cout << value_str << std::endl;
    DEBUG_PRINT("Exiting print(" + value->to_string() + ")");
}
String* Interpreter::input(){
    std::string line;
    std::getline(std::cin, line);
    return new String(line);
}
Integer* Interpreter::intcast(std::string variable){
    intcast(lookup_read(stack.top(), variable));
}
Integer* Interpreter::intcast(Value* value) {
    DEBUG_PRINT("Entering intcast(" + value->to_string() + ")");
    if (dynamic_cast<Integer*>(value)) {
        DEBUG_PRINT("Exiting intcast(" + value->to_string() + ")");
        return dynamic_cast<Integer*>(value); // Return the casted Integer value
    }

    if (dynamic_cast<Bool*>(value)) {
        Bool* boolVal = dynamic_cast<Bool*>(value);
        DEBUG_PRINT("Exiting intcast(" + value->to_string() + ")");
        return new Integer(boolVal->val ? 1 : 0); // Convert Bool to Integer
    }

    // Check if the value is of type String
    if (dynamic_cast<String*>(value)) {
        String* stringVal = dynamic_cast<String*>(value);
        int num = atoi((stringVal->val).c_str());
        if (num == 0 && stringVal->val != "0"){
            throw IllegalCastException("intcast", value);
        }
        DEBUG_PRINT("Exiting intcast(" + value->to_string() + ")");
        return new Integer(num);
    }

    // Check if the value is of type Function
    if (dynamic_cast<Function*>(value)) {
        // Functions cannot be cast to Integer
        DEBUG_PRINT("Exiting intcast(" + value->to_string() + ")");
        throw IllegalCastException("intcast", value);
    }

    // Check if the value is of type None
    if (dynamic_cast<None*>(value)) {
        // Optionally return an Integer representation of None, such as 0
        DEBUG_PRINT("Exiting intcast(" + value->to_string() + ")");
        return new Integer(0); // Treat None as 0
    }
    assert(0);
}
