cmake_minimum_required(VERSION 3.16)
PROJECT(slim)

set(ENV{LLVM_DIR} /usr/lib/llvm-14/lib/cmake/llvm)

include_directories(include)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} all)

add_library(slim SHARED
    IR.cpp
    Instructions.cpp
    Operand.cpp
)

target_link_libraries(slim ${llvm_libs})

# set_target_properties(slim PROPERTIES
#     COMPILE_FLAGS "-g -std=c++14 -fno-rtti -fPIC"
# )

install(TARGETS slim LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY
    include/ DESTINATION /usr/local/include/slim)
