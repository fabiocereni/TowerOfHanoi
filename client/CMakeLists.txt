cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)

# Percorso dipendenze ereditato dal root
set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")

set(FREEGLUT_INC "${DEPENDENCIES_DIR}/freeglut/include")
set(GLM_INC      "${DEPENDENCIES_DIR}/glm/include")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Raccolta sorgenti ----
file(GLOB_RECURSE CLIENT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# ---- Crea eseguibile ----
add_executable(client ${CLIENT_SOURCES}
        Test/test_suite.h
        Test/test_suite.cpp)

# ---- Include necessari ----
target_include_directories(client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}   # sorgenti del client
        ${CMAKE_SOURCE_DIR}/engine    # header dell'engine
        ${GLM_INC}                    # glm
        ${FREEGLUT_INC}               # freeglut, solo se il client include GL
)

# ---- Trova e linka le librerie GLUT e OpenGL ----
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# ---- Linka l'engine e le librerie di sistema (OpenGL e GLUT) ----
target_link_libraries(client PRIVATE engine OpenGL::GL GLUT::GLUT)

# ---- Opzioni di compilazione ----
target_compile_options(client PRIVATE -Wall -fexceptions)

target_compile_definitions(client PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
)

# ---- Output ----
set_target_properties(client PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>"
)
