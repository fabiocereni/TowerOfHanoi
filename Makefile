# Makefile Root

# === CONFIGURAZIONE ===
PKG_NAME       = hanoiTower.tar.gz
FINAL_PKG_NAME = hanoiTower_linux_complete.tar.gz
ARTIFACT_NAME  = hanoiTower_COMPLETE.zip

# Cross-compiler per Windows
WIN_CXX   = x86_64-w64-mingw32-g++

# FIX CRITICO: Non usiamo -I/usr/include.
# Usiamo una cartella locale 'win_deps/include' dove copieremo solo ciò che serve.
WIN_FLAGS = -std=c++20 -O2 -D_WIN32 -I./win_deps/include

.PHONY: all clean engine client package universal test windows_build linux_build

all: engine client

# === TARGET STANDARD (Linux Locale) ===
engine:
	$(MAKE) -C engine all

client: engine
	$(MAKE) -C client all

test: engine
	$(MAKE) -C engine test

clean:
	$(MAKE) -C engine clean
	$(MAKE) -C client clean
	rm -f *.so *.dylib *.dll *.exe *.o *.tar.gz *.zip
	rm -rf temp_pkg universal_bundle release_temp build_artifacts win_deps
	rm -f client/hanoiTower client/*.o engine/*.o engine/*.so

# === BUILD LINUX (Helper) ===
linux_build:
	@echo "=== [LINUX] Building... ==="
	$(MAKE) -C engine all
	$(MAKE) -C client all
	$(MAKE) -C client package
	# Sposta e rinomina il pacchetto
	mv client/$(PKG_NAME) . 2>/dev/null || true
	rm -rf temp_pkg && mkdir -p temp_pkg
	tar -xf $(PKG_NAME) -C temp_pkg
	tar -C temp_pkg -zcvf $(FINAL_PKG_NAME) .
	rm -rf temp_pkg
	@echo "=== [LINUX] Done ==="

# === BUILD WINDOWS (Helper) ===
windows_build:
	@echo "=== [WINDOWS] Preparing Headers... ==="
	# 1. Crea una cartella temporanea per le dipendenze Windows
	rm -rf win_deps
	mkdir -p win_deps/include
	
	# 2. Copia GLM da sistema Linux a cartella locale per MinGW
	# (GLM è header-only, quindi funziona ovunque se isolata)
	cp -r /usr/include/glm win_deps/include/
	
	# (Opzionale) Se servono header di FreeImage/FreeGLUT e non li hai nel progetto:
	# cp /usr/include/FreeImage.h win_deps/include/ 2>/dev/null || true
	# cp -r /usr/include/GL win_deps/include/ 2>/dev/null || true

	@echo "=== [WINDOWS] Cross-Compiling... ==="
	# Nota: Se fallisce il LINKING ("cannot find -lfreeglut"), mancano le librerie .a/.lib per Windows.
	# Ma questo risolve l'errore di compilazione degli header.
	$(WIN_CXX) -shared -o libengine.dll engine/*.cpp $(WIN_FLAGS) -lfreeglut -lopengl32 -lglu32 -lfreeimage
	$(WIN_CXX) -o hanoiTower.exe client/main.cpp -Iengine -L. -lengine $(WIN_FLAGS)
	@echo "=== [WINDOWS] Done ==="

# === TARGET UNIVERSALE (CI) ===
universal:
	@echo "=== Starting Universal Build ==="
	
	# 1. Prepara cartelle sicure
	rm -rf build_artifacts
	mkdir -p build_artifacts/linux
	mkdir -p build_artifacts/windows

	# 2. Build Windows
	$(MAKE) clean
	$(MAKE) windows_build
	mv hanoiTower.exe build_artifacts/windows/
	mv libengine.dll build_artifacts/windows/
	echo "hanoiTower.exe" > build_artifacts/windows/run.bat

	# 3. Build Linux
	$(MAKE) clean
	$(MAKE) linux_build
	mv $(FINAL_PKG_NAME) build_artifacts/linux/

	# 4. Crea ZIP finale
	rm -rf release_temp
	mkdir -p release_temp
	cp -r build_artifacts/linux release_temp/
	cp -r build_artifacts/windows release_temp/
	cd release_temp && zip -r ../$(ARTIFACT_NAME) .
	
	@echo "Artifact creato: $(ARTIFACT_NAME)"
	
	# Pulizia finale
	rm -rf release_temp build_artifacts win_deps
