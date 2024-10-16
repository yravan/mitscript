#include "interpreter.h"
#include <sstream>

std::string Interpreter::string_cast(Value* v) {
    if (typeid(*v) == typeid(String)) {
        String* val = dynamic_cast<String*>(v);
        return val->val;  // If it's already a String, cast and return
    }
    if (typeid(*v) == typeid(Bool)) {
        return v->to_string();  // Cast Bool to String
    }
    if (typeid(*v) == typeid(Integer)) {
        return v->to_string();  // Cast Integer to String
    }
    if (typeid(*v) == typeid(None)) {
        return "None";  // None case returns a string "None"
    }
    if (typeid(*v) == typeid(Function)) {
        return "FUNCTION";  // Placeholder for Function type
    }
    if (typeid(*v) == typeid(Record)) {
        std::stringstream ss;
        ss << "{";
        Record* val = dynamic_cast<Record*>(v);
        for (const auto& pair: val->map){
            ss << pair.first << ":" << string_cast(pair.second) << " ";
        }
        ss << "}";
        return ss.str();
    }
    throw std::runtime_error("Unsupported type for string cast");
}
