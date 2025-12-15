cmake_minimum_required(VERSION 3.16)
project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(FREEGLUT_INC "${DEPENDENCIES_DIR}/freeglut/include")
set(GLM_INC      "${DEPENDENCIES_DIR}/glm/include")
set(DEPS_LIB     "${DEPENDENCIES_DIR}/lib")


file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)


add_library(engine SHARED ${ENGINE_SOURCES}
        light_number_exception.h
        engine_test.cpp
)

target_compile_definitions(engine PRIVATE ENGINE_BUILD)


target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${FREEGLUT_INC}
        ${GLM_INC}
)


link_directories(${DEPS_LIB})


target_link_libraries(engine PUBLIC
        freeimage
        glut
        GL
        GLU
)


target_compile_options(engine PRIVATE -Wall -fPIC -fexceptions)

target_compile_definitions(engine PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
)