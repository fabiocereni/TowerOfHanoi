CXX := g++
LD  := g++

# cartella sorgenti client e auto-raccolta .cpp (anche sotto Test/)
SRC_DIR := .
SRCS    := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/Test/*.cpp)

# include di engine e dipendenze vendored
INCLUDES := -I../engine -I../dependencies/glm/include -I../dependencies/freeglut/include
# flag comuni con depfile
CXXFLAGS_COMMON := -Wall -Wextra -std=c++20 -fexceptions -MMD -MP $(INCLUDES)

# librerie da linkare
ENGINE_LIB := -lengine
GL_LIBS := -lglut -lGL -lGLU -lm

# configurazione Debug
OBJDIR_Debug   := obj/Debug
BINDIR_Debug   := bin/Debug
OBJS_Debug     := $(SRCS:%.cpp=$(OBJDIR_Debug)/%.o)
DEPS_Debug     := $(OBJS_Debug:.o=.d)
TARGET_Debug   := $(BINDIR_Debug)/client
CXXFLAGS_Debug := $(CXXFLAGS_COMMON) -g -D_DEBUG
# path libreria engine e rpath relativo per trovare la .so a runtime
LDFLAGS_Debug  := -L../engine/bin/Debug -Wl,-rpath,'$$ORIGIN/../../engine/bin/Debug'

# configurazione Release
OBJDIR_Release   := obj/Release
BINDIR_Release   := bin/Release
OBJS_Release     := $(SRCS:%.cpp=$(OBJDIR_Release)/%.o)
DEPS_Release     := $(OBJS_Release:.o=.d)
TARGET_Release   := $(BINDIR_Release)/client
CXXFLAGS_Release := $(CXXFLAGS_COMMON) -O2 -DNDEBUG
LDFLAGS_Release  := -L../engine/bin/Release -Wl,-rpath,'$$ORIGIN/../../engine/bin/Release' -s

# target aggregati
all: debug release

# build Debug/Release
debug: $(TARGET_Debug)
release: $(TARGET_Release)

# link eseguibile
$(TARGET_Debug): $(OBJS_Debug) | $(BINDIR_Debug)
	$(LD) -o $@ $(OBJS_Debug) $(LDFLAGS_Debug) $(ENGINE_LIB) $(GL_LIBS)

$(TARGET_Release): $(OBJS_Release) | $(BINDIR_Release)
	$(LD) -o $@ $(OBJS_Release) $(LDFLAGS_Release) $(ENGINE_LIB) $(GL_LIBS)

# compilazione singolo oggetto
$(OBJDIR_Debug)/%.o: %.cpp | $(OBJDIR_Debug)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS_Debug) -c $< -o $@

$(OBJDIR_Release)/%.o: %.cpp | $(OBJDIR_Release)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS_Release) -c $< -o $@

# creazione cartelle obj/bin
$(OBJDIR_Debug) $(BINDIR_Debug) $(OBJDIR_Release) $(BINDIR_Release):
	mkdir -p $@

# pulizia
clean:
	rm -rf $(OBJDIR_Debug) $(OBJDIR_Release) $(BINDIR_Debug) $(BINDIR_Release)

# include dei depfile generati
-include $(DEPS_Debug) $(DEPS_Release)

.PHONY: all debug release clean
