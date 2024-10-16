#pragma once
#include <iostream>
#include "debug.h"
#include "value.h"
#include <set>
#include <map>
#include <sstream>

class Frame{
public:
    std::map<std::string, Value*> map;
    Frame* parent_frame;
    Frame* global_frame;
    std::set<std::string> global_variables;

    Frame(Frame* parent_frame_,Frame* global_frame_):parent_frame(parent_frame_), global_frame(global_frame_){
        // TODO
    }

    Frame():parent_frame(nullptr){
        global_frame = this;
    }

    std::string to_string(){
        std::stringstream ss;
        ss << "{" << std::endl;
        for (const auto& pair : map){
            ss << "\t" << pair.first << ":" << pair.second->to_string() << std::endl;
        }
        ss << "  Global:" << std::endl;
        for (const auto& var : global_variables){
            std::cout << "\t" << var << ":" ;
            // std::cout << global_frame->map[var]->to_string() << std::endl;
        }
        ss << "}" << std::endl;
        return ss.str();
    }
};
