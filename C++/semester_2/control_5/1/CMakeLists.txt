cmake_minimum_required(VERSION 2.8)
project(cw2_1_tests)

if(MSVC)
    add_definitions(/W4 /WX)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")
endif()
add_definitions(-DTASK1)
add_definitions(-DVARIADIC_TESTS)
#add_definitions(-DTASK2)
#add_definitions(-DTASK3)

file(GLOB SRC_LIST "./src/*.cpp" "./src/*.h*")

add_executable(${PROJECT_NAME} ${SRC_LIST})
