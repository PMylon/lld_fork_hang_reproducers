cmake_minimum_required(VERSION 3.20)
project(ForkParallelHang LANGUAGES CXX)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(fork_parallel_hang fork_parallel_hang.cpp)

llvm_map_components_to_libnames(llvm_libs support)

target_link_libraries(fork_parallel_hang PRIVATE
    ${llvm_libs}
    pthread
)

set_target_properties(fork_parallel_hang PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)
