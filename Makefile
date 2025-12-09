MAKE = make
ENGINE_DIR = engine
CLIENT_DIR = client
# Nome dell'archivio finale
ARCHIVE_NAME = hanoiTower_release.tar.gz

.PHONY: all build_engine build_client test package clean cross_win

# Default: costruisce tutto per il sistema attuale (Linux/Mac)
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
	@echo "=== Packaging ==="
	$(MAKE) -C $(CLIENT_DIR) package
	# Sposta l'archivio nella root
	mv $(CLIENT_DIR)/*.tar.gz . || mv $(CLIENT_DIR)/*.zip . 2>/dev/null || true

# --- COMANDO SPECIALE PER GITLAB (Cross-Compile Windows) ---
cross_win:
	@echo "=== Cross Compiling for Windows ==="
	
	# 1. Compila Engine per Windows
	$(MAKE) -C $(ENGINE_DIR) clean
	# FIX: Aggiunti i percorsi -I per GLM e FreeGLUT
	$(MAKE) -C $(ENGINE_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=libengine.dll CXX_FLAGS="-c -O2 -std=c++20 -Dfreeglut_static -I../dependencies/glm/include -I../dependencies/freeglut/include" LIBS="-lfreeglut -lopengl32 -lglu32" all
	
	# 2. Compila Client per Windows
	$(MAKE) -C $(CLIENT_DIR) clean
	# FIX: Aggiunti i percorsi -I per GLM, FreeGLUT e Engine
	$(MAKE) -C $(CLIENT_DIR) CXX=x86_64-w64-mingw32-g++ TARGET=hanoiTower.exe ENGINE_LIB_FILENAME=libengine.dll CXX_FLAGS="-c -O2 -std=c++20 -I../engine -I../dependencies/glm/include -I../dependencies/freeglut/include" LDFLAGS="-L../engine -lengine -lfreeglut -lopengl32 -lglu32 -static-libgcc -static-libstdc++" all
	
	# 3. Pacchettizza per Windows (Zip)
	mkdir -p windows_dist
	cp $(CLIENT_DIR)/hanoiTower.exe windows_dist/
	cp $(ENGINE_DIR)/libengine.dll windows_dist/
	# Scarica freeglut.dll necessaria
	wget -q -O windows_dist/freeglut.dll https://github.com/transmission/transmission/raw/main/third-party/freeglut/freeglut.dll || echo "Warning: freeglut.dll download failed"
	cd windows_dist && zip -r ../hanoiTower_win.zip .
	rm -rf windows_dist

clean:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -f *.tar.gz *.zip
