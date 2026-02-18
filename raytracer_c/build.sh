#!/bin/bash
# build.sh - Build script for raytracer
#
# Usage: ./build.sh [target]
# Targets: all, clean, run, profile, debug
#

set -e  # Exit on first error

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Compiler settings
CC=${CC:-gcc}
CFLAGS_RELEASE="-O3 -march=native -std=c99 -Wall -Wextra -ffast-math"
CFLAGS_DEBUG="-O0 -g -std=c99 -Wall -Wextra"
LDFLAGS="-lm -lpthread"

# Directories
BUILD_DIR="build"
BIN_DIR="$BUILD_DIR/bin"
OBJ_DIR="$BUILD_DIR/obj"

# Target
TARGET=${1:-all}

# Helper functions
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

# Functions for each target
build_release() {
    print_info "Building release version..."
    
    mkdir -p "$OBJ_DIR" "$BIN_DIR"
    
    # Find all source files
    SOURCES=$(find src -name "*.c" -type f | grep -v main.c)
    MAIN_SRC="src/main.c"
    
    # Compile
    print_info "Compiling with $CC $CFLAGS_RELEASE..."
    
    OBJECTS=""
    for src in $SOURCES; do
        obj="$OBJ_DIR/${src//\//_}.o"
        if [ ! -f "$obj" ] || [ "$src" -nt "$obj" ]; then
            print_info "  Compiling: $src"
            $CC $CFLAGS_RELEASE -c "$src" -o "$obj"
        fi
        OBJECTS="$OBJECTS $obj"
    done
    
    # Compile main
    main_obj="$OBJ_DIR/main.o"
    if [ ! -f "$main_obj" ] || [ "$MAIN_SRC" -nt "$main_obj" ]; then
        print_info "  Compiling: $MAIN_SRC"
        $CC $CFLAGS_RELEASE -c "$MAIN_SRC" -o "$main_obj"
    fi
    
    # Link
    print_info "Linking..."
    $CC $CFLAGS_RELEASE $OBJECTS "$main_obj" $LDFLAGS -o "$BIN_DIR/raytracer"
    
    print_info "Build successful: $BIN_DIR/raytracer"
}

build_debug() {
    print_info "Building debug version..."
    
    mkdir -p "$OBJ_DIR" "$BIN_DIR"
    
    SOURCES=$(find src -name "*.c" -type f | grep -v main.c)
    MAIN_SRC="src/main.c"
    
    print_info "Compiling with $CC $CFLAGS_DEBUG..."
    
    OBJECTS=""
    for src in $SOURCES; do
        obj="$OBJ_DIR/${src//\//_}.o"
        print_info "  Compiling: $src"
        $CC $CFLAGS_DEBUG -c "$src" -o "$obj"
        OBJECTS="$OBJECTS $obj"
    done
    
    main_obj="$OBJ_DIR/main.o"
    print_info "  Compiling: $MAIN_SRC"
    $CC $CFLAGS_DEBUG -c "$MAIN_SRC" -o "$main_obj"
    
    print_info "Linking..."
    $CC $CFLAGS_DEBUG $OBJECTS "$main_obj" $LDFLAGS -o "$BIN_DIR/raytracer"
    
    print_info "Debug build successful: $BIN_DIR/raytracer"
}

clean() {
    print_info "Cleaning..."
    rm -rf "$BUILD_DIR"
    rm -f output.ppm gmon.out profile.txt
    print_info "Clean complete"
}

run() {
    build_release
    print_info "Running raytracer..."
    time "$BIN_DIR/raytracer" output.ppm
    print_info "Output saved to output.ppm"
    
    # Try to open image viewer
    if command -v feh &> /dev/null; then
        feh output.ppm &
    elif command -v eog &> /dev/null; then
        eog output.ppm &
    elif command -v open &> /dev/null; then
        open output.ppm &
    else
        print_warn "No image viewer found. Open output.ppm manually."
    fi
}

profile() {
    print_info "Building with profiling..."
    
    mkdir -p "$OBJ_DIR" "$BIN_DIR"
    
    SOURCES=$(find src -name "*.c" -type f | grep -v main.c)
    MAIN_SRC="src/main.c"
    
    PROF_FLAGS="$CFLAGS_RELEASE -pg"
    print_info "Compiling with $CC $PROF_FLAGS..."
    
    OBJECTS=""
    for src in $SOURCES; do
        obj="$OBJ_DIR/${src//\//_}.o"
        print_info "  Compiling: $src"
        $CC $PROF_FLAGS -c "$src" -o "$obj"
        OBJECTS="$OBJECTS $obj"
    done
    
    main_obj="$OBJ_DIR/main.o"
    print_info "  Compiling: $MAIN_SRC"
    $CC $PROF_FLAGS -c "$MAIN_SRC" -o "$main_obj"
    
    print_info "Linking..."
    $CC $PROF_FLAGS $OBJECTS "$main_obj" $LDFLAGS -o "$BIN_DIR/raytracer"
    
    print_info "Running with profiling..."
    time "$BIN_DIR/raytracer" output.ppm
    
    print_info "Generating profile report..."
    gprof "$BIN_DIR/raytracer" gmon.out > profile.txt 2>&1
    print_info "Profile saved to profile.txt"
    cat profile.txt | head -20
}

# Main
case "$TARGET" in
    all|"")
        build_release
        ;;
    debug)
        build_debug
        ;;
    clean)
        clean
        ;;
    run)
        run
        ;;
    profile)
        clean
        profile
        ;;
    *)
        print_error "Unknown target: $TARGET"
        echo "Available targets: all, debug, clean, run, profile"
        exit 1
        ;;
esac

exit 0
