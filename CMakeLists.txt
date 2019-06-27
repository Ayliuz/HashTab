cmake_minimum_required(VERSION 3.13)

project(HashTab)


set(CMAKE_CXX_STANDARD 11)
set(Map PtrList.cpp stack.cpp map.cpp)
SET(MYCOMPILE_FLAGS "-pg")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MYCOMPILE_FLAGS}")
add_library(list STATIC ${Map})


add_executable(HashTab main.cpp)
target_link_libraries(HashTab list)