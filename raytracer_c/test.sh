#!/bin/bash
# test.sh - Run unit tests for raytracer
#
# Usage: ./test.sh [test_name]
#

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

TEST=${1:-phase1}

case "$TEST" in
    phase1)
        echo "Building Phase 1 tests..."
        gcc -O2 -std=c99 -Wall -Wextra -lm \
            src/math/vec3.c \
            src/core/ray.c \
            src/primitives/sphere.c \
            test_phase1.c \
            -o test_phase1
        
        if [ $? -ne 0 ]; then
            echo -e "${RED}Compilation failed${NC}"
            exit 1
        fi
        
        echo "Running Phase 1 tests..."
        ./test_phase1
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✓ All tests passed!${NC}"
        else
            echo -e "${RED}✗ Some tests failed${NC}"
            exit 1
        fi
        ;;
    
    all)
        echo "Running all tests..."
        ./test.sh phase1
        ;;
    
    clean)
        echo "Cleaning tests..."
        rm -f test_phase1 test_*.o
        ;;
    
    *)
        echo "Unknown test: $TEST"
        echo "Available: phase1, all, clean"
        exit 1
        ;;
esac

exit 0
