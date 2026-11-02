# Build everything from the repository root:  make
# Then run the CLI:                           ./cli/cli
DIRS = llist crypto blockchain/v0.1 blockchain/v0.2 blockchain/v0.3 cli

.PHONY: all clean $(DIRS)

all: $(DIRS)

$(DIRS):
	$(MAKE) -C $@

crypto: llist
blockchain/v0.1 blockchain/v0.2 blockchain/v0.3: crypto llist
cli: blockchain/v0.3

clean:
	for d in $(DIRS); do $(MAKE) -C $$d clean; done
