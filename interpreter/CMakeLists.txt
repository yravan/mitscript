add_compile_options(-Wno-attributes)
include_directories(.)

# generate the grammar cpp file
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_LIST_DIR}/MITScript.cpp
        COMMAND antlr4 -Dlanguage=Cpp MITScript.g
        COMMAND sed -i 's/u8//g' MITScript.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DEPENDS MITScript.g
)

# Grammar library
add_library(MITScriptGrammar MITScript.cpp)
target_link_libraries(MITScriptGrammar ${CMAKE_SYSROOT}/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/libantlr4-runtime.a)
target_include_directories(MITScriptGrammar PUBLIC /usr/include/antlr4-runtime)

# Parser sources
add_library(ParserSources OBJECT
parser_src/CST.cpp
parser_src/Arithmetic.cpp
parser_src/Boolean.cpp
parser_src/ControlFlow.cpp
parser_src/Expressions.cpp
parser_src/TreeConverter.cpp
AST.cpp
        )
target_link_libraries(ParserSources MITScriptGrammar)

# Parser targets
add_executable(mitscript-parser parser.cpp)
target_link_libraries(mitscript-parser ParserSources)

# add_executable(mitscript-parse-tree parse-tree.cpp)
# target_link_libraries(mitscript-parse-tree MITScriptGrammar)

add_executable(mitscript-parser-unique-ptr parser-unique-ptr.cpp)
target_link_libraries(mitscript-parser-unique-ptr MITScriptGrammar)


# Interpreter sources
add_library(InterpreterSources OBJECT
parser_src/empty.cpp
interpreter_src/expression.cpp
interpreter_src/statement.cpp
interpreter_src/cast.cpp
interpreter_src/native_functions.cpp
interpreter_src/data_functions.cpp
        )

add_executable(mitscript interpreter.cpp)
target_link_libraries(mitscript InterpreterSources ParserSources)
