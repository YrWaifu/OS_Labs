cmake_minimum_required(VERSION 3.10)

add_compile_options(-pedantic -Wall)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lab4)

add_library(GCFLib SHARED GCFLib/GCF.cpp)
add_library(GCFLib::lib1 ALIAS GCFLib)
target_include_directories(GCFLib PUBLIC ${PROJECT_SOURCE_DIR}/GCFLib)

add_library(SquareLib SHARED SquareLib/Square.cpp)
add_library(SquareLib::lib2 ALIAS SquareLib)
target_include_directories(SquareLib PUBLIC ${PROJECT_SOURCE_DIR}/SquareLib)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE GCFLib::lib1 SquareLib::lib2)