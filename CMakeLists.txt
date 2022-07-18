include_directories(include)

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} all)

add_library(slim SHARED
    IR.cpp
    Instructions.cpp
)

target_link_libraries(slim ${llvm_libs})

# set_target_properties(slim PROPERTIES
#     COMPILE_FLAGS "-g -std=c++14 -fno-rtti -fPIC"
# )

install(TARGETS slim LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY
    include/ DESTINATION /usr/local/include/slim)