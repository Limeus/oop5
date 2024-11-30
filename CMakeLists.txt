cmake_minimum_required(VERSION 3.10)
project(lab5)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(include)

add_executable(lab5_exe main.cpp src/MemoryResource.cpp)

enable_testing()

add_executable(test5 test/tests.cpp src/MemoryResource.cpp)
target_link_libraries(test5 gtest_main)

add_test(NAME lab5_test COMMAND test5)