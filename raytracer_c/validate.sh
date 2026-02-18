#!/bin/bash
# validate.sh - Complete validation of raytracer project
#
# Checks:
# 1. All files exist
# 2. Code compiles without warnings
# 3. Unit tests pass
# 4. Renders successfully
# 5. Output is valid PPM
#

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

PASS=0
FAIL=0

check() {
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓${NC} $1"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}✗${NC} $1"
        FAIL=$((FAIL + 1))
    fi
}

echo -e "${BLUE}===============================================${NC}"
echo -e "${BLUE}  Raytracer - Complete Validation${NC}"
echo -e "${BLUE}===============================================${NC}"
echo ""

# 1. File structure
echo -e "${BLUE}[1/5] Checking file structure...${NC}"

declare -a REQUIRED_FILES=(
    "README.md"
    "SPECIFICATION.md"
    "COPILOT.md"
    "PROJECT_STRUCTURE.md"
    "GETTING_STARTED.md"
    "GIT_WORKFLOW.md"
    "Makefile"
    ".gitignore"
    "build.sh"
    "test.sh"
    "test_phase1.c"
    "src/config.h"
    "src/main.c"
    "src/math/vec3.h"
    "src/math/vec3.c"
    "src/core/ray.h"
    "src/core/ray.c"
    "src/core/camera.h"
    "src/core/camera.c"
    "src/core/image.h"
    "src/core/image.c"
    "src/core/color.h"
    "src/core/material.h"
    "src/primitives/sphere.h"
    "src/primitives/sphere.c"
    "src/utils/random.h"
    "src/utils/random.c"
    "src/utils/allocator.h"
    "src/utils/allocator.c"
)

for file in "${REQUIRED_FILES[@]}"; do
    if [ -f "$file" ]; then
        echo -e "  ${GREEN}✓${NC} $file"
    else
        echo -e "  ${RED}✗${NC} $file (MISSING)"
        FAIL=$((FAIL + 1))
    fi
done

echo ""

# 2. Compilation
echo -e "${BLUE}[2/5] Compiling...${NC}"

make clean > /dev/null 2>&1
make > compile.log 2>&1
check "Compilation without errors"

if [ -f "build/bin/raytracer" ]; then
    echo -e "  ${GREEN}✓${NC} Executable created: build/bin/raytracer"
    PASS=$((PASS + 1))
else
    echo -e "  ${RED}✗${NC} Executable not created"
    FAIL=$((FAIL + 1))
fi

echo ""

# 3. Unit tests
echo -e "${BLUE}[3/5] Running unit tests...${NC}"

./test.sh phase1 > test.log 2>&1
check "Unit tests pass"

if [ -f "test_phase1" ]; then
    TEST_OUTPUT=$(./test_phase1 2>&1)
    if echo "$TEST_OUTPUT" | grep -q "Failed: 0"; then
        echo -e "  ${GREEN}✓${NC} All unit tests passed"
        PASS=$((PASS + 1))
    fi
fi

echo ""

# 4. Render test
echo -e "${BLUE}[4/5] Running render test...${NC}"

timeout 10 ./build/bin/raytracer output.ppm > render.log 2>&1
check "Render completes successfully"

echo ""

# 5. Output validation
echo -e "${BLUE}[5/5] Validating output...${NC}"

if [ -f "output.ppm" ]; then
    echo -e "  ${GREEN}✓${NC} output.ppm exists"
    PASS=$((PASS + 1))
    
    # Check PPM format
    if head -c 2 output.ppm | grep -q "P6"; then
        echo -e "  ${GREEN}✓${NC} Valid PPM P6 format"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}✗${NC} Invalid PPM format"
        FAIL=$((FAIL + 1))
    fi
    
    # Check file size (should be roughly 800*600*3 bytes + header)
    SIZE=$(stat -f%z output.ppm 2>/dev/null || stat -c%s output.ppm 2>/dev/null || echo 0)
    if [ $SIZE -gt 1000000 ] && [ $SIZE -lt 2000000 ]; then
        echo -e "  ${GREEN}✓${NC} File size reasonable ($SIZE bytes)"
        PASS=$((PASS + 1))
    else
        echo -e "  ${YELLOW}!${NC} File size unexpected ($SIZE bytes)"
    fi
else
    echo -e "  ${RED}✗${NC} output.ppm not created"
    FAIL=$((FAIL + 1))
fi

echo ""
echo -e "${BLUE}===============================================${NC}"
echo -e "${BLUE}  Summary${NC}"
echo -e "${BLUE}===============================================${NC}"
echo -e "Checks passed: ${GREEN}$PASS${NC}"
echo -e "Checks failed: ${RED}$FAIL${NC}"

if [ $FAIL -eq 0 ]; then
    echo -e "\n${GREEN}✓ All validations passed!${NC}"
    echo -e "${GREEN}Phase 1 is ready.${NC}\n"
    exit 0
else
    echo -e "\n${RED}✗ Some validations failed.${NC}"
    echo -e "See logs: compile.log, test.log, render.log\n"
    exit 1
fi
