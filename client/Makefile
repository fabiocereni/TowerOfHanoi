# --- CLIENT MAKEFILE ---
BASE_NAME := hanoiTower
TARGET := $(BASE_NAME)
TARGET_ARCHIVE := $(BASE_NAME).tar.gz

# Sorgenti
MAIN_SRC_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp,%.o,$(MAIN_SRC_FILES))

# Riferimenti all'Engine
ENGINE_DIR := ../engine
ENGINE_LIB_NAME := engine
ENGINE_LIB_FILENAME := lib$(ENGINE_LIB_NAME).so

# Compiler Settings
CXX := g++
CXX_FLAGS := -std=c++20 -O2 -I$(ENGINE_DIR)
# Linker: cerca la libreria in ../engine durante la compilazione
LDFLAGS := -L$(ENGINE_DIR) -l$(ENGINE_LIB_NAME) -lglut -lGL -lGLU -lfreeimage
TAR_GZ := tar zcvf

# Target Principale
all: $(TARGET)

# Compilazione Eseguibile
$(TARGET): $(MAIN_OBJ_FILES)
	$(CXX) -o $(TARGET) $(MAIN_OBJ_FILES) $(LDFLAGS)
	@echo "$(TARGET) compile done!"

# Regola .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# --- PACKAGING SECTION (Corretta) ---

# Crea l'archivio con Eseguibile + Libreria .so + Script di avvio
package: $(TARGET)
	@echo "Creating package..."
	# 1. Copia la libreria dall'engine qui
	cp $(ENGINE_DIR)/$(ENGINE_LIB_FILENAME) .
	
	# 2. Crea uno script per lanciare il programma (fix per LD_LIBRARY_PATH)
	echo '#!/bin/bash' > run.sh
	echo 'export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH' >> run.sh
	echo './$(TARGET)' >> run.sh
	chmod +x run.sh
	
	# 3. Crea il tar.gz con tutto dentro
	$(TAR_GZ) $(TARGET_ARCHIVE) $(TARGET) $(ENGINE_LIB_FILENAME) run.sh
	
	# 4. Pulizia file temporanei copiati
	rm $(ENGINE_LIB_FILENAME) run.sh
	@echo "Package $(TARGET_ARCHIVE) created!"

# --- CLEAN ---
clean:
	rm -f $(TARGET) $(MAIN_OBJ_FILES) $(TARGET_ARCHIVE)

.PHONY: all clean package
