cmake_minimum_required(VERSION 3.20)
project(Gruppo_03 LANGUAGES CXX)

set(DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_compile_options(-Wall -Wextra -Wpedantic)



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/Release")


set(CMAKE_POSITION_INDEPENDENT_CODE ON)


include_directories(
        ${DEPENDENCIES_DIR}
        ${CMAKE_SOURCE_DIR}/engine
)


add_subdirectory(engine)
add_subdirectory(client)


add_executable(engine_test_runner engine/engine_test.cpp)
target_link_libraries(engine_test_runner PRIVATE engine)



message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Dependencies: ${DEPENDENCIES_DIR}")
message(STATUS "Output binario: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")