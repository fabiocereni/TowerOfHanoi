cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)


set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")

set(FREEGLUT_INC "${DEPENDENCIES_DIR}/freeglut/include")
set(GLM_INC      "${DEPENDENCIES_DIR}/glm/include")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE CLIENT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)


add_executable(client ${CLIENT_SOURCES}
        setup.h
        setup.cpp)


target_include_directories(client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/engine
        ${GLM_INC}
        ${FREEGLUT_INC}
)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(client PRIVATE engine OpenGL::GL GLUT::GLUT)

target_compile_options(client PRIVATE -Wall -fexceptions)

target_compile_definitions(client PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
)

set_target_properties(client PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>"
)
