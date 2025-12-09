# output: chessGame.tar.gz

BASE_NAME := hanoiTower
TARGET_DIR := .
# Name of main executable -> chessGame
TARGET := $(BASE_NAME)
# Name of archive file -> chessGame.tar.gz
TARGET_ARCHIVE := $(BASE_NAME).tar.gz
MAIN_SRC_FILES := $(wildcard *.cpp)
# List of .o object files corresponding to source files
MAIN_OBJ_FILES := $(patsubst %.cpp,%.o,$(MAIN_SRC_FILES))
# variables to manage the external shared library (engine)
ENGINE_BASE_NAME := engine
ENGINE_DIR := $(ENGINE_BASE_NAME)
ENGINE_LIB_NAME := $(ENGINE_BASE_NAME)
ENGINE_LIB_FILENAME := lib$(ENGINE_LIB_NAME).so

# C++ compiler to use -> g++
CXX := g++
CXX_FLAGS := -std=c++20 -O2
# Adds the directory ../$(ENGINE_DIR) to the list of directories from which the compiler should search for included header files
CXX_FLAGS += -I../$(ENGINE_DIR)
LDFLAGS := -L../$(ENGINE_DIR) -l$(ENGINE_LIB_NAME) -lglut -lGL -lGLU -lfreeimage
# Command to create a tar.gz archive
TAR_GZ := tar zcvf

# OS uses LD_LIBRARY_PATH to determine where to look for shared libraries required by the program
export LD_LIBRARY_PATH := ../$(ENGINE_DIR):$(LD_LIBRARY_PATH)

# package: Creates a tar.gz archive of the main executable
# default target (first)
package: $(TARGET_ARCHIVE)
	@echo "all done!"

# If $(TARGET) is update, it creates a tar.gz archive containing the main executable
package: $(TARGET_ARCHIVE)
	@echo "Package created: $(TARGET_ARCHIVE)"

$(TARGET_ARCHIVE): $(TARGET)
	# 1. Copia la libreria engine qui per includerla nell'archivio
	cp ../$(ENGINE_DIR)/$(ENGINE_LIB_FILENAME) .
	# 2. Crea l'archivio con Eseguibile + Libreria
	$(TAR_GZ) $(TARGET_ARCHIVE) $(TARGET) $(ENGINE_LIB_FILENAME)
	# (Opzionale) Rimuovi la copia locale della lib dopo il tar
	rm $(ENGINE_LIB_FILENAME)

$(TARGET): $(MAIN_OBJ_FILES)
	# build your client here
	$(CXX) $(LDFLAGS) -o $(TARGET) $(MAIN_OBJ_FILES) $(LDFLAGS)
	@echo "$(TARGET) compile done!"

# Compile .cpp source files into .o object files
%.o: %.cpp
	# eg: compile example.cpp and create example.o.
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# Used to remove files generated during the building process
clean:
	@rm -f $(TARGET)
	@rm -f $(MAIN_OBJ_FILES)

# Declaration that clean and install are not files
# Always execute commands associated with that target, regardless of whether a file with the same name exists
.PHONY: clean package
