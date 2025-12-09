# --- MAKEFILE ROOT (Universal v2: Linux, Mac, Windows) ---
MAKE = make
ENGINE_DIR = engine
CLIENT_DIR = client

# Nome archivio
ARCHIVE_NAME = hanoiTower.tar.gz

.PHONY: all build_engine build_client test package clean

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
	mv $(CLIENT_DIR)/$(ARCHIVE_NAME) .

clean:
	$(MAKE) -C $(ENGINE_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
	rm -f $(ARCHIVE_NAME)
