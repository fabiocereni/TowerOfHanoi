# --- MAKEFILE ROOT ---
MAKE = make
ENGINE_DIR = engine
CLIENT_DIR = client

# Nomi dei file di output attesi (per pulizia)
ARCHIVE_NAME = hanoiTower.tar.gz

.PHONY: all build_engine build_client test package clean

# Workflow completo per la CI: Compila -> Testa -> Pacchettizza
all: package

# 1. Costruisce l'Engine (Backend)
build_engine:
	@echo "=== Building Engine ==="
	$(MAKE) -C $(ENGINE_DIR) all

# 2. Costruisce il Client (Frontend) - Dipende dall'Engine
build_client: build_engine
	@echo "=== Building Client ==="
	$(MAKE) -C $(CLIENT_DIR) all

# 3. Esegue i Test dell'Engine (Bloccante se fallisce)
test: build_engine
	@echo "=== Running Engine Tests ==="
	$(MAKE) -C $(ENGINE_DIR) test

# 4. Packaging (Crea il .tar.gz)
# Esegue il packaging dentro la cartella client e poi sposta il file qui
package: test build_client
	@echo "=== Packaging ==="
	$(MAKE) -C $(CLIENT_DIR) package
	# Sposta l'archivio nella root per facilitare l'artifact di GitLab
	mv $(CLIENT_DIR)/$(ARCHIVE_NAME) .

# Pulizia ricorsiva
clean:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -f $(ARCHIVE_NAME)
