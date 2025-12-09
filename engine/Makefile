# output: shared library (.so) and an executable to run tests


BASE_NAME := engine
TARGET_DIR := .
# Name of shared library that will be build --> libengine.so
TARGET := lib$(BASE_NAME).so
# Set of all .cpp files in the current directory
MAIN_SRC_FILES := $(wildcard *.cpp)
# Set of all object (.o) correspondents to (.cpp) files
MAIN_OBJ_FILES := $(MAIN_SRC_FILES:.cpp=.o)
# Name of the executable file for the test runner --> engine-tests-runner
TESTS_RUNNER := $(BASE_NAME)-tests-runner
# Set of all test file in the tests directory
TEST_SRC_FILES := $(wildcard tests/*_test.cpp)
# Set of all object file (.o) for the test file
TEST_OBJ_FILES := $(TEST_SRC_FILES:.cpp=.o)
DEPENDENCIES_DIR := dependencies

# Specifies the C++ compiler to use --> g++
CXX := g++
CXX_FLAGS := -c -fPIC -std=c++20 -O2
# Flag for the linker to create a shared library
LD_FLAGS := -shared
# Libraries to link with the project (glut, GL, GLU, freeimage)
LIBS := -lglut -lGL -lGLU -lfreeimage

# Your default target (first in the makefile) run make without specifying a target (ex: make clean)
install: $(TARGET)
	@echo "all done!"

# Target for building the shared library
# check if the test runner and object files are update
$(TARGET): $(MAIN_OBJ_FILES)
	# builds the shared library
	$(CXX) $(LD_FLAGS) -o $(TARGET) $(MAIN_OBJ_FILES) $(LIBS)
	@echo "$(TARGET) build done!"

# Generic rule to compile source file (.c++) into object file (.o)
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -o $@ $<

# Target to clean generated files (.o, executables)
# Removes test runner, object files and shared library
clean:
	@rm -f $(TARGET)
	@rm -f $(MAIN_OBJ_FILES)

# Declaration that clean and install are not files
# Always execute commands associated with that target, regardless of whether a file with the same name exists
.PHONY: clean install
