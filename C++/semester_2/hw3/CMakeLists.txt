cmake_minimum_required(VERSION 2.8)
project(ha2_3_tests)

if(MSVC)
    add_definitions(/W4 /WX)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -pedantic -pthread -O0 -ggdb")
endif()

file(GLOB SRC_LIST "./src/*.cpp" "./src/*.h*")

add_executable(${PROJECT_NAME} ${SRC_LIST})
