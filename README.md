## Simplified LLVM IR Modeling (SLIM)

The SLIM implementation takes a `.ll` file (which is the human readable form of LLVM-IR) as an input, processes it and populates the SLIM-IR data structures. There are various APIs available in order to manipulate the SLIM-IR, but this can be extended to include other APIs based on your requirements. It is easy to extend because the APIs in SLIM use the LLVM infrastructure and its documentation is easily available on the internet. Please raise a pull request if you have extended the implementation in any manner so that we can make the feature available for everyone.

#### Install the SLIM Library

To install the SLIM library, please follow these steps:

1. Create a build folder and change your working directory to this folder
```bash
mkdir build
cd build
```

2. Run the cmake command to generate the Makefile:
`cmake -S .. -B .`

3. Install the library by the command:
`sudo make install`

The library will be installed at the location `/usr/local/lib` and the header files will be stored at the location `/usr/local/include/slim`. These locations can be used in the parent CMakeFile of your LLVM project. A sample CMakeLists.txt file is shown below:

```c++
cmake_minimum_required(VERSION 3.4)
set (CMAKE_CXX_STANDARD 14)
project(cs-ssa)

set(ENV{LLVM_DIR} /usr/lib/llvm-14/lib/cmake/llvm)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

include_directories(/usr/local/include/slim)

add_library(slim SHARED IMPORTED)

set_target_properties(slim PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libslim.so")

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} all)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} slim ${llvm_libs})
```

#### Using SLIM

Before using SLIM, please ensure that your LLVM IR output (.ll) file is generated using the flags present in the below commands (otherwise the results may be incorrect):

```bash
lloutput() {
    clang-14 -fno-discard-value-names -emit-llvm -O0 -Xclang -disable-O0-optnone -g -S "$1"
    opt-14 -instnamer -mem2reg -aa-pipeline='basic-aa' -S "${1%%.*}.ll" > "output.ll"
}
```

You may also paste this at the end of `~/.bashrc` file so that the output can be generated using the following command:
`lloutput <file-name>`

Some basic info about the important flags in the above commands:
- `-fno-discard-value-names` is used to avoid the numeric (or temporary) naming of variables wherever possible and retain the original source program variable name. [[Reference]](https://stackoverflow.com/questions/50432967/how-to-save-the-variable-name-when-use-clang-to-generate-llvm-ir) 
- `-Xclang` and `-disable-O0-optnone` are used to prevent clang from adding the optnone attribute to each function. The optnone attribute when used with the optimization level `-O0` prevents further optimizations like mem2reg, which is required by our implementation. [[Reference]](https://stackoverflow.com/questions/46513801/llvm-opt-mem2reg-has-no-effect)

The SLIM IR can be generated by first parsing the `.ll` file and then calling the constructor with the module as an argument. Once the IR object is created, all the instructions can be processed and various APIs can be used in order to fetch the required information. A sample main.cpp file is shown which shows how the object is created and how the IR can be dumped (for now we are considering only the stdout as the possible output destination, it can be changed using the `1>` redirection but in future we will change the code to dump the IR into any file without the need of any redirection):

```c++
// Some headers

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2)
    {
        llvm::errs() << "We expect exactly one argument - the name of the LLVM IR file!\n";
        exit(1);
    }

    llvm::SMDiagnostic smDiagnostic;

    std::unique_ptr<llvm::Module> module = parseIRFile(argv[1], smDiagnostic, context);

    slim::IR *transformIR = new slim::IR(module);

    transformIR->dumpIR();

    return 0;
}

```

Since this is the first beta version of SLIM, it is quite possible that the implementation has some bug(s). Please feel free to raise a pull request or send a mail to pradhanaditya@cse.iitb.ac.in in case of any issue.