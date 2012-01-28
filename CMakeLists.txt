cmake_minimum_required (VERSION 2.8.7)
project ("ebc")

set (CMAKE_CXX_FLAGS "-std=c++0x -ggdb3 -Wall -Wextra -pedantic")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

# Do not optimize LLVM IR
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_OPT")

###############
# Target test #
###############

add_executable (
    test
    "src/AST.cpp"
    "src/helpers.cpp"
    "src/parse.cpp"
    "src/test.cpp"
    "src/Tokenizer.cpp"
    "src/Visitor.cpp"
)

##############
# Target ebc #
##############

find_package (LLVM)

if (NOT LLVM_FOUND)
    message (FATAL_ERROR "LLVM cannot be found")
endif ()

include_directories (${LLVM_INCLUDE_DIRS})
message ("LLVM lib dir : ${LLVM_LIBRARY_DIRS}")
link_directories (${LLVM_LIBRARY_DIRS})
add_definitions (${LLVM_DEFINITIONS})

add_executable (
    ebc
    "src/AST.cpp"
    "src/ebc.cpp"
    "src/compile_ir.cpp"
    "src/helpers.cpp"
    "src/LLVM_Visitor.cpp"
    "src/parse.cpp"
    "src/Tokenizer.cpp"
    "src/Visitor.cpp"
)

llvm_map_components_to_libraries (REQ_LLVM_LIBRARIES native ipo jit)
target_link_libraries (ebc ${REQ_LLVM_LIBRARIES})
