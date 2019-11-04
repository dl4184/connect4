cmake_minimum_required(VERSION 3.6)
project(connect4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

ADD_LIBRARY(c4solver_dependencies STATIC
        solver.cpp
        solver.hpp
        position.hpp
        TranspositionTable.hpp
        MoveSorter.hpp)

ADD_EXECUTABLE(engine engine.cpp)
ADD_EXECUTABLE(best_move best_move.cpp)

TARGET_LINK_LIBRARIES(engine c4solver_dependencies)
TARGET_LINK_LIBRARIES(best_move c4solver_dependencies)

INSTALL(TARGETS c4solver_dependencies DESTINATION lib)

INSTALL(TARGETS engine DESTINATION bin)
INSTALL(TARGETS best_move DESTINATION bin)