# compilatore e linker
CXX := g++
LD  := g++

# cartella sorgenti e auto-raccolta di tutti i .cpp
SRC_DIR := .
SRCS    := $(wildcard $(SRC_DIR)/*.cpp)

# include locali (engine + dipendenze vendored)
INCLUDES := -I. -I../dependencies/glm/include -I../dependencies/freeglut/include

# flag comuni: warning, standard, PIC, depfile per header
CXXFLAGS_COMMON := -Wall -Wextra -std=c++20 -fPIC -m64 -fexceptions -MMD -MP $(INCLUDES)
# link shared e librerie OpenGL/freeglut di sistema
LDFLAGS_COMMON := -shared -m64 -Wl,-rpath,'$$ORIGIN/../../dependencies/lib'
LIBS := -lglut -lGL -lGLU -lm

# configurazione Debug
OBJDIR_Debug   := obj/Debug
BINDIR_Debug   := bin/Debug
OBJS_Debug     := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJDIR_Debug)/%.o)
DEPS_Debug     := $(OBJS_Debug:.o=.d)
TARGET_Debug   := $(BINDIR_Debug)/libengine.so
CXXFLAGS_Debug := $(CXXFLAGS_COMMON) -g -D_DEBUG

# configurazione Release
OBJDIR_Release   := obj/Release
BINDIR_Release   := bin/Release
OBJS_Release     := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJDIR_Release)/%.o)
DEPS_Release     := $(OBJS_Release:.o=.d)
TARGET_Release   := $(BINDIR_Release)/libengine.so
CXXFLAGS_Release := $(CXXFLAGS_COMMON) -O2 -DNDEBUG

# target aggregati
all: debug release

# build Debug/Release
debug: $(TARGET_Debug)
release: $(TARGET_Release)

# link libreria condivisa
$(TARGET_Debug): $(OBJS_Debug) | $(BINDIR_Debug)
	$(LD) $(LDFLAGS_COMMON) -o $@ $(OBJS_Debug) $(LIBS)

$(TARGET_Release): $(OBJS_Release) | $(BINDIR_Release)
	$(LD) $(LDFLAGS_COMMON) -s -o $@ $(OBJS_Release) $(LIBS)

# compilazione singolo oggetto con dipendenze
$(OBJDIR_Debug)/%.o: $(SRC_DIR)/%.cpp | $(OBJDIR_Debug)
	$(CXX) $(CXXFLAGS_Debug) -c $< -o $@

$(OBJDIR_Release)/%.o: $(SRC_DIR)/%.cpp | $(OBJDIR_Release)
	$(CXX) $(CXXFLAGS_Release) -c $< -o $@

# creazione cartelle obj/bin
$(OBJDIR_Debug) $(BINDIR_Debug) $(OBJDIR_Release) $(BINDIR_Release):
	mkdir -p $@

# pulizia
clean:
	rm -rf $(OBJDIR_Debug) $(OBJDIR_Release) $(BINDIR_Debug) $(BINDIR_Release)

# include automatico dei depfile generati
-include $(DEPS_Debug) $(DEPS_Release)

.PHONY: all debug release clean
