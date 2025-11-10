CXX = g++
AR = ar
LD = g++
WINDRES = windres

# --- Dipendenze esterne (esportate dal Makefile root) ---
INC += -I$(DEPS_DIR)/glm \
       -I$(DEPS_DIR)/freeglut/include \
       -I$(DEPS_DIR)/glew/include
LIBDIR += -L$(DEPS_DIR)/freeglut/lib \
          -L$(DEPS_DIR)/glew/lib
LIB += -lglut -lGL -lGLU -lGLEW

# --- Include interni ---
INC += -I../engine

# --- Configurazione generale ---
CFLAGS = -Wall -std=c++20 -fexceptions
RCFLAGS =
RESINC =
LIBDIR =
LIB += -lengine
LDFLAGS =

# --- DEBUG ---
INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g -D_DEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR) -L../engine/bin/Debug
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG =
OUT_DEBUG = bin/Debug/client

# --- RELEASE ---
INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR) -L../engine/bin/Release
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE =
OUT_RELEASE = bin/Release/client

# --- Sorgenti ---
OBJ_DEBUG = $(OBJDIR_DEBUG)/main.o
OBJ_RELEASE = $(OBJDIR_RELEASE)/main.o

# --- Regole principali ---
all: debug release

clean: clean_debug clean_release

before_debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug:

debug: before_debug out_debug after_debug

# Forza la ricompilazione dell'engine se serve
$(OUT_DEBUG): ../engine/bin/Debug/libengine.so

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -o $(OUT_DEBUG) $(OBJ_DEBUG) $(LIBDIR_DEBUG) $(LIB_DEBUG) $(LDFLAGS_DEBUG)

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

clean_debug:
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release:
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release:

release: before_release out_release after_release

# Forza la ricompilazione dell'engine se serve
$(OUT_RELEASE): ../engine/bin/Release/libengine.so

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -o $(OUT_RELEASE) $(OBJ_RELEASE) $(LIBDIR_RELEASE) $(LIB_RELEASE) $(LDFLAGS_RELEASE)

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

clean_release:
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release
