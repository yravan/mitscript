include_directories(. src bc-parser src/bytecode src/compiler ../interpreter)

add_subdirectory(bc-parser)

add_library(CompilerSources OBJECT
        src/compiler/compiler.cpp
        src/bytecode/types.cpp
        )

add_library(BytecodeSources OBJECT
        src/interpreter.cpp # TODO replace with your files
        src/bytecode/frame.cpp
        )

add_executable(mitscriptc src/bytecode-compiler.cpp)
target_link_libraries(mitscriptc CompilerSources ParserSources BC_Parser)

# If you call this mitscript instead, make sure you rename/comment out your interpreter executable
add_executable(mitscript src/bytecode-interpreter.cpp)

# add InterpreterSources or anything you need from the interpreter directory
target_link_libraries(mitscript CompilerSources BytecodeSources ParserSources BC_Parser)

