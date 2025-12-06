# Makefile for Counter C Extension

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Python configuration
PYTHON = python3
PYTHON_CONFIG = python3-config
PYTHON_INCLUDES = $(shell $(PYTHON_CONFIG) --includes)
PYTHON_LDFLAGS = $(shell $(PYTHON_CONFIG) --ldflags)

# Targets
.PHONY: all clean test install build standalone

all: build

# Build the Python extension in-place
build:
	$(PYTHON) setup.py build_ext --inplace

# Install the extension module
install:
	$(PYTHON) setup.py install --user

# Build standalone C test
standalone: counter
	@echo "Built standalone C test program"

counter: counter.c
	$(CC) $(CFLAGS) -DSTANDALONE_TEST counter.c -o counter

# Run tests
test: build
	$(PYTHON) test_counter.py

# Test standalone C version
test-standalone: standalone
	./counter

# Clean build artifacts
clean:
	rm -rf build/
	rm -rf *.so
	rm -rf *.egg-info
	rm -f counter
	rm -rf __pycache__
	find . -name "*.pyc" -delete
	find . -name "*.o" -delete

# Help message
help:
	@echo "Available targets:"
	@echo "  make build           - Build the Python extension in-place"
	@echo "  make install         - Install the extension module"
	@echo "  make standalone      - Build standalone C test program"
	@echo "  make test            - Run Python tests"
	@echo "  make test-standalone - Run standalone C test"
	@echo "  make clean           - Remove build artifacts"
	@echo "  make help            - Show this help message"
