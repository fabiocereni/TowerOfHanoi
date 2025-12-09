# --- CONFIGURAZIONE ---
ENGINE_DIR = engine
CLIENT_DIR = client

.PHONY: all clean clean_builds build_linux package_linux cross_win universal

# Default
all: build_linux

# --- 1. BUILD LINUX (Standard) ---
build_linux:
	@echo "=== Building Engine (Linux) ==="
	$(MAKE) -C $(ENGINE_DIR) all
	@echo "=== Building Client (Linux) ==="
	$(MAKE) -C $(CLIENT_DIR) all

# --- 2. PACKAGE LINUX ---
package_linux: build_linux
	@echo "=== Packaging Linux Bundle ==="
	rm -rf linux_dist
	mkdir -p linux_dist/libs
	
	# 1. Copia eseguibile e libreria engine
	# I file vengono creati nella root delle rispettive cartelle
	cp $(CLIENT_DIR)/hanoiTower linux_dist/hanoiTower
	cp $(ENGINE_DIR)/libengine.so linux_dist/libs/
	
	# 2. Copia dipendenze di sistema (ldd)
	@echo "Gathering system libraries..."
	ldd linux_dist/hanoiTower | grep "=> /" | while read -r line; do \
		lib_path=$$(echo "$$line" | awk '{print $$3}'); \
		lib_name=$$(echo "$$line" | awk '{print $$1}'); \
		if echo "$$lib_name" | grep -Eq "^(libc\.so|libstdc\+\+|libm\.so|libpthread|libdl|librt|ld-linux|libgcc_s)"; then continue; fi; \
		echo "Copying $$lib_name"; \
		cp -L -n "$$lib_path" linux_dist/libs/ || true; \
	done
	
	# Fix specifico per libglut
	if [ ! -f linux_dist/libs/libglut.so.3 ]; then \
		GLUT_PATH=$$(find /usr/lib -name "libglut.so*" | head -n 1); \
		if [ -n "$$GLUT_PATH" ]; then \
			cp -L "$$GLUT_PATH" linux_dist/libs/libglut.so.3; \
			cp -L "$$GLUT_PATH" linux_dist/libs/libglut.so; \
		fi \
	fi

	# 3. Crea script di avvio run.sh
	echo '#!/bin/bash' > linux_dist/run.sh
	echo 'DIR="$$( cd "$$( dirname "$${BASH_SOURCE[0]}" )" && pwd )"' >> linux_dist/run.sh
	echo 'export LD_LIBRARY_PATH="$$DIR/libs:$$LD_LIBRARY_PATH"' >> linux_dist/run.sh
	echo 'chmod +x "$$DIR/hanoiTower"' >> linux_dist/run.sh
	echo 'exec "$$DIR/hanoiTower"' >> linux_dist/run.sh
	chmod +x linux_dist/run.sh
	
	# 4. Crea archivio Linux
	cd linux_dist && tar -czvf ../hanoiTower_linux.tar.gz .

# --- 3. CROSS-COMPILE WINDOWS ---
cross_win:
	@echo "=== Cross Compiling for Windows ==="
	rm -rf win_deps windows_dist
	mkdir -p win_deps
	
	# Scarica FreeGLUT
	wget -q -O win_deps/freeglut.zip https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MinGW-3.0.0-1.mp.zip
	unzip -o -q win_deps/freeglut.zip -d win_deps
	
	# Pulisce build precedenti (Solo Engine/Client, NON universal_bundle)
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	
	# Compila Engine (DLL)
	$(MAKE) -C $(ENGINE_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -Dfreeglut_static -D_WIN32 -I../win_deps/freeglut/include" \
		LIBS="-L../win_deps/freeglut/lib/x64 -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition" all
	
	# Compila Client (EXE)
	$(MAKE) -C $(CLIENT_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=hanoiTower.exe ENGINE_LIB_FILENAME=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -D_WIN32 -I../engine -I../win_deps/freeglut/include" \
		LDFLAGS="-L../engine -lengine -L../win_deps/freeglut/lib/x64 -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition" all
	
	# Pacchettizzazione Windows
	mkdir -p windows_dist
	cp $(CLIENT_DIR)/hanoiTower.exe windows_dist/
	cp $(ENGINE_DIR)/libengine.dll windows_dist/
	cp win_deps/freeglut/bin/x64/freeglut.dll windows_dist/ 2>/dev/null || true
	
	# Script run.bat semplice
	echo "hanoiTower.exe" > windows_dist/run.bat
	
	cd windows_dist && zip -r ../hanoiTower_win.zip .
	rm -rf win_deps windows_dist

# --- Helper: Pulisce solo i file di build (non il bundle finale) ---
clean_builds:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -rf linux_dist windows_dist win_deps

# --- 4. UNIVERSAL (Orchestratore) ---
universal:
	@echo "=== Creating Universal Bundle ==="
	# 1. Pulizia Totale Iniziale
	$(MAKE) clean
	
	# 2. Crea cartelle di destinazione (DOPO clean!)
	mkdir -p universal_bundle/linux
	mkdir -p universal_bundle/windows
	
	# ----------------------------------------
	# FASE 1: LINUX
	# ----------------------------------------
	$(MAKE) package_linux
	mv hanoiTower_linux.tar.gz universal_bundle/linux/
	
	# Pulizia intermedia (SOLO oggetti, NON universal_bundle)
	$(MAKE) clean_builds
	
	# ----------------------------------------
	# FASE 2: WINDOWS
	# ----------------------------------------
	$(MAKE) cross_win
	mv hanoiTower_win.zip universal_bundle/windows/
	
	# ----------------------------------------
	# PACKAGING FINALE
	# ----------------------------------------
	zip -r hanoiTower_COMPLETE.zip universal_bundle
	rm -rf universal_bundle
	@echo "SUCCESS: hanoiTower_COMPLETE.zip created!"

# Clean Globale (Cancella tutto)
clean:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -rf win_deps windows_dist linux_dist universal_bundle *.zip *.tar.gz
