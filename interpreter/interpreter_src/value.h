#pragma once
#include "debug.h"
#include <map>
#include <vector>
#include <sstream>
#include "AST.h"
class Frame;

class Value{
public:
    virtual std::string to_string() = 0;
};

class Bool: public Value{
public:
    bool val;
    Bool(bool val_):val(val_){}
    std::string to_string(){
        std::string str = (val ? "true" : "false");
        return str;
    }
};
class Integer: public Value{
public:
    int val;
    Integer(int val_):val(val_){}
    std::string to_string(){
        std::string str = std::to_string(val);
        return str;
    }

};
class String: public Value{
public:
    std::string val;
    String(std::string val_):val(val_){}
    std::string to_string(){
        return "\"" + val + "\"" ;
    }
};
class Record: public Value{
public:
    std::map<std::string, Value*> map;
    // could be nullptr
    Record(){}

    std::string to_string(){
        DEBUG_PRINT("Record");
        std::stringstream ss;
        ss << "{";
        for (const auto& pair: map){
            ss << pair.first << ":" << pair.second->to_string() << " ";
        }
        ss << "}";
        return ss.str();
    }

};
class Function: public Value{
public:
    Frame* function_frame;
    std::vector<std::string> formal_arguments;
    AST::Block* body;
    Function(Frame* function_frame_, std::vector<std::string> formal_arguments_, AST::Block* body_ ):
        function_frame(function_frame_), formal_arguments(formal_arguments_), body(body_){}

    std::string to_string(){
        std::stringstream ss;
        ss << "fun(";
        for (const auto& arg:formal_arguments){
            ss << arg << ", ";
        }
        if(formal_arguments.size() > 0){
            ss << "){}";    
        }
        else{
            ss << "){}";    
        }
        return ss.str();
    }
};

class None: public Value{
public:
    std::string to_string(){
        return "None";
    }
};
