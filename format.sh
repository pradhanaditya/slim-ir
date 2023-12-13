#!/bin/sh

CLANG_FORMAT=clang-format-14

## This script is used to format the source code of the project.
find ./ -iname "*.h" -o -iname "*.hpp" -o -iname "*.cc" -o -iname "*.cpp" -not -iwholename "./build/*.cpp" | xargs $CLANG_FORMAT -style=LLVM -i
