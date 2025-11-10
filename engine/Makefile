CXX = g++
AR = ar
LD = g++
WINDRES = windres

# --- Dipendenze esterne (arrivano dal Makefile root) ---
# DEPS_DIR viene esportata, ad esempio $(CURDIR)/../dependencies
INC += -I$(DEPS_DIR)/glm \
       -I$(DEPS_DIR)/freeglut/include \
       -I$(DEPS_DIR)/glew/include
LIBDIR += -L$(DEPS_DIR)/freeglut/lib \
          -L$(DEPS_DIR)/glew/lib
LIB += -lglut -lGL -lGLU -lGLEW

# --- Configurazione generale ---
CFLAGS = -Wall -std=c++20 -fPIC -fexceptions
LDFLAGS =
RCFLAGS =
RESINC =
LIBDIR =
LIB =

# --- DEBUG ---
INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g -D_DEBUG
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG =
OUT_DEBUG = bin/Debug/libengine.so

# --- RELEASE ---
INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE =
OUT_RELEASE = bin/Release/libengine.so

# --- Sorgenti ---
OBJ_DEBUG = $(OBJDIR_DEBUG)/engine.o
OBJ_RELEASE = $(OBJDIR_RELEASE)/engine.o

# --- Regole principali ---
all: debug release

clean: clean_debug clean_release

before_debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug:

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared -o $(OUT_DEBUG) $(OBJ_DEBUG) $(LIBDIR_DEBUG) $(LIB_DEBUG) $(LDFLAGS_DEBUG)

$(OBJDIR_DEBUG)/engine.o: engine.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c engine.cpp -o $(OBJDIR_DEBUG)/engine.o

clean_debug:
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release:
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release:

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -shared -o $(OUT_RELEASE) $(OBJ_RELEASE) $(LIBDIR_RELEASE) $(LIB_RELEASE) $(LDFLAGS_RELEASE)

$(OBJDIR_RELEASE)/engine.o: engine.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c engine.cpp -o $(OBJDIR_RELEASE)/engine.o

clean_release:
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release
