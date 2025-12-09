# Makefile Root

# Nome del pacchetto generato dal client (senza _release)
PKG_NAME = hanoiTower.tar.gz
# Nome finale del pacchetto Linux completo
FINAL_PKG_NAME = hanoiTower_linux_complete.tar.gz

.PHONY: all clean engine client package universal test

# Target predefinito
all: engine client

# Compila l'engine
engine:
	$(MAKE) -C engine all

# Compila il client
client: engine
	$(MAKE) -C client all

# Esegue i test dell'engine
test: engine
	$(MAKE) -C engine test

# Target Clean
clean:
	$(MAKE) -C engine clean
	$(MAKE) -C client clean
	rm -rf *.tar.gz *.zip temp_pkg universal_bundle

# Target Package: compila, pacchettizza e prepara il pacchetto finale
package:
	@echo "=== Building Engine ==="
	$(MAKE) -C engine all
	@echo "=== Running Engine Tests ==="
	$(MAKE) -C engine test
	@echo "=== Building Client ==="
	$(MAKE) -C client all
	@echo "=== Packaging Linux ==="
	$(MAKE) -C client package
	# Sposta il pacchetto generato nella root
	mv client/$(PKG_NAME) . 2>/dev/null || true
	
	# --- FIX LINUX: INIEZIONE DIPENDENZE E CREAZIONE PACCHETTO FINALE ---
	@echo "=== Finalizing Linux Package ==="
	rm -rf temp_pkg
	mkdir -p temp_pkg
	# QUI ERA L'ERRORE: ora usa il nome corretto $(PKG_NAME)
	tar -xf $(PKG_NAME) -C temp_pkg
	
	# Qui puoi aggiungere comandi extra se devi copiare altre lib (es. freeimage)
	# cp /usr/lib/x86_64-linux-gnu/libfreeimage.so.3 temp_pkg/ || true
	
	# Ricrea l'archivio finale
	tar -C temp_pkg -zcvf $(FINAL_PKG_NAME) .
	@echo "Pacchetto finale creato: $(FINAL_PKG_NAME)"
	
	# Pulizia
	rm -rf temp_pkg

# Target Universal (dal log sembra essere quello chiamato dalla CI)
universal:
	@echo "=== Creating Universal Bundle ==="
	rm -rf universal_bundle
	mkdir -p universal_bundle/linux
	mkdir -p universal_bundle/windows
	
	# 1. Pulisci tutto
	$(MAKE) clean
	
	# 2. Crea pacchetto Linux
	$(MAKE) package
	
	# (Opzionale) Se avessi cross-compilazione Windows la aggiungeresti qui
	# $(MAKE) package_windows ...
