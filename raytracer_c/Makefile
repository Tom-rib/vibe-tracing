CC := gcc
CFLAGS := -O3 -march=native -std=c99 -Wall -Wextra -ffast-math
DEBUGFLAGS := -g -O0 -Wall -Wextra
LDFLAGS := -lm -lpthread

SRCDIR := src
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
BINDIR := $(BUILDDIR)/bin

# Find all .c files (except main.c for now, handled separately)
SOURCES := $(filter-out $(SRCDIR)/main.c, $(shell find $(SRCDIR) -name "*.c"))
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
MAIN_OBJ := $(OBJDIR)/main.o

TARGET := $(BINDIR)/raytracer

# Default target
all: $(TARGET)

# Main executable
$(TARGET): $(OBJECTS) $(MAIN_OBJ)
	@mkdir -p $(BINDIR)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "✓ Build successful: $@"

# Object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS := $(DEBUGFLAGS)
debug: clean all

# Clean
clean:
	@echo "Cleaning..."
	@rm -rf $(BUILDDIR)
	@rm -f *.ppm gmon.out profile.txt
	@echo "✓ Clean complete"

# Profile build
profile: CFLAGS += -pg
profile: clean all

# Run with profiling
profile-run: profile
	@echo "Running with profiling..."
	@time ./$(TARGET) output.ppm
	@echo ""
	@echo "Generating profile report..."
	gprof ./$(TARGET) gmon.out > profile.txt
	@echo "✓ Profile saved to profile.txt"
	@cat profile.txt | head -30

# Normal run
run: all
	@echo "Running raytracer..."
	@time ./$(TARGET) output.ppm
	@echo "✓ Image saved to output.ppm"

# Help
help:
	@echo "Raytracer Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make all          - Build optimized raytracer"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make run          - Build and run raytracer"
	@echo "  make profile      - Build with profiling enabled"
	@echo "  make profile-run  - Profile the raytracer"
	@echo "  make help         - Show this message"
	@echo ""
	@echo "Compiler: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Target: $(TARGET)"

.PHONY: all debug clean run profile profile-run help
