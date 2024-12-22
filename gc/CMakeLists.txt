include_directories(. ../vm/src/bytecode)

add_executable(gctest main.cpp)
target_link_libraries(gctest CompilerSources BytecodeSources BC_Parser ParserSources)


