# Makefile Root

PKG_NAME = hanoiTower.tar.gz
FINAL_PKG_NAME = hanoiTower_linux_complete.tar.gz
ARTIFACT_NAME = hanoiTower_COMPLETE.zip

.PHONY: all clean engine client package universal test

all: engine client

engine:
	$(MAKE) -C engine all

client: engine
	$(MAKE) -C client all

test: engine
	$(MAKE) -C engine test

clean:
	$(MAKE) -C engine clean
	$(MAKE) -C client clean
	rm -rf *.tar.gz *.zip temp_pkg universal_bundle release_temp

package:
	@echo "=== Building Engine ==="
	$(MAKE) -C engine all
	@echo "=== Building Client ==="
	$(MAKE) -C client all
	@echo "=== Packaging Linux ==="
	$(MAKE) -C client package
	# Sposta il pacchetto generato nella root
	mv client/$(PKG_NAME) . 2>/dev/null || true
	
	# --- FIX LINUX ---
	rm -rf temp_pkg
	mkdir -p temp_pkg
	# Estrae usando il nome corretto
	tar -xf $(PKG_NAME) -C temp_pkg
	# Ricrea l'archivio .tar.gz finale
	tar -C temp_pkg -zcvf $(FINAL_PKG_NAME) .
	rm -rf temp_pkg

# Target chiamato dalla CI
universal:
	@echo "=== Creating Universal Bundle ==="
	
	# 1. Pulizia INIZIALE (così non cancelliamo le cartelle che stiamo per creare)
	$(MAKE) clean

	# 2. Crea la struttura delle directory
	mkdir -p release_temp/linux
	
	# 3. Crea il pacchetto Linux
	$(MAKE) package
	
	# 4. Sposta il .tar.gz finale nella cartella temporanea
	mv $(FINAL_PKG_NAME) release_temp/linux/
	
	# 5. CREA LO ZIP RICHIESTO DALLA CI
	cd release_temp && zip -r ../$(ARTIFACT_NAME) .
	
	@echo "Artifact creato con successo: $(ARTIFACT_NAME)"
	
	# Pulizia finale (opzionale, rimuove solo la temp folder)
	rm -rf release_temp
