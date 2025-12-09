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
	
	# --- FIX LINUX: INIEZIONE DIPENDENZE ---
	mkdir -p temp_pkg
	tar -xf $(ARCHIVE_NAME) -C temp_pkg
	cp /usr/lib/x86_64-linux-gnu/libglut.so.3* temp_pkg/ 2>/dev/null || true
	cp /usr/lib/x86_64-linux-gnu/libGLU.so.1* temp_pkg/ 2>/dev/null || true
	tar -czvf $(ARCHIVE_NAME) -C temp_pkg .
	rm -rf temp_pkg
	@echo "Linux Package creato con dipendenze incluse."

# --- CROSS-COMPILE WINDOWS (Usa la TUA cartella dependencies) ---
cross_win:
	@echo "=== Cross Compiling for Windows using LOCAL dependencies ==="
	
	# Definiamo i path basandoci sulla tua cartella 'dependencies'
	# Nota: Adatto i path alla struttura che vedo nei tuoi file caricati
	$(eval DEP_INC := -I../dependencies/glm/include -I../dependencies/freeglut/include -I../dependencies/freeimage/include)
	$(eval DEP_LIB := -L../dependencies/freeglut/lib/x64/Release -L../dependencies/freeimage/lib/x64/Release)
	
	# 1. Compila Engine (DLL)
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(ENGINE_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -Dfreeglut_static $(DEP_INC)" \
		LIBS="$(DEP_LIB) -lfreeglut_static -lFreeImage -lopengl32 -lglu32 -static-libgcc -static-libstdc++" all
	
	# 2. Compila Client (EXE)
	$(MAKE) -C $(CLIENT_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=hanoiTower.exe ENGINE_LIB_FILENAME=libengine.dll \
		CXX_FLAGS="-c -O2 -std=c++20 -I../engine $(DEP_INC)" \
		LDFLAGS="-L../engine -lengine $(DEP_LIB) -lfreeglut_static -lFreeImage -lopengl32 -lglu32 -static-libgcc -static-libstdc++" all
	
	# 3. Crea ZIP Windows
	mkdir -p windows_dist
	cp $(CLIENT_DIR)/hanoiTower.exe windows_dist/
	cp $(ENGINE_DIR)/libengine.dll windows_dist/
	# Se hai una DLL di FreeImage o FreeGLUT nella cartella dependencies, copiala qui
	# cp dependencies/freeglut/bin/x64/freeglut.dll windows_dist/ 2>/dev/null || true
	# cp dependencies/freeimage/Dist/x64/FreeImage.dll windows_dist/ 2>/dev/null || true
	
	cd windows_dist && zip -r ../hanoiTower_win.zip .
	rm -rf windows_dist

# --- BUILD UNIVERSALE ---
universal:
	@echo "=== Creating Universal Bundle ==="
	rm -rf universal_bundle
	mkdir -p universal_bundle/linux
	mkdir -p universal_bundle/windows
	
	# 1. Linux
	$(MAKE) clean
	$(MAKE) package
	mv *.tar.gz universal_bundle/linux/
	
	# 2. Windows (Usa le tue dipendenze)
	$(MAKE) cross_win
	mv *.zip universal_bundle/windows/
	
	# 3. Finale
	zip -r hanoiTower_COMPLETE.zip universal_bundle
	rm -rf universal_bundle
	@echo "SUCCESS: hanoiTower_COMPLETE.zip created!"

clean:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -f *.tar.gz *.zip
