# --- ENGINE MAKEFILE ---
BASE_NAME := engine
TARGET := lib$(BASE_NAME).so

# File sorgenti
MAIN_SRC_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(MAIN_SRC_FILES:.cpp=.o)

# Test Config
TESTS_RUNNER := $(BASE_NAME)-tests-runner
TEST_SRC_FILES := $(wildcard tests/*_test.cpp)
TEST_OBJ_FILES := $(TEST_SRC_FILES:.cpp=.o)

# Compiler Settings
CXX := g++
CXX_FLAGS := -c -fPIC -std=c++20 -O2
LD_FLAGS := -shared
LIBS := -lglut -lGL -lGLU -lfreeimage

# --- TARGETS ---

# Target di default (FIX: aggiunto 'all' per compatibilità con la root)
all: $(TARGET)

# Compilazione Libreria (.so)
$(TARGET): $(MAIN_OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o $(TARGET) $(MAIN_OBJ_FILES) $(LIBS)
	@echo "$(TARGET) build done!"

# Regola generica .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -o $@ $<

# --- TEST SECTION (Nuova) ---

# Compila ed esegue i test
test: $(TESTS_RUNNER)
	@echo "--- Executing Tests ---"
	# Imposta LD_LIBRARY_PATH corrente per trovare la libengine.so locale
	LD_LIBRARY_PATH=. ./$(TESTS_RUNNER)

# Compila il runner dei test
$(TESTS_RUNNER): $(TEST_OBJ_FILES) $(TARGET)
	$(CXX) -std=c++20 -o $(TESTS_RUNNER) $(TEST_OBJ_FILES) -L. -l$(BASE_NAME) $(LIBS)

# --- CLEAN ---
clean:
	rm -f $(TARGET) $(MAIN_OBJ_FILES) $(TESTS_RUNNER) $(TEST_OBJ_FILES)

.PHONY: all clean test
