# Makefile Root

# Nomi file
PKG_NAME = hanoiTower.tar.gz
FINAL_PKG_NAME = hanoiTower_linux_complete.tar.gz
ARTIFACT_NAME = hanoiTower_COMPLETE.zip

# Compilatore Windows (Cross-compiler)
WIN_CXX = x86_64-w64-mingw32-g++
# FIX: Aggiunto -I/usr/include per trovare GLM anche con MinGW
WIN_FLAGS = -std=c++20 -O2 -D_WIN32 -I/usr/include

# --- TARGETS ---
.PHONY: all clean engine client package universal test windows_build linux_build

all: engine client

# Standard build (Linux locale)
engine:
	$(MAKE) -C engine all

client: engine
	$(MAKE) -C client all

test: engine
	$(MAKE) -C engine test

# Clean: Pulisce tutto (compresi i temporanei della CI)
clean:
	$(MAKE) -C engine clean
	$(MAKE) -C client clean
	rm -f *.so *.dylib *.dll *.exe *.o *.tar.gz *.zip
	rm -rf temp_pkg universal_bundle release_temp client/hanoiTower client/*.o engine/*.o engine/*.so

# --- BUILD LINUX ---
linux_build:
	@echo "=== [LINUX] Building... ==="
	$(MAKE) -C engine all
	$(MAKE) -C client all
	$(MAKE) -C client package
	# Sposta il pacchetto generato nella root
	mv client/$(PKG_NAME) . 2>/dev/null || true
	
	# Repackaging completo
	rm -rf temp_pkg && mkdir -p temp_pkg
	tar -xf $(PKG_NAME) -C temp_pkg
	tar -C temp_pkg -zcvf $(FINAL_PKG_NAME) .
	rm -rf temp_pkg
	@echo "=== [LINUX] Done ==="

# --- BUILD WINDOWS ---
windows_build:
	@echo "=== [WINDOWS] Cross-Compiling... ==="
	# Compila DLL Engine (Linka GLM grazie a -I/usr/include)
	$(WIN_CXX) -shared -o libengine.dll engine/*.cpp $(WIN_FLAGS) -lfreeglut -lopengl32 -lglu32 -lfreeimage
	# Compila EXE Client
	$(WIN_CXX) -o hanoiTower.exe client/main.cpp -Iengine -L. -lengine $(WIN_FLAGS)
	@echo "=== [WINDOWS] Done ==="

# --- TARGET UNIVERSALE (CI) ---
universal:
	@echo "=== Starting Universal Build ==="
	
	# 1. Preparazione cartella sicura per gli artefatti (non cancellata da make clean)
	rm -rf build_artifacts
	mkdir -p build_artifacts/linux
	mkdir -p build_artifacts/windows

	# ---------------------------------------------------------
	# FASE 1: WINDOWS
	# ---------------------------------------------------------
	$(MAKE) clean
	$(MAKE) windows_build
	# Salviamo subito i file Windows al sicuro
	mv hanoiTower.exe build_artifacts/windows/
	mv libengine.dll build_artifacts/windows/
	# Creiamo un bat per avviare
	echo "hanoiTower.exe" > build_artifacts/windows/run.bat

	# ---------------------------------------------------------
	# FASE 2: LINUX
	# ---------------------------------------------------------
	$(MAKE) clean
	$(MAKE) linux_build
	# Salviamo il file Linux al sicuro
	mv $(FINAL_PKG_NAME) build_artifacts/linux/

	# ---------------------------------------------------------
	# FASE 3: PACKAGING FINALE
	# ---------------------------------------------------------
	# Creiamo la cartella finale da zippare
	rm -rf release_temp
	mkdir -p release_temp
	
	# Copiamo i file salvati
	cp -r build_artifacts/linux release_temp/
	cp -r build_artifacts/windows release_temp/
	
	# Zippiamo tutto
	cd release_temp && zip -r ../$(ARTIFACT_NAME
