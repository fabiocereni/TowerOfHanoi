BASE_NAME := engine

# Rilevamento OS
ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

# --- Default: Linux Settings ---
LIB_EXT := .so
CXX_FLAGS := -c -fPIC -std=c++20 -O2
LD_FLAGS := -shared
LIBS := -lglut -lGL -lGLU -lfreeimage
EXE_EXT :=

# --- Mac Settings (Darwin) ---
ifeq ($(detected_OS),Darwin)
	LIB_EXT := .dylib
	CXX_FLAGS += -I/opt/homebrew/include -Wno-deprecated-declarations
	LD_FLAGS := -dynamiclib
	LIBS := -L/opt/homebrew/lib -lglut -framework OpenGL -lfreeimage
endif

# --- Windows Settings ---
ifeq ($(detected_OS),Windows)
	LIB_EXT := .dll
	LIBS := -lfreeglut -lopengl32 -lglu32
	LD_FLAGS := -shared
	EXE_EXT := .exe
endif

TARGET := lib$(BASE_NAME)$(LIB_EXT)

# Sorgenti
MAIN_SRC_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(MAIN_SRC_FILES:.cpp=.o)

# Test Config
TESTS_RUNNER := $(BASE_NAME)-tests-runner$(EXE_EXT)
TEST_SRC_FILES := $(wildcard tests/*_test.cpp)
TEST_OBJ_FILES := $(TEST_SRC_FILES:.cpp=.o)
CXX := g++

# --- TARGETS ---
all: $(TARGET)

$(TARGET): $(MAIN_OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o $(TARGET) $(MAIN_OBJ_FILES) $(LIBS)
	@echo "$(TARGET) build done!"

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -o $@ $<

# --- TEST SECTION (Importante: esegue i test) ---
test: $(TESTS_RUNNER)
	@echo "--- Executing Tests ---"
ifeq ($(detected_OS),Windows)
	./$(TESTS_RUNNER)
else ifeq ($(detected_OS),Darwin)
	DYLD_LIBRARY_PATH=. ./$(TESTS_RUNNER)
else
	LD_LIBRARY_PATH=. ./$(TESTS_RUNNER)
endif

$(TESTS_RUNNER): $(TEST_OBJ_FILES) $(TARGET)
	$(CXX) -std=c++20 -o $(TESTS_RUNNER) $(TEST_OBJ_FILES) -L. -l$(BASE_NAME) $(LIBS)

clean:
	rm -f *.so *.dylib *.dll $(MAIN_OBJ_FILES) $(TESTS_RUNNER) $(TEST_OBJ_FILES)

.PHONY: all clean test
