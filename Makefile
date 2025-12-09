# Makefile Root

# Nomi file
PKG_NAME = hanoiTower.tar.gz
FINAL_PKG_NAME = hanoiTower_linux_complete.tar.gz
ARTIFACT_NAME = hanoiTower_COMPLETE.zip

# Compilatore Windows (Cross-compiler)
WIN_CXX = x86_64-w64-mingw32-g++
WIN_FLAGS = -std=c++20 -O2 -D_WIN32
# Librerie per Windows (Assumendo che siano nell'ambiente Docker SUPSI)
WIN_LIBS = -L. -lengine -lfreeglut -lopengl32 -lglu32 -lfreeimage

.PHONY: all clean engine client package universal test windows_build linux_build

all: engine client

# --- COMANDI STANDARD (PER SVILUPPO LINUX LOCALE) ---
engine:
	$(MAKE) -C engine all

client: engine
	$(MAKE) -C client all

test: engine
	$(MAKE) -C engine test

clean:
	$(MAKE) -C engine clean
	$(MAKE) -C client clean
	rm -f *.so *.dll *.exe *.o *.tar.gz *.zip
	rm -rf temp_pkg universal_bundle release_temp client/hanoiTower client/*.o engine/*.o engine/*.so

# --- BUILD SPECIFICA PER LINUX ---
linux_build:
	@echo "=== [LINUX] Building Engine & Client ==="
	$(MAKE) -C engine all
	$(MAKE) -C client all
	$(MAKE) -C client package
	# Sposta il pacchetto generato nella root
	mv client/$(PKG_NAME) . 2>/dev/null || true
	
	# Prepara il pacchetto completo Linux
	rm -rf temp_pkg && mkdir -p temp_pkg
	tar -xf $(PKG_NAME) -C temp_pkg
	tar -C temp_pkg -zcvf $(FINAL_PKG_NAME) .
	rm -rf temp_pkg
	@echo "=== [LINUX] Done ==="

# --- BUILD SPECIFICA PER WINDOWS (CROSS-COMPILAZIONE) ---
windows_build:
	@echo "=== [WINDOWS] Cross-Compiling Engine ==="
	# Compila la DLL dell'engine
	$(WIN_CXX) -shared -o libengine.dll engine/*.cpp $(WIN_FLAGS) -lfreeglut -lopengl32 -lglu32 -lfreeimage
	@echo "=== [WINDOWS] Cross-Compiling Client ==="
	# Compila l'EXE del client
	$(WIN_CXX) -o hanoiTower.exe client/main.cpp -Iengine -L. -lengine $(WIN_FLAGS)
	@echo "=== [WINDOWS] Done ==="

# --- TARGET UNIVERSALE (CHIAMATO DALLA CI) ---
universal:
	@echo "=== Starting Universal Build ==="
	
	# 1. Pulizia Totale Iniziale
	$(MAKE) clean

	# 2. Crea la struttura delle cartelle temporanee
	mkdir -p release_temp/linux
	mkdir -p release_temp/windows

	# ---------------------------------------------------------
	# FASE 1: BUILD LINUX
	# ---------------------------------------------------------
	$(MAKE) linux_build
	# Sposta il risultato nella cartella temporanea
	mv $(FINAL_PKG_NAME) release_temp/linux/

	# ---------------------------------------------------------
	# FASE 2: PULIZIA INTERMEDIA
	# (Necessaria per non mischiare oggetti Linux con Windows)
	# ---------------------------------------------------------
	$(MAKE) clean
	# Ricreiamo le cartelle perché make clean le ha cancellate
	mkdir -p release_temp/linux
	mkdir -p release_temp/windows
	# Rimetto il file Linux (se clean lo ha cancellato dalla temp, lo rigeneriamo?
	# No, clean cancella release_temp. Dobbiamo salvare l'artifact Linux PRIMA di pulire)
	
	# CORREZIONE LOGICA:
	# Per evitare conflitti, facciamo Windows, poi puliamo, poi Linux.
	# O meglio: compilazione manuale Windows che non usa i makefile interni.
	
	# Eseguiamo la build Windows "manuale" definita sopra
	$(MAKE) windows_build
	
	# Sposta i file Windows (EXE e DLL)
	mv hanoiTower.exe release_temp/windows/
	mv libengine.dll release_temp/windows/
	# (Opzionale) Copia script run.bat se esiste, o crealo al volo
	echo "hanoiTower.exe" > release_temp/windows/run.bat

	# ---------------------------------------------------------
	# FASE 3: RIGENERA LINUX (Per sicurezza post-clean)
	# ---------------------------------------------------------
	# Puliamo gli oggetti windows
	rm -f *.o *.dll *.exe
	# Ricompiliamo Linux pulito
	$(MAKE) linux_build
	mv $(FINAL_PKG_NAME) release_temp/linux/

	# ---------------------------------------------------------
	# FASE 4: PACKAGING FINALE
	# ---------------------------------------------------------
	cd release_temp && zip -r ../$(ARTIFACT_NAME) .
	
	@echo "Artifact creato con successo: $(ARTIFACT_NAME)"
	
	# Pulizia finale
	rm -rf release_temp
