cmake_minimum_required(VERSION 3.20)
project(lld_fork_hang LANGUAGES CXX)

find_package(LLVM REQUIRED CONFIG)
find_package(LLD REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${LLD_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
link_directories(${LLD_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(lld_fork_hang lld_fork_hang.cpp)

target_link_libraries(lld_fork_hang PRIVATE
    lldCommon
    lldELF
    LLVMCore
    LLVMSupport
    pthread
)

set_target_properties(lld_fork_hang PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)
