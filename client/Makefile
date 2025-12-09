# Nome del tuo progetto
BASE_NAME := hanoiTower
TARGET_ARCHIVE := $(BASE_NAME).tar.gz

# Rilevamento OS
ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

ENGINE_DIR := ../engine
ENGINE_LIB_NAME := engine

# --- Default: Linux ---
LIB_EXT := .so
EXE_EXT :=
CXX := g++
CXX_FLAGS := -std=c++20 -O2 -I$(ENGINE_DIR)
LDFLAGS := -L$(ENGINE_DIR) -l$(ENGINE_LIB_NAME) -lglut -lGL -lGLU -lfreeimage
TAR_GZ := tar zcvf

# --- Mac Settings ---
ifeq ($(detected_OS),Darwin)
	LIB_EXT := .dylib
	CXX_FLAGS += -I/opt/homebrew/include -Wno-deprecated-declarations
	LDFLAGS := -L$(ENGINE_DIR) -L/opt/homebrew/lib -l$(ENGINE_LIB_NAME) -lglut -framework OpenGL -lfreeimage
endif

# --- Windows Settings ---
ifeq ($(detected_OS),Windows)
	LIB_EXT := .dll
	EXE_EXT := .exe
	LDFLAGS := -L$(ENGINE_DIR) -l$(ENGINE_LIB_NAME) -lfreeglut -lopengl32 -lglu32
endif

TARGET := $(BASE_NAME)$(EXE_EXT)
ENGINE_LIB_FILENAME := lib$(ENGINE_LIB_NAME)$(LIB_EXT)

MAIN_SRC_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp,%.o,$(MAIN_SRC_FILES))

all: $(TARGET)

$(TARGET): $(MAIN_OBJ_FILES)
	$(CXX) -o $(TARGET) $(MAIN_OBJ_FILES) $(LDFLAGS)
	@echo "$(TARGET) compile done!"

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# --- PACKAGING SECTION (Include Lib + Script) ---
package: $(TARGET)
	@echo "Creating package..."
	# Copia la libreria dinamica (necessaria per eseguire)
	cp $(ENGINE_DIR)/$(ENGINE_LIB_FILENAME) .
	
	# Crea script run.sh universale
	echo '#!/bin/bash' > run.sh
	echo 'OS="$$(uname -s)"' >> run.sh
	echo 'if [[ "$$OS" == "MINGW"* ]] || [[ "$$OS" == "CYGWIN"* ]]; then' >> run.sh
	echo '  ./$(TARGET)' >> run.sh
	echo 'elif [ "$$OS" = "Darwin" ]; then' >> run.sh
	echo '  export DYLD_LIBRARY_PATH=.:$$DYLD_LIBRARY_PATH' >> run.sh
	echo '  ./$(TARGET)' >> run.sh
	echo 'else' >> run.sh
	echo '  export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH' >> run.sh
	echo '  ./$(TARGET)' >> run.sh
	echo 'fi' >> run.sh
	chmod +x run.sh
	
	# Crea l'archivio
	$(TAR_GZ) $(TARGET_ARCHIVE) $(TARGET) $(ENGINE_LIB_FILENAME) run.sh
	
	# Pulizia file temporanei
	rm $(ENGINE_LIB_FILENAME) run.sh
	@echo "Package $(TARGET_ARCHIVE) created!"

clean:
	rm -f $(TARGET) $(MAIN_OBJ_FILES) $(TARGET_ARCHIVE)

.PHONY: all clean package
