MAKE = make
ENGINE_DIR = engine
CLIENT_DIR = client
ARCHIVE_NAME = hanoiTower_release.tar.gz

.PHONY: all build_engine build_client test package clean cross_win universal

# Default
all: package

build_engine:
	@echo "=== Building Engine ==="
	$(MAKE) -C $(ENGINE_DIR) all

build_client: build_engine
	@echo "=== Building Client ==="
	$(MAKE) -C $(CLIENT_DIR) all

test: build_engine
	@echo "=== Running Engine Tests ==="
	$(MAKE) -C $(ENGINE_DIR) test

package: test build_client
	@echo "=== Packaging Linux ==="
	$(MAKE) -C $(CLIENT_DIR) package
	mv $(CLIENT_DIR)/*.tar.gz . 2>/dev/null || true

# --- CROSS-COMPILE WINDOWS ---
cross_win:
	@echo "=== Cross Compiling for Windows ==="
	mkdir -p win_deps
	wget -q -O win_deps/freeglut.zip https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MinGW-3.0.0-1.mp.zip
	unzip -o -q win_deps/freeglut.zip -d win_deps
	
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(ENGINE_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -Dfreeglut_static -I../dependencies/glm/include -I../win_deps/freeglut/include" \
		LIBS="-L../win_deps/freeglut/lib/x64 -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++" all
	
	$(MAKE) -C $(CLIENT_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=hanoiTower.exe ENGINE_LIB_FILENAME=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -I../engine -I../dependencies/glm/include -I../win_deps/freeglut/include" \
		LDFLAGS="-L../engine -lengine -L../win_deps/freeglut/lib/x64 -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++" all
	
	mkdir -p windows_dist
	cp $(CLIENT_DIR)/hanoiTower.exe windows_dist/
	cp $(ENGINE_DIR)/libengine.dll windows_dist/
	cp win_deps/freeglut/bin/x64/freeglut.dll windows_dist/
	# Creiamo lo zip temporaneo per Windows
	cd windows_dist && zip -r ../hanoiTower_win.zip .
	rm -rf windows_dist win_deps

# --- NUOVO COMANDO: BUILD UNIVERSALE (Linux + Windows) ---
universal:
	@echo "=== Creating Universal Bundle ==="
	rm -rf universal_bundle
	mkdir -p universal_bundle/linux
	mkdir -p universal_bundle/windows
	
	# 1. Compila versione Linux
	$(MAKE) clean
	$(MAKE) package
	# Sposta il tar.gz nella cartella linux
	mv *.tar.gz universal_bundle/linux/
	
	# 2. Compila versione Windows
	$(MAKE) cross_win
	# Sposta lo zip windows nella cartella windows (e lo scompatta se preferisci, qui lo lascio zip)
	mv *.zip universal_bundle/windows/
	
	# 3. Crea il pacchetto finale unico
	zip -r hanoiTower_COMPLETE.zip universal_bundle
	rm -rf universal_bundle
	@echo "SUCCESS: hanoiTower_COMPLETE.zip created!"

clean:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -f *.tar.gz *.zip
