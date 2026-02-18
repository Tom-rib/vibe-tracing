#!/bin/bash
# start.sh - Start ImageGen services
#
# Usage:
#   ./start.sh              # Start both backend and CLI
#   ./start.sh backend      # Start only backend
#   ./start.sh cli          # Start only CLI
#   ./start.sh clean        # Clean data
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m'

# Configuration
BACKEND_PORT=5000
BACKEND_HOST="0.0.0.0"

print_header() {
    echo -e "${BLUE}"
    echo "╔════════════════════════════════════════╗"
    echo "║         ImageGen - Start Services      ║"
    echo "╚════════════════════════════════════════╝"
    echo -e "${NC}"
}

check_python() {
    if ! command -v python3 &> /dev/null; then
        echo -e "${RED}❌ Python 3 not found${NC}"
        echo "Please install Python 3.8+"
        exit 1
    fi
    
    echo -e "${GREEN}✓ Python 3 found${NC}"
}

check_dependencies() {
    echo -e "${BLUE}Checking dependencies...${NC}"
    
    if ! python3 -c "import fastapi" 2>/dev/null; then
        echo -e "${RED}❌ Dependencies missing${NC}"
        echo "Installing..."
        pip install -r requirements.txt
    else
        echo -e "${GREEN}✓ Dependencies installed${NC}"
    fi
}

start_backend() {
    echo ""
    echo -e "${BLUE}Starting Backend API...${NC}"
    echo -e "   Host: ${BACKEND_HOST}"
    echo -e "   Port: ${BACKEND_PORT}"
    echo -e "   Docs: http://localhost:${BACKEND_PORT}/docs"
    echo ""
    
    python3 backend/app.py --host "$BACKEND_HOST" --port "$BACKEND_PORT"
}

start_cli() {
    echo ""
    echo -e "${BLUE}ImageGen CLI ready!${NC}"
    echo ""
    echo "Usage:"
    echo "  python3 cli/main.py \"A red cube\""
    echo "  python3 cli/main.py \"A blue sphere\" --animate"
    echo "  python3 cli/main.py \"A scene\" --output result.png"
    echo ""
    
    # Show examples
    show_examples
}

show_examples() {
    echo -e "${GREEN}Quick Examples:${NC}"
    echo ""
    echo "1. Simple shape:"
    echo "   python3 cli/main.py \"Un cube bleu\""
    echo ""
    echo "2. With animation:"
    echo "   python3 cli/main.py \"Une sphère qui tourne\" --animate"
    echo ""
    echo "3. Multiple objects:"
    echo "   python3 cli/main.py \"Un cube rouge et une sphère bleue\""
    echo ""
    echo "4. Export to PNG:"
    echo "   python3 cli/main.py \"Un torus\" --output scene.png"
    echo ""
}

wait_for_backend() {
    echo ""
    echo -e "${BLUE}Waiting for backend...${NC}"
    
    max_attempts=30
    attempt=0
    
    while [ $attempt -lt $max_attempts ]; do
        if curl -s http://localhost:${BACKEND_PORT}/api/health > /dev/null 2>&1; then
            echo -e "${GREEN}✓ Backend is ready!${NC}"
            return 0
        fi
        
        echo -n "."
        sleep 1
        attempt=$((attempt + 1))
    done
    
    echo -e "${RED}✗ Backend failed to start${NC}"
    return 1
}

clean() {
    echo -e "${BLUE}Cleaning data...${NC}"
    
    rm -rf data/
    rm -rf __pycache__ backend/__pycache__ cli/__pycache__
    rm -f *.pyc
    
    echo -e "${GREEN}✓ Cleaned${NC}"
}

# Main
main() {
    print_header
    
    check_python
    check_dependencies
    
    case "${1:-both}" in
        backend)
            start_backend
            ;;
        cli)
            wait_for_backend
            start_cli
            ;;
        both)
            # Start backend in background
            start_backend &
            BACKEND_PID=$!
            
            # Wait and start CLI
            sleep 2
            if wait_for_backend; then
                start_cli
            fi
            
            # Cleanup on exit
            trap "kill $BACKEND_PID 2>/dev/null" EXIT
            wait
            ;;
        clean)
            clean
            ;;
        *)
            echo "Usage: $0 [backend|cli|both|clean]"
            echo ""
            echo "Examples:"
            echo "  $0 backend    - Start backend API only"
            echo "  $0 cli        - Start CLI only (requires backend)"
            echo "  $0 both       - Start both (default)"
            echo "  $0 clean      - Clean data and cache"
            ;;
    esac
}

main "$@"
