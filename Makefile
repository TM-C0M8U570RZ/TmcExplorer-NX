# Define subdirectories
SUBDIRS := External/Plutonium app

.PHONY: all clean $(SUBDIRS)

# Default target runs all subdirectories
all: $(SUBDIRS)

# Core logic: jump into each directory and run make
$(SUBDIRS):
	$(MAKE) -C $@

# Enforce order if 'app' depends on 'lib' to compile first
app: External/Plutonium

# Clean all subdirectories
clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
