cmake_minimum_required(VERSION 3.10)

add_compile_options(-pedantic -Wall)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lab4)

# Add the source files for the shared and static versions
set(SHARED_SOURCES src/main.cpp)
set(STATIC_SOURCES src/main2.cpp)

# Create shared library for GCFLib
add_library(GCFLib SHARED GCFLib/GCF.cpp)
add_library(GCFLib::lib1 ALIAS GCFLib)
target_include_directories(GCFLib PUBLIC ${PROJECT_SOURCE_DIR}/GCFLib)

# Create static library for GCFLib
add_library(GCFLib_static STATIC GCFLib/GCF.cpp)
target_include_directories(GCFLib_static PUBLIC ${PROJECT_SOURCE_DIR}/GCFLib)

# Create shared library for SquareLib
add_library(SquareLib SHARED SquareLib/Square.cpp)
add_library(SquareLib::lib2 ALIAS SquareLib)
target_include_directories(SquareLib PUBLIC ${PROJECT_SOURCE_DIR}/SquareLib)

# Create static library for SquareLib
add_library(SquareLib_static STATIC SquareLib/Square.cpp)
target_include_directories(SquareLib_static PUBLIC ${PROJECT_SOURCE_DIR}/SquareLib)

# Create the executable for the dynamic linking version
add_executable(${PROJECT_NAME}_dynamic ${SHARED_SOURCES})
target_link_libraries(${PROJECT_NAME}_dynamic PRIVATE GCFLib::lib1 SquareLib::lib2)

# Create the executable for the static linking version
add_executable(${PROJECT_NAME}_static ${STATIC_SOURCES})
target_link_libraries(${PROJECT_NAME}_static PRIVATE GCFLib_static SquareLib_static)
