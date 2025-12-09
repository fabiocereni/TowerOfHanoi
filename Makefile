# --- CONFIGURAZIONE DIRECTORY ---
CLIENT_DIR = client
ENGINE_DIR = engine
ARCHIVE_NAME = hanoiTower_release.tar.gz

.PHONY: all build_engine build_client test package clean cross_win universal

# Default target
all: package

# Compilazione e Test Engine
build_engine:
	$(MAKE) -C $(ENGINE_DIR) test

# Pacchetto Linux Locale
package: build_engine
	@echo "=== Packaging Linux ==="
	$(MAKE) -C $(CLIENT_DIR) all
	$(MAKE) -C $(CLIENT_DIR) package
	# Sposta il file generato (tar.gz) nella root
	mv $(CLIENT_DIR)/*.tar.gz . 2>/dev/null || true

# --- CROSS-COMPILE WINDOWS (Scarica dipendenze al volo) ---
cross_win:
	@echo "=== Cross Compiling for Windows ==="
	
	# 1. Prepara cartella dipendenze
	rm -rf win_deps
	mkdir -p win_deps
	
	# 2. Scarica FreeGLUT per MinGW (Header e Libs per Windows)
	wget -q -O win_deps/freeglut.zip https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MinGW-3.0.0-1.mp.zip
	unzip -o -q win_deps/freeglut.zip -d win_deps
	# La struttura dello zip è win_deps/freeglut/...
	
	# 3. Compila Engine (DLL) - Static Linking
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(ENGINE_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -Dfreeglut_static -D_WIN32 -I../win_deps/freeglut/include" \
		LIBS="-L../win_deps/freeglut/lib/x64 -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition" all
	
	# 4. Compila Client (EXE) - Static Linking
	$(MAKE) -C $(CLIENT_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=hanoiTower.exe ENGINE_LIB_FILENAME=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -D_WIN32 -I../engine -I../win_deps/freeglut/include" \
		LDFLAGS="-L../engine -lengine -L../win_deps/freeglut/lib/x64 -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition" all
	
	# 5. Crea pacchetto ZIP per Windows
	rm -rf windows_dist
	mkdir -p windows_dist
	cp $(CLIENT_DIR)/hanoiTower.exe windows_dist/
	cp $(ENGINE_DIR)/libengine.dll windows_dist/
	# Copia anche la DLL di freeglut (anche se abbiamo linkato statico, a volte serve, nel dubbio la mettiamo o si linka statico la lib)
	# Nota: avendo usato -Dfreeglut_static e -lfreeglut (versione static) non dovrebbe servire la dll, ma la copiamo per sicurezza se presente
	cp win_deps/freeglut/bin/x64/freeglut.dll windows_dist/ 2>/dev/null || true
	
	cd windows_dist && zip -r ../hanoiTower_win.zip .
	
	# Pulizia dipendenze windows
	rm -rf windows_dist win_deps

# --- BUILD UNIVERSALE (Chiamato dalla CI) ---
universal:
	@echo "=== Creating Universal Bundle ==="
	rm -rf universal_bundle hanoiTower_COMPLETE.zip
	mkdir -p universal_bundle/linux
	mkdir -p universal_bundle/windows
	
	# 1. FASE LINUX
	$(MAKE) clean
	$(MAKE) package
	mv *.tar.gz universal_bundle/linux/
	
	# 2. FASE WINDOWS
	$(MAKE) clean
	$(MAKE) cross_win
	mv *.zip universal_bundle/windows/
	
	# 3. ZIP FINALE
	zip -r hanoiTower_COMPLETE.zip universal_bundle
	rm -rf universal_bundle
	@echo "SUCCESS: hanoiTower_COMPLETE.zip created!"

clean:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -rf win_deps windows_dist universal_bundle *.zip *.tar.gz
