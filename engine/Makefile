CXX := g++
LD  := g++

# --- CONFIGURAZIONE NOMI ---
BASE_NAME := engine
# Nome dell'eseguibile dei test
TESTS_RUNNER := $(BASE_NAME)-tests-runner
# File sorgente specifico dei test (nella root)
TEST_SRC := engine_test.cpp

# --- SORGENTI ENGINE ---
SRC_DIR := .
# Raccoglie tutti i .cpp MA esclude il file di test per non metterlo nella .so
ALL_CPPS := $(wildcard $(SRC_DIR)/*.cpp)
SRCS     := $(filter-out $(SRC_DIR)/$(TEST_SRC), $(ALL_CPPS))

# --- INCLUDE E FLAG ---
# include locali (engine + dipendenze vendored)
INCLUDES := -I. -I../dependencies/glm/include -I../dependencies/freeglut/include -I../dependencies/freeimage/include

# flag comuni: warning, standard, PIC, depfile per header
CXXFLAGS_COMMON := -Wall -Wextra -std=c++20 -fPIC -m64 -fexceptions -MMD -MP $(INCLUDES)
# link shared e librerie OpenGL/freeglut di sistema
LDFLAGS_COMMON := -shared -m64 -Wl,-rpath,'$$ORIGIN/../../dependencies/lib'
LIBS := -lglut -lGL -lGLU -lfreeimage -lm

# --- CONFIGURAZIONE DEBUG ---
OBJDIR_Debug    := obj/Debug
BINDIR_Debug    := bin/Debug
OBJS_Debug      := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJDIR_Debug)/%.o)
DEPS_Debug      := $(OBJS_Debug:.o=.d)
TARGET_Debug    := $(BINDIR_Debug)/lib$(BASE_NAME).so
CXXFLAGS_Debug  := $(CXXFLAGS_COMMON) -g -D_DEBUG

# --- CONFIGURAZIONE RELEASE ---
OBJDIR_Release    := obj/Release
BINDIR_Release    := bin/Release
OBJS_Release      := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJDIR_Release)/%.o)
DEPS_Release      := $(OBJS_Release:.o=.d)
TARGET_Release    := $(BINDIR_Release)/lib$(BASE_NAME).so
CXXFLAGS_Release  := $(CXXFLAGS_COMMON) -O2 -DNDEBUG

# --- TARGETS ---
all: debug release

# build Debug/Release
debug: $(TARGET_Debug)
release: $(TARGET_Release)

# --- LINKING LIBRERIA CONDIVISA ---
$(TARGET_Debug): $(OBJS_Debug) | $(BINDIR_Debug)
	$(LD) $(LDFLAGS_COMMON) -o $@ $(OBJS_Debug) $(LIBS)

$(TARGET_Release): $(OBJS_Release) | $(BINDIR_Release)
	$(LD) $(LDFLAGS_COMMON) -s -o $@ $(OBJS_Release) $(LIBS)

# --- COMPILAZIONE OGGETTI ---
$(OBJDIR_Debug)/%.o: $(SRC_DIR)/%.cpp | $(OBJDIR_Debug)
	$(CXX) $(CXXFLAGS_Debug) -c $< -o $@

$(OBJDIR_Release)/%.o: $(SRC_DIR)/%.cpp | $(OBJDIR_Release)
	$(CXX) $(CXXFLAGS_Release) -c $< -o $@

# --- COMPILAZIONE ED ESECUZIONE TEST ---
# Il test runner dipende dalla build Debug dell'engine
tests: $(TESTS_RUNNER)

# Compila l'eseguibile di test linkandolo alla libengine.so (Debug)
$(TESTS_RUNNER): $(TEST_SRC) $(TARGET_Debug)
	$(CXX) $(CXXFLAGS_Debug) $(TEST_SRC) -o $@ \
		-L$(BINDIR_Debug) -l$(BASE_NAME) $(LIBS) \
		-Wl,-rpath,'$$ORIGIN/$(BINDIR_Debug)'

# Target per GitLab CI o per lanciare i test manualmente
run-tests: tests
	./$(TESTS_RUNNER)

# --- UTILS ---
# creazione cartelle obj/bin
$(OBJDIR_Debug) $(BINDIR_Debug) $(OBJDIR_Release) $(BINDIR_Release):
	mkdir -p $@

# pulizia (rimuove anche il runner dei test)
clean:
	rm -rf $(OBJDIR_Debug) $(OBJDIR_Release) $(BINDIR_Debug) $(BINDIR_Release) $(TESTS_RUNNER)

# include automatico dei depfile generati
-include $(DEPS_Debug) $(DEPS_Release)

.PHONY: all debug release clean tests run-tests
