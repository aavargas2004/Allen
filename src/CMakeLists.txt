include_directories(../include)
add_executable(../allcomp AST.cpp Exceptions.cpp main.cpp Types.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
