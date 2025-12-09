cmake_minimum_required(VERSION 3.16)
project(engine LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dipendenze (per Linux, se installate via pacman/paru, i path manuali spesso non servono,
# ma li lascio per compatibilità col tuo setup esistente)
set(FREEGLUT_INC "${DEPENDENCIES_DIR}/freeglut/include")
set(GLM_INC      "${DEPENDENCIES_DIR}/glm/include")
set(DEPS_LIB     "${DEPENDENCIES_DIR}/lib")

# ====================================
# 1) RACCOLTA AUTOMATICA SORGENTI
# ====================================
# Nota: GLOB_RECURSE trova già tutto. Non serve elencare i file manualmente dopo.
file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# ====================================
# 2) TARGET LIBRARY SHARED
# ====================================
# Qui usiamo solo ${ENGINE_SOURCES} per evitare duplicati
add_library(engine SHARED ${ENGINE_SOURCES}
        light_number_exception.h
)

target_compile_definitions(engine PRIVATE ENGINE_BUILD)

# ====================================
# 3) INCLUDE PATHS
# ====================================
target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${FREEGLUT_INC}
        ${GLM_INC}
        # Su Linux FreeImage è in /usr/include, che è incluso di default,
        # ma se fosse altrove verrebbe trovato automaticamente dal sistema.
)

# ====================================
# 4) LINKING
# ====================================
link_directories(${DEPS_LIB})

# IMPORTANTE: Aggiunto 'freeimage' alla lista.
# Uso PUBLIC così chi usa 'engine' eredita le dipendenze se necessario.
target_link_libraries(engine PUBLIC
        freeimage
        glut
        GL
        GLU
)

# ====================================
# 5) FLAGS COMPILAZIONE
# ====================================
target_compile_options(engine PRIVATE -Wall -fPIC -fexceptions)

target_compile_definitions(engine PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
)