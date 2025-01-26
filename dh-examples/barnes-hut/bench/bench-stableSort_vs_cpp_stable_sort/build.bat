@echo off
REM Compile all dasae-headers C files to .o
clang -std=c17 -IC:/dasae-storage/dev/source/c-cpp-workspace/projects/dasae-src/dasae-headers/dh -IC:/dasae-storage/dev/source/c-cpp-workspace/projects/dasae-src/dasae-headers/dh/include -DCOMP -DNDEBUG -DMEM_NO_TRACE_ALLOC_AND_FREE -Wall -Wextra -O3 -c C:\dasae-storage\dev\source\c-cpp-workspace\projects\dasae-src\dasae-headers\dh\src/*.c -static

REM Create dasae-headers static library
llvm-ar rcs libdh.a *.o

REM Compile C++ files to .o
clang++ -std=c++17 -DCOMP -DNDEBUG -DMEM_NO_TRACE_ALLOC_AND_FREE -O3 -c benchmark_cpp_stable_sort.cpp -o benchmark_cpp_stable_sort.o -static

REM Compile main C file to .o
clang -std=c17 -IC:/dasae-storage/dev/source/c-cpp-workspace/projects/dasae-src/dasae-headers/dh -IC:/dasae-storage/dev/source/c-cpp-workspace/projects/dasae-src/dasae-headers/dh/include -DCOMP -DNDEBUG -DMEM_NO_TRACE_ALLOC_AND_FREE -Wall -Wextra -O3 -c main.c -o main.o -static

REM Build main program
clang -std=c17 -IC:/dasae-storage/dev/source/c-cpp-workspace/projects/dasae-src/dasae-headers/dh -IC:/dasae-storage/dev/source/c-cpp-workspace/projects/dasae-src/dasae-headers/dh/include -DCOMP -DNDEBUG -DMEM_NO_TRACE_ALLOC_AND_FREE -O3 main.o benchmark_cpp_stable_sort.o -o main.exe -static -L. -ldh -lstdc++

REM Cleanup
del *.o
